#include "ZUdp.h"

ZUdp::ZUdp()
{
    mUdpContent = NULL;
}

ZUdp::~ZUdp()
{
    mUdpContent = NULL;
    mUdpLen = 0;
    mApplicationLen = 0;
}

const u_char *ZUdp::transportContent() const
{
    if (mUdpContent != NULL)
    {
        return mUdpContent->mSrcPort;
    }
    else
    {
        return NULL;
    }
}

int ZUdp::transportLen() const
{
    return mUdpLen;
}

void ZUdp::setTransportContent(const u_char *content, int length)
{
    if (content == NULL)
    {
        mUdpContent = nullptr;
        mUdpLen = 0;
        mApplicationLen = 0;
        return ;
    }

    mUdpContent = (UdpForm *)content;
    mUdpLen = length;
    mApplicationLen = mUdpLen-20 ;
}

const u_char *ZUdp::applicationContent() const
{
    if (mUdpContent != NULL)
    {
        return mUdpContent->mData;
    }
    return NULL;
}

int ZUdp::applicationLen() const
{
    return mApplicationLen;
}

int ZUdp::srcPort() const
{
    if (mUdpContent == nullptr)
    {
        return -1;
    }
//    long num = toLong(mTcpContent->mSrcPort,2);
    int num = getPortFormat(mUdpContent->mSrcPort); // between max and min of unsigned short
    return num;
}

int ZUdp::dstPort() const
{
    if (mUdpContent == nullptr)
    {
        return -1;
    }
    int num = getPortFormat(mUdpContent->mDesPort); // between max and min of unsigned short
    return num;
}

int ZUdp::getPortFormat(const u_char *bytes) const
{
    if (bytes == nullptr)
    {
        return -1;
    }

    int num = 0;
    // 将多个unsigned char类型字节转为int数字
    for (int i = 0; i < PORT_LENGTH; ++i)
    {
        num *= 1 << 8;
        num += bytes[i];
    }
    return num;
}

