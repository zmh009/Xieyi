#ifndef ZCAPTURE_H
#define ZCAPTURE_H
#define OPTION_MAX_SIZE     65535
#define OPTION_PROSIMC      0
#define OPTION_WAIT_TIME    1000
#define EFILTER     "过滤器表达式错误"
#define ESET_FILTER "设置过滤器表达式错误"
#define ELINKLAYOUT "非以太网络"

#include <string>
#include <pcap.h>
//#include "ZPcapOption.h"
#include "ZBuffer.h"
//#include "ZNetworkData.h"
//#include "ZInformation.h"
using std::string;
//__test
#include <iostream>
#include <iomanip>

typedef pcap_t capHandlerT;

class ZCapture
{
public:
    ZCapture();
//    ZCapture(const ZPcapOption& option);
    ~ZCapture();

    int start();
    int stop();

    void setNetcard(const char *netcard);
    void setMaxSize(unsigned int maxSize);
    void setProsimc(int prosimc);
    void setWaitTime(unsigned int waitTime);
    void setFilter(const char* filter);

    //void setOption(const ZPcapOption &option);
    const char *getError() const;

private:
    const u_char* getData(capHandlerT *handler, pcap_pkthdr *infor);
    //int sendToBuffer(const ZNetWorkData &netWorkData);
    capHandlerT *getHandler();
    bool isStopStatus() const;
    void close();

//    void showData(const u_char *data, u_int length);
//    void setStopStatus(bool status = true);
//    void setRunStatus(bool status = true);

private:
    //ZPcapOption mOption;

//    ZInformation mInformation;
    ZBuffer* mBuffer;
    capHandlerT *mHandler;

    const char* mFilter;
    const char* mNetcard;
    unsigned int mMaxSize;
    unsigned int mWaitTime;
    int mProsimc;

    bool mRun;
    char mError[PCAP_ERRBUF_SIZE] = {'\0'};

};

#endif // ZCAPTURE_H
