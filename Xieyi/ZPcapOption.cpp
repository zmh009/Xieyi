#include "ZPcapOption.h"

ZPcapOption::ZPcapOption()
{
    mNetcard = nullptr;
    mMaxSize = OPTION_MAX_SIZE;
    mProsimc = OPTION_PROSIMC;
    mWaitTime = OPTION_WAIT_TIME;
}

ZPcapOption::ZPcapOption(const ZPcapOption &option)
{
    mNetcard = option.mNetcard;
    mMaxSize = option.mMaxSize;
    mProsimc = option.mProsimc;
    mWaitTime = option.mWaitTime;
}

ZPcapOption &ZPcapOption::operator =(const ZPcapOption &option)
{
    if (this == &option) {
        return *this;
    }

    mNetcard = option.mNetcard;
    mMaxSize = option.mMaxSize;
    mProsimc = option.mProsimc;
    mWaitTime = option.mWaitTime;
    return *this;
}

ZPcapOption::~ZPcapOption()
{
    ;
}

void ZPcapOption::setNetcard(const char *netcard)
{
    mNetcard = netcard;
}

const char *ZPcapOption::getNetcard() const
{
    return mNetcard;
}

void ZPcapOption::setMaxSize(unsigned int maxSize)
{
    mMaxSize = maxSize;
}

unsigned int ZPcapOption::getMaxSize() const
{
    return mMaxSize;
}

void ZPcapOption::setProsimc(int prosimc)
{
    mProsimc = prosimc;
}

int ZPcapOption::getProsimc() const
{
    return mProsimc;
}

void ZPcapOption::setWaitTime(unsigned int time)
{
    mWaitTime = time;
}

unsigned int ZPcapOption::getWaitTime() const
{
    return mWaitTime;
}
