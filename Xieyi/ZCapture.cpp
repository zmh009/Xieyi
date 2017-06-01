#include "ZCapture.h"

ZCapture::ZCapture()
{
    mBuffer = ZBuffer::getBuffer(2);

    mHandler = nullptr;
    mMaxSize = OPTION_MAX_SIZE;
    mProsimc = OPTION_PROSIMC;
    mWaitTime = OPTION_WAIT_TIME;
}

ZCapture::~ZCapture()
{
    close();
}

// 子线程运行
int ZCapture::start()
{
    capHandlerT *handler = getHandler();
    if (handler == nullptr)
    {
        return -1;
    }

    pcap_pkthdr infor;
    mRun = true;
    while (mRun)
    {
        const u_char *data = getData(handler, &infor);
        size_t length = infor.len;
        if (data == nullptr)
        {
            continue;
        }

        // 使用string拷贝存储，因此使用const char*类型
        string networkData((const char*)(data), length);
        if (mBuffer->addData(networkData) == -1)
        {
            return -1;
        }
    }
    close();
    return 0;
}

int ZCapture::stop()
{
    mRun = false;
    return 0;
}

void ZCapture::setNetcard(const std::__cxx11::string netcard)
{
    mNetcard = netcard;
}

void ZCapture::setMaxSize(unsigned int maxSize)
{
    mMaxSize = maxSize;
}

void ZCapture::setProsimc(int prosimc)
{
    mProsimc = prosimc;
}

void ZCapture::setWaitTime(unsigned int waitTime)
{
    mWaitTime = waitTime;
}

void ZCapture::setFilter(const std::__cxx11::string filter)
{
    mFilter = filter;
}

const u_char *ZCapture::getData(capHandlerT *handler, pcap_pkthdr *infor)
{
    const u_char *data = pcap_next(handler, infor);
    return data;
}

capHandlerT *ZCapture::getHandler()
{
    // 获得初始的捕获句柄
    if (mNetcard.empty())
    {
        // 默认获取第一个网卡名
        mNetcard = pcap_lookupdev(mError);
        if (mNetcard.empty())
        {
            // 如果获取网卡名出错，返回NULL
            return nullptr;
        }
    }
    mHandler = pcap_open_live(mNetcard.c_str(), mMaxSize, mProsimc, mWaitTime, mError);
    if (mHandler == nullptr)
    {
        return nullptr;
    }

    // 设置过滤器,需要先获得当前的子网掩码
    struct bpf_program filterSt;
    bpf_u_int32 net;
    bpf_u_int32 netmask;
    if (pcap_lookupnet(mNetcard.c_str(), &net, &netmask, mError) != 0) // 获得当前的子网掩码出错
    {
        goto errorOpr;
    }
    if (pcap_compile(mHandler, &filterSt, mFilter.c_str(), 1, netmask) != 0) // 获得当前的过滤器, 1为优化控制过滤器程序
    {
        const char *pcapError = pcap_geterr(mHandler);
        strncpy(mError, pcapError, PCAP_ERRBUF_SIZE-1);
        mError[PCAP_ERRBUF_SIZE-1] = '\0';
        goto errorOpr;
    }
    if (pcap_setfilter(mHandler, &filterSt) != 0) // 设置过滤器出错
    {
        const char *pcapError = pcap_geterr(mHandler);
        strncpy(mError, pcapError, PCAP_ERRBUF_SIZE-1);
        mError[PCAP_ERRBUF_SIZE-1] = '\0';
        goto errorOpr;
    }
    if (pcap_datalink(mHandler) != 1)  // 非以太网
    {
        strncpy(mError, ELINKLAYOUT, PCAP_ERRBUF_SIZE-1);
        mError[PCAP_ERRBUF_SIZE-1] = '\0';
        goto errorOpr;
    }
    return mHandler;

// 出错时资源处理
errorOpr:
    close();
    return nullptr;
}

const char *ZCapture::getError() const
{
    return mError;
}

void ZCapture::close()
{
    if (mHandler != nullptr)
    {
        pcap_close(mHandler);
        mHandler = nullptr;
    }
}
