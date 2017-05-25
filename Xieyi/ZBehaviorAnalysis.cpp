#include "ZBehaviorAnalysis.h"

ZBehaviorAnalysis::ZBehaviorAnalysis()
{
    // 初始化存储网络数据的缓冲区
    mBuffer = ZBuffer::getBuffer();
    mReadHandle = mBuffer->openRead();
    mRun = true;
}

ZBehaviorAnalysis::~ZBehaviorAnalysis()
{
    mBuffer->closeRead(mReadHandle);
}

//
int ZBehaviorAnalysis::start()
{
    if ( 0 != readConfiguration(CONFIGURATION_PATH))
    {
        return -1;
    }
    mRun = true;
    while(mRun)
    {
        // 读取网络数据，并判断是否合法，包括非空等
        string networkData = getNetworkData();
        if (networkData.empty())
        {
            continue;
        }

        // 获得数据明细，并记录分析
        ZDataItem dataItem = getItem(networkData);
        if (0 != recordBehavior(dataItem))
        {
            return -1;
        }

        mError = "";
    }
    return 0;
}

ZBehaviorAnalysis::ZDataItem ZBehaviorAnalysis::getItem(const std::__cxx11::string &networkData)
{
    if (networkData.empty())
    {
        return ZDataItem();
    }

    ZDataItem dataItem;
    // 将协议分析的类设为静态类型，可不必多次初始化
    static ZMac sMac;
    static ZIpv4 sIpv4;
    static ZTcp sTcp;
    static ZUdp sUdp;

    dataItem.mLength = networkData.length();
    // -------------链路层分析------------
    const char *data = networkData.data();

    sMac.setLinkContent((const u_char*)data, networkData.length());
    const u_char* networkLayoutData = sMac.networkContent();
    int networkLen = sMac.networkLen();
    int networkType = sMac.netkworkType();
    dataItem.mSrcMac = sMac.srcMac();
    dataItem.mDstMac = sMac.dstMac();
    dataItem.mProcotolType = "MAC";

    // -------------网络层分析------------
    INetwork *networkLayout;
    switch (networkType)
    {
    case IP_TYPE:
        networkLayout = &sIpv4;
        dataItem.mProcotolType = "IPv4";
        break;
    default:
        // 未知网络层协议类型
        return dataItem;
    }
    // 向明细里添加数据
    networkLayout->setNetworkContent(networkLayoutData, networkLen);
    dataItem.mSrcIp = networkLayout->srcIp();
    dataItem.mDstIp = networkLayout->dstIp();

    // -------------运输层分析------------
    ITransport *transportLayout;
    int transportType = networkLayout->transportType();
    switch (transportType)
    {
    case TCP_TYPE:
        dataItem.mProcotolType = "TCP";
        sTcp.setTransportContent(sIpv4.transportContent(), sIpv4.transportLen());
        // 判断TCP状态
        if ( sTcp.sign() & ACK) // 是否为TCP一次会话的结束
        {
            dataItem.mProcotolType.append(" ACK");
        }
        if (sTcp.sign() & PSH)
        {
            dataItem.mProcotolType.append(" PSH");
        }
        if (sTcp.sign() & RET)
        {
            dataItem.mProcotolType.append(" RET");
        }
        if (sTcp.sign() & SYN)
        {
            dataItem.mProcotolType.append(" SYN");
        }
        if (sTcp.sign() & FIN)
        {
            dataItem.mProcotolType.append(" FIN");
        }
        if (sTcp.sign() & URG)
        {
            dataItem.mProcotolType.append(" URG");
        }
        transportLayout = &sTcp;
        break;
    case UDP_TYPE:
        dataItem.mProcotolType = "UDP";
        sUdp.setTransportContent(sIpv4.transportContent(), sIpv4.transportLen());
        transportLayout = &sUdp;
        break;
    default:
        return dataItem; // 非TCP、UDP协议
    }
    // 向明显里添加数据
    dataItem.mSrcPort = transportLayout->srcPort();
    dataItem.mDstPort = transportLayout->dstPort();

    // -------------应用层分析------------
    if (dataItem.mSrcPort == HTTP_PORT)
    {
        dataItem.mProcotolType = "HTTP response";
    }
    else if (dataItem.mDstPort == HTTP_PORT)
    {
        dataItem.mProcotolType = "HTTP request";
    }
    else if (dataItem.mSrcPort == HTTPS_PORT)
    {
        dataItem.mProcotolType = "HTTPS response";
    }
    else if (dataItem.mDstPort == HTTPS_PORT)
    {
        dataItem.mProcotolType = "HTTPS request";
    }

    return dataItem;
}

/*
 * 记录指定秒级时间内的通信次数，有异常则响应处理
 */
int ZBehaviorAnalysis::recordBehavior(const ZDataItem &dataItem)
{
    if (0 != moreVisitResponse(dataItem) || // 访问量过量响应
        0 != showOnGUi(dataItem) // 在用户界面上显示流量
        )
    {
        return -1;
    }
    return 0;
}

int ZBehaviorAnalysis::showOnGUi(const ZBehaviorAnalysis::ZDataItem &dataItem)
{
    static ZResponseWidget *responseWidget = ZResponseWidget::getResponseWidget();
    if (responseWidget == nullptr)
    {
        mError = "response widget get error.";
        return -1;
    }

    const char *week[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    time_t timeStamp;
    struct tm *currentTime;
    std::time(&timeStamp);
    currentTime = std::localtime(&timeStamp); // 取得当地时间
    // 时间格式化为字符串类型
    string timeStr = std::to_string(1900+currentTime->tm_year) + "/" +
                     std::to_string(1+currentTime->tm_mon) + "/" +
                     std::to_string(currentTime->tm_mday) + "/ " +
                     week[currentTime->tm_wday] + " " +
                     std::to_string(currentTime->tm_hour) + ":" +
                     std::to_string(currentTime->tm_min) + ":" +
                     std::to_string(currentTime->tm_sec);

    // 行为特点
    string behavior = dataItem.mSrcIp + "("+dataItem.mSrcMac+"):" + std::to_string(dataItem.mSrcPort) + "-> " +
                      dataItem.mDstIp + "("+dataItem.mDstMac+"):" + std::to_string(dataItem.mDstPort);


    responseWidget->add(timeStr,
                        dataItem.mProcotolType,
                        behavior,
                        dataItem.mLength);
    return 0;
}

int ZBehaviorAnalysis::moreVisitResponse(const ZDataItem &dataItem)
{
    time_t currentTimeStamp = time(nullptr); // 当前时间戳不会小于之前通信的时间戳
    ZCommunicationItem &communicationItem = mRecord[dataItem.mSrcMac];
    if (communicationItem.mTimeStamp == 0 || currentTimeStamp > communicationItem.mTimeStamp)
    {
        // 第一次通信或超过一秒后通信
        communicationItem.mTimeStamp = currentTimeStamp;
        communicationItem.mCount = 1;
    }
    else
    {
        // 在同一秒内通信
        communicationItem.mCount += 1;
    }

    if (communicationItem.mCount > mMaxCommunicationTimes ||
        communicationItem.mCount < mMinCommunicationTimes)
    {
        // 访问量过量响应
        string information =
            dataItem.mSrcIp + "("+dataItem.mSrcMac+") -> " +
            dataItem.mDstIp + "("+dataItem.mDstMac+"): count(" +
            std::to_string(communicationItem.mCount) + "), min count("+
            std::to_string(mMinCommunicationTimes) + "), max count("+
            std::to_string(mMaxCommunicationTimes)+")\n";

        if (0 != mBehaviorResponse.record(information, mLogPath) )
        {
            mError = "more visit response record error.";
            return -1;
        }
    }
    return 0;
}

int ZBehaviorAnalysis::readConfiguration(const std::__cxx11::string &configurationPath)
{
    // 配置文件字段的默认值
    mMaxCommunicationTimes = MAX_COMMUNICATION_TIMES;
    mMinCommunicationTimes = MIN_COMMUNICATION_TIMES;
    mLogPath = LOG_PATH;

    Config cfg;

    try
    {
        cfg.readFile(configurationPath.c_str());
    }
    catch (const libconfig::FileIOException &fioex)
    {
        mError = "IO error. ";
        mError += fioex.what();
        return -1;
    }
    catch (const libconfig::ParseException &pex)
    {
        mError = "parsing error. ";
        mError += pex.what();
        return -1;
    }

    cfg.lookupValue("minCommunicationTimes", mMinCommunicationTimes);
    cfg.lookupValue("maxCommunicationTimes", mMaxCommunicationTimes);
    cfg.lookupValue("logPath", mLogPath);
    return 0;
}
