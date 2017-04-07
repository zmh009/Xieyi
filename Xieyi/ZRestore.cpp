#include "ZRestore.h"
time_t ZItem::mIndex = 0;

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

void ZRestore::restoreToFile(const std::__cxx11::string &dirPath)
{
    // 提供默认存储路径ZRestoreLocation
    const char *path = dirPath.c_str();
    if (path == nullptr)
    {
        path = "nullptr";
    }
    if (dirPath.empty())
    {
        path = "ZRestoreLocation";
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
        ZItem item = getItem(networkData);
        if (!item.mData.empty())
        {
            // 文件名格式：指定目录/目的IP/目的端口/recive/源IP/源端口/index.数据类型.加密类型.压缩类型
            string filePath = dirPath + '/'
                                + item.mDstIp + '/' + std::to_string(item.mDstPort) + '/'
                                + "receive/"
                                + item.mSrcIp + '/' + std::to_string(item.mSrcPort) + '/';
            time_t index = item.mIndex++;
            filePath += std::to_string(index) + item.mDataType + item.mEncodeType + item.mCompressionType;

            if ( -1 == mFile.write(filePath, item.mData, item.mIsBinary)) // <--使用dstIP+dstPort+srcIP+srcPort做索引，tcp 的fin为tcp协议的删除标志，其他协议不存储历史文件名
            {
                cout << "write error" << endl;
            }
//            cout << item.mData << endl;
        }
//        QThread::sleep(1);
    }
}

void ZRestore::restoreToSQL(const std::__cxx11::string &dbName)
{
    // 提供默认存储路径ZRestoreLocation
    const char *path = dbName.c_str();
    if (dbName.empty())
    {
        path = "ZRestoreLocation";
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
        QThread::sleep(1);
        qDebug() << "restore to sql in "<< path;
    }
}

void ZRestore::stop()
{
    mRun = false;
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

    // --test
//    size_t length = networkData.length();
//    for (size_t i = 0; i < length; ++i)
//    {
//        std::cout << std::setfill('0') << std::setw(2) << std::setbase(16)<< (int)(u_char)data[i] << " ";
//    }
//    std::cout << std::endl;
    // --test end
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
//        break;
    }
    // 向明细里添加数据
    networkLayout->setNetworkContent(networkLayoutData, networkLen);
    item.mSrcIp = networkLayout->srcIp();
    item.mDstIp = networkLayout->dstIp();

    // -------------运输层分析------------
    ITransport *transportLayout;
//    int ipDataType = mIpv4.getDataType();
    int transportType = networkLayout->transportType();
    switch (transportType)
    {
    case TCP_TYPE:
//        mTcp.setTransportContent(mIpv4.transportContent(), mIpv4.transportLen());
        transportLayout = &mTcp;
        break;
    case UDP_TYPE:
//        mUdp.setTransportContent(mIpv4.transportContent(), mIpv4.transportLen());
        transportLayout = &mUdp;
        break;
    default:
        cout << "transportType is " << transportType << endl;
        return ZItem(); // 非TCP、UDP协议则返回空明细
//        break;
    }
    transportLayout->setTransportContent(networkLayout->transportContent(), networkLayout->transportLen());
    // 向明显里添加数据
    item.mSrcPort = transportLayout->srcPort();
    item.mDstPort = transportLayout->dstPort();

    // -------------应用层分析------------
    IApplication *applicationLayout = nullptr;
//    switch (item.mDstPort) // and item.mSrcPort
//    {
//    case HTTP_PORT:
//        applicationLayout = &mHttp;
//        break;
//    case HTTPS_PORT:
//        applicationLayout = &mHttps;
//    default:
//        // 向明显里添加数据
//        // 未知应用层协议类型,进行返回空明细
//        return ZItem();
////        break;
//    }
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
        if (item.mData == string("\r\n")) // <-- 获得gif文件时有01、0等非gif图像数据，其他图形、音频等类型文件可能有类似情况
        {
            std::cout << "\\r\\n:"<< item.mDstIp << "/" << item.mDstPort << "/"
                      << "recive/" << item.mSrcIp << "/" << item.mDstPort<< std::endl;
        }
    }
    else
    {
        // 当应用层使用未知协议时
        item.mIsBinary = true;
        item.mDataType = NORMAL_TYPE;
        item.mCompressionType = "";
        item.mEncodeType = "";
        item.mLength = appLength;
        item.mData = string((const char*)appData, item.mLength);
    }

    // return item
    return item;
}

//void ZRestore::start(RestoreStatusE status)
//{
//    switch (status)
//    {
//    case NORESTORE:
//        qDebug() << "no restore";
//        break;
//    case RESTORE_TO_FILE:
//        qDebug() << "restore to file";
//        break;
//    case RESTORE_TO_SQL:
//        qDebug() << "restore to sql";
//        break;
//    default:
//        qDebug() << "unknow case";
//        break;
//    }
//}
