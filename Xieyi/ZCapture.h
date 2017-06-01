#ifndef ZCAPTURE_H
#define ZCAPTURE_H
#define OPTION_MAX_SIZE     65535
#define OPTION_PROSIMC      0
#define OPTION_WAIT_TIME    1000
#define EFILTER     "过滤器表达式错误"
#define ESET_FILTER "设置过滤器表达式错误"
#define ELINKLAYOUT "非以太网络"

#include <string>
#include <cstring>
#include <pcap.h>
#include "ZBuffer.h"
using std::string;

typedef pcap_t capHandlerT;

class ZCapture
{
public:
    ZCapture();
    ~ZCapture();

    int start();
    int stop();

    void setNetcard(const string netcard);
    void setMaxSize(unsigned int maxSize);
    void setProsimc(int prosimc);
    void setWaitTime(unsigned int waitTime);
    void setFilter(const string filter);

    const char* getError() const;

private:
    const u_char* getData(capHandlerT *handler, pcap_pkthdr *infor);
    capHandlerT *getHandler();
    bool isStopStatus() const;
    void close();

private:
    ZBuffer* mBuffer;
    capHandlerT *mHandler;

    string mFilter;
    string mNetcard;
    unsigned int mMaxSize;
    unsigned int mWaitTime;
    int mProsimc;

    bool mRun;
    char mError[PCAP_ERRBUF_SIZE] = {'\0'};
};

#endif // ZCAPTURE_H
