#include "ZTcp.h"

ZTcp::ZTcp()
{
    mTcpContent = nullptr;
    mTcpLen = 0;
    mApplicationLen = 0;
}

ZTcp::~ZTcp()
{
    mTcpContent = nullptr;
}

const u_char *ZTcp::transportContent() const
{
    if (mTcpContent != nullptr)
    {
        return mTcpContent->mSrcPort;
    }
    else
    {
        return nullptr;
    }
}

int ZTcp::transportLen() const
{
    return mTcpLen;
}

void ZTcp::setTransportContent(const u_char *content, int length)
{
    if (content == nullptr)
    {
        mTcpContent = nullptr;
        mTcpLen = 0;
        mApplicationLen = 0;
        return ;
    }
    mTcpContent = (TcpForm *)content;
    mTcpLen = length;
    // 防止最高位为一时产生的错误
    int headLen = (mTcpContent->mHeadLenAndSign[0] & 0xF0) >> 4;
    mApplicationLen = mTcpLen - headLen * 4;
}

const u_char *ZTcp::applicationContent() const
{
    if (mTcpContent != nullptr)
    {
        return mTcpContent->mData;
    }
    else
    {
        return nullptr;
    }
}

int ZTcp::applicationLen() const
{
    return mApplicationLen;
}

int ZTcp::srcPort() const
{
    if (mTcpContent == nullptr)
    {
        return -1;
    }
//    long num = toLong(mTcpContent->mSrcPort,2);
    int num = bytesToNum(mTcpContent->mSrcPort,2); // between max and min of unsigned short
    return num;
}

int ZTcp::dstPort() const
{
    if (mTcpContent == nullptr)
    {
        return -1;
    }
    int num = bytesToNum(mTcpContent->mDesPort,2); // between max and min of unsigned short
    return num;
}

long ZTcp::ackId()
{
    if (mTcpContent == nullptr)
    {
        return -1;
    }
    long num = bytesToNum(mTcpContent->mAcknowledge,4); //between max and min of unsigned int
    return num;
}

long ZTcp::seqId()
{
    if (mTcpContent == nullptr)
    {
        return -1;
    }
    long num = bytesToNum(mTcpContent->mSequence,4); //between max and min of unsigned int
    return num;
}

u_char ZTcp::sign()
{
    if (mTcpContent == nullptr)
    {
        return 0;
    }
    return mTcpContent->mHeadLenAndSign[1];
}

long ZTcp::bytesToNum(const u_char *bytes,int count) const
{
    if (bytes == nullptr || count <= 0)
    {
        return -1;
    }
    long num = 0;
    // 将多个unsigned char类型字节转为int数字
    for (int i = 0; i < count; ++i)
    {
        num *= 1 << 8;
        num += bytes[i];
    }
    return num;
}

//const TcpForm *ZTcp::tcpContent() const
//{
//    return mTcpContent;
//}

