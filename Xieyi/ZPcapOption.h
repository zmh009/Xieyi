#ifndef ZPCAPOPTION_H
#define ZPCAPOPTION_H
#define OPTION_MAX_SIZE     65535
#define OPTION_PROSIMC      1
#define OPTION_WAIT_TIME    1000

class ZPcapOption
{
public:
    ZPcapOption();
    ZPcapOption(const ZPcapOption &option);
    ZPcapOption &operator =(const ZPcapOption &option);
    ~ZPcapOption();
    void setNetcard(const char *netcard);
    const char *getNetcard() const;

    void setMaxSize(unsigned int maxSize);
    unsigned int getMaxSize() const;

    void setProsimc(int prosimc);
    int getProsimc() const;

    void setWaitTime(unsigned int time);
    unsigned int getWaitTime() const;

    const char *getError() const;

private:
    const char* mNetcard;
    unsigned int mMaxSize;
    unsigned int mWaitTime;
    int mProsimc;
};

#endif // ZPCAPOPTION_H
