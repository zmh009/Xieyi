#include "ZRestore.h"
//time_t ZRestore::mIndex = 0;

ZRestore::ZRestore()
{
    mBuffer = ZBuffer::getBuffer();
    mBufferHandler = mBuffer->openRead();
    mRun = true;
}

ZRestore::~ZRestore()
{
    mBuffer->closeRead(mBufferHandler);
}

int ZRestore::restoreToFile(std::__cxx11::string dirPath)
{
    // 提供默认存储路径ZRestoreLocation
    if (dirPath.empty())
    {
        dirPath = "./ZRestoreLocation";
    }

    // 获取执行时的函数返回值，在结束时反馈外部调用以检测是否有错误发生
    int ret = 0;
    // 获取文件交互接口
    ZFile file;
    // stop方法会将mRun置为false，因此在每次start里将mRun置为true
    mRun = true;
    while (mRun)
    {
        string networkData = mBuffer->read(mBufferHandler);
        if (networkData.empty())
        {
            continue;
        }
        ZItem item = getItem(networkData);

        if (!item.mData.empty())
        {
            // 文件名格式：指定目录/目的IP/目的端口/recive/源IP/源端口/序号.数据类型.加密类型.压缩类型
            string filePath = dirPath + '/'
                                + item.mDstIp + '/' + std::to_string(item.mDstPort) + '/'
                                + "receive/"
                                + item.mSrcIp + '/' + std::to_string(item.mSrcPort) + '/'
                                + item.mSequence + '.' + item.mDataType;
            // 加密类型与压缩类型可能为空
            filePath += item.mEncodeType.empty() ?
                        "" : ('.' + item.mEncodeType);
            filePath += item.mCompressionType.empty() ?
                        "" : ('.' + item.mCompressionType);

            if ( -1 == file.write(filePath, item.mData, /*item.mIsBinary*/true)) // <--使用dstIP+dstPort+srcIP+srcPort做索引，tcp 的fin为tcp协议的删除标志，其他协议不存储历史文件名
            {
                ret = -1;
                mError = file.getError();
                break;
            }
        }
    }

    // 在运行结束后不会捕获到后续的会话结束报文，可能会占用额外空间与产生数据误差，所以运行结束后要清除历史记录
    clearHistory();
    return ret;
}

/* 存储数据库位置为：库名/dataContent、库名/dataItem；
 * 数据表列：
 * dataContent：index(time)、 sequence（time）、data（blob）；
 * dataItem：sequence(time)、dataType(varchar(50))、encodeType(varchar(5))、compressionType(varchar(5))、
 *           dstIp（varchar(15)）、dstPort（short_int）、srcIp（varchar(15)）、srcPort（short_int）；
 * 存储时不重组数据包，因为数据可能累积超出数据表的设置上限，数据内容与序号是一对多关系，添加index为主键，时间戳类型
 * 序号和数据类型、加密类型、压缩类型、目的IP、目的端口、源IP、源端口是一对一关系，设置sequence为主键
 * 所以使用序号关联一组数据明显，同时dataContent的序号sequence为dataItem的外键。
 */
int ZRestore::restoreToSQL(std::__cxx11::string dbName)
{
    // 提供默认存储路径ZRestoreLocation
    if (dbName.empty())
    {
        dbName = "ZRestoreLocation";
    }

    // 获取执行时的函数返回值，在结束时反馈外部调用以检测是否有错误发生
    int ret = 0;
    // 获得SQL交互接口
    ZSQL sql;
    // 连接到数据库
    // 存储数据的数据库和数据表要提前创建，在捕获数据后可直接存入
    if (0 != sql.connect("root", "westos", dbName))
    {
        mError = sql.getError();
        return -1;
    }
    // stop方法会将mRun置为false，因此在每次start里将mRun置为true
    mRun = true;
    while (mRun)
    {
        string networkData = mBuffer->read(mBufferHandler);
        if (networkData.empty())
        {
            continue;
        }

        // 获得数据明显
        ZItem item = getItem(networkData);
        // 数据为空则不操作
        if (item.mData.empty())
        {
            continue;
        }

        // 如果序号不存在于dataItem表，则需写入
        string condition = "sequence = " + item.mSequence;
        ret = sql.select("select * from dataItem where " + condition);
        if (ret == 0)
        {
            // 向dataItem表添加记录
            if (0 != sql.insert("insert dataItem values(" +
                                item.mSequence + ",'" +
                                item.mDataType + "','" +
                                (item.mEncodeType.empty() ? "-" : item.mEncodeType) + "','" +
                                (item.mCompressionType.empty() ? "-" : item.mCompressionType) + "','" +
                                item.mDstIp + "'," +
                                std::to_string(item.mDstPort) + ",'" +
                                item.mSrcIp + "'," +
                                std::to_string(item.mSrcPort) + ")")
                    )
            {
                // 有错误则记录
                mError = sql.getError();
                ret = -1;
                break;
            }
        }
        else if (ret < 0)
        {
            // 有错误则记录
            mError = sql.getError();
            break;
        }

        // 有捕获的数据就要写入dataContent表
        // 二进制数据转义
        string escape;
        if (0 != sql.binaryEscape(item.mData, escape))
        {
            mError = sql.getError();
            ret = -1;
            break;
        }
        if (0 != sql.insert("insert dataContent values(NULL, "+
                            item.mSequence + ",'" +
                            escape.c_str() +"')"
                            )
            )
        {
            // 有错误则记录
            mError = sql.getError();
            ret = -1;
            break;
        }
    }

    // 在运行结束后不会捕获到会话结束报文，可能会占用额外空间与产生数据误差，所以运行结束后要清除历史记录
    clearHistory();
    // 关闭数据库连接
    sql.close();
    return ret;
}

void ZRestore::stop()
{
    mRun = false;
}

std::__cxx11::string ZRestore::getError() const
{
    return mError;
}

ZItem ZRestore::getItem(const std::__cxx11::string &networkData)
{
    if (networkData.empty())
    {
        return ZItem();
    }

    ZItem item;

    // -------------链路层分析------------
    const char *data = networkData.data();
    mMac.setLinkContent((const u_char*)data, networkData.length());
    const u_char* networkLayoutData = mMac.networkContent();
    int networkLen = mMac.networkLen();
    int networkType = mMac.netkworkType();

    // -------------网络层分析------------
    INetwork *networkLayout;
    switch (networkType)
    {
    case IP_TYPE:
        networkLayout = &mIpv4;
        break;
    default:
        // 未知网络层协议类型,返回空明细
        return ZItem();
    }
    // 向明细里添加数据
    networkLayout->setNetworkContent(networkLayoutData, networkLen);
    item.mSrcIp = networkLayout->srcIp();
    item.mDstIp = networkLayout->dstIp();

    // -------------运输层分析------------
    ITransport *transportLayout;
    int transportType = networkLayout->transportType();
    switch (transportType)
    {
    case TCP_TYPE:
        // 如果是TCP连接，判断当前是否为结束的会话
        mTcp.setTransportContent(mIpv4.transportContent(), mIpv4.transportLen());
        if ( mTcp.sign() & FIN) // 是否为TCP一次会话的结束
        {
            item.mRecombine = ZItem::FINISH;
        }
        else
        {
            item.mRecombine = ZItem::RECOMBINEING;
        }
        transportLayout = &mTcp;
        break;
    case UDP_TYPE:
        item.mRecombine = ZItem::NOT_RECOMBINE;
        mUdp.setTransportContent(mIpv4.transportContent(), mIpv4.transportLen());
        transportLayout = &mUdp;
        break;
    default:
        item.mRecombine = ZItem::NOT_RECOMBINE;
        return ZItem(); // 非TCP、UDP协议则返回空明细
    }
    // 向明显里添加数据
    item.mSrcPort = transportLayout->srcPort();
    item.mDstPort = transportLayout->dstPort();

    // -------------应用层分析------------
    IApplication *applicationLayout = nullptr;
    if (item.mSrcPort == HTTP_PORT || item.mDstPort == HTTP_PORT)
    {
        applicationLayout = &mHttp;
    }
    else if (item.mSrcPort == HTTPS_PORT || item.mDstPort == HTTPS_PORT)
    {
        applicationLayout = &mHttps;
    }
    else
    {
        // 未知应用层协议类型
        // no operator;
        return ZItem();
    }

    // 向明显里添加数据
    const u_char *appData = transportLayout->applicationContent();
    int appLength = transportLayout->applicationLen();

    if (applicationLayout != nullptr)
    {
        // 当应用层使用已知协议时
        applicationLayout->setApplicationContent(appData, appLength);
        item.mIsBinary = applicationLayout->isBinary();
        item.mDataType = applicationLayout->dataType();
        item.mCompressionType = applicationLayout->compressionType();
        item.mEncodeType = applicationLayout->encodeType();
        item.mLength = applicationLayout->dataLen();
        const u_char* dataContent = applicationLayout->dataContent();
        item.mData = string((const char*)dataContent, item.mLength);
    }
    else
    {
        // 当应用层使用未知协议时
        item.mIsBinary = true;
        item.mDataType = TYPE_NORMAL;
        item.mCompressionType = "";
        item.mEncodeType = "";
        item.mLength = appLength;
        item.mData = string((const char*)appData, item.mLength);
    }

    if (!item.mData.empty() || item.mRecombine == ZItem::FINISH)
    {
        setRecombineIndex(item);
    }
    return item;
}

/* 通过数据索引与重组状态更新历史数据，或从历史数据里读取重组索引，同时设置该明细的重组索引，
 * 重组索引包含：数据序号、数据类型、加密类型(可能无)、压缩类型(可能无)。
 * 重组数据方法：
 * 在TCP的一次连接的多个数据包里，某个包会携带数据类型，
 * 而之后是数据内容，通过索引记录数据类型，在获得未知的数据时查看索引获取类型；
 * 如果有新的数据类型则更新索引，以支持数据重组；
 * 捕获结束状态的报文要删除历史记录，以防止数据误差。
 * @param   item    ZItem类型，存储捕获数据的明细
 */
void ZRestore::setRecombineIndex(ZItem &item)
{
    // 数据索引由数据的源IP、源端口号、目的IP、目的端口组成
    string dataIndex = item.mSrcIp+std::to_string(item.mSrcPort)+
            item.mDstIp+std::to_string(item.mDstPort);

    // 在需获取时间戳时才调用获取函数，可使在已有存储索引时避免调用
    switch (item.mRecombine)
    {
    // 当为不重组类型时，只设置序号
    case ZItem::NOT_RECOMBINE:
        item.mSequence = getUniqueTime();
        break;
    // 当为重组中时，如果存在索引则读取历史记录并设置所有类型和序号，如果不存在则创建；
    // 如果出现新的数据类型则要更新索引
    case ZItem::RECOMBINEING:
    {
        RecordT record = mHistoryRecord[dataIndex];
        if (record.empty() || !item.mDataType.empty())
        {
            // 无索引或有新数据类型时要更新索引
            item.mSequence = std::to_string(getUniqueTime());

            // 存在是新索引并且数据类型为空时的情况，如捕获到的新索引数据是当前会话正在传输的数据
            if (record.empty() && item.mDataType.empty())
            {
                item.mDataType = TYPE_OTHER;
            }

            // 更新索引
            RecordT newRecord = {{SEQUENCE_KEY, item.mSequence},
                                 {DATA_TYPE_KEY,item.mDataType},
                                 {ENCODE_TYPE_KEY, item.mEncodeType},
                                 {COMPRESSION_TYPE_KEY, item.mCompressionType}};
            mHistoryRecord[dataIndex] = newRecord;
        }
        else
        {
            // 有索引且数据类型为空，需要从历史记录里读取并设置所有类型和序号
            item.mSequence = record[SEQUENCE_KEY];
            item.mDataType = record[DATA_TYPE_KEY];
            item.mEncodeType = record[ENCODE_TYPE_KEY];
            item.mCompressionType = record[COMPRESSION_TYPE_KEY];
        }
        break;
    }
    // 当为结束时，如果存在索引且数据类型为空则要设置所有类型和序号，不存在则只创建序号，存在且数据类型不为空则无操作，最后统一删除
    case ZItem::FINISH:
    {
        RecordT record = mHistoryRecord[dataIndex];
        if (!record.empty())
        {
            // 存在索引且数据类型为空,可能在会话终止时携带数据，则需要存储该数据的详情以支持数据重组
            item.mSequence = record[SEQUENCE_KEY];
            item.mDataType = record[DATA_TYPE_KEY];
            item.mEncodeType = record[ENCODE_TYPE_KEY];
            item.mCompressionType = record[COMPRESSION_TYPE_KEY];
        }
        else
        {
            // 该终止会话是新的会话
            item.mSequence = std::to_string(getUniqueTime());
            item.mDataType = TYPE_OTHER;
        }

        mHistoryRecord.erase(dataIndex);
    }
        break;
    default:
        // 其他情况只创建序号
        item.mSequence = std::to_string(getUniqueTime());
        break;
    }
}

/* 获得唯一的时间戳，单位秒，如果短时间内获取则值向后推移
 * return   成功返回秒数，否则返回((time_t)-1)，错误原因存储在errno中
 */
time_t ZRestore::getUniqueTime()
{
    // 使用静态变量记录上一次的时间戳
    static time_t oldTime = 0;
    time_t nowTime = time(nullptr);

    if (nowTime == (time_t)-1)
    {
        return nowTime;
    }
    else if (nowTime <= oldTime)
    {
        // 如果获取的时间戳小于等于旧时间戳，则将获取的值向后推移，避免时间戳在短时间内的重复问题
        nowTime = oldTime+1;
    }
    // 更新旧时间戳,作为下一次的比较
    oldTime = nowTime;

    return nowTime;
}

/* 清除运行时产生的历史数据
 */
void ZRestore::clearHistory()
{
    mIndexPool.clear();
    mTypePool.clear();
}

ZItem::ZItem()
{
    mRecombine = ZItem::NOT_RECOMBINE;
    mIsBinary = true;
    mLength = 0;
}
