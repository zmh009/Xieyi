#include "ZIpv4.h"

ZIpv4::ZIpv4()
{
    mIpContent = nullptr;
    mIpv4Len = 0;
    mTransportLen = 0;
}

ZIpv4::~ZIpv4()
{
    mIpContent = nullptr;
}

const u_char *ZIpv4::networkContent() const
{
    if (mIpContent != nullptr)
    {
        return mIpContent->mVersionAndHeadLen;
    }
    else
    {
        return nullptr;
    }
}

int ZIpv4::networkLen() const
{
    return mIpv4Len;
}

void ZIpv4::setNetworkContent(const u_char *content, int length)
{
    if (content == nullptr || length <= 0)
    {
        mIpContent = nullptr;
        mIpv4Len = 0;
        mTransportLen = 0;
        return ;
    }
    mIpContent = (Ipv4Form*)content;
    mIpv4Len = length;
    u_char headLen = (mIpContent->mVersionAndHeadLen[0] & 0x0F);
    mTransportLen = mIpv4Len - (int)headLen * 4;
}

const u_char *ZIpv4::transportContent() const
{
    if (mIpContent != nullptr || mTransportLen <= 0)
    {
        return mIpContent->mTransport;
    }
    else
    {
        return nullptr;
    }
}

int ZIpv4::transportLen() const
{
    return mTransportLen;
}

int ZIpv4::transportType() const
{
    return (int)mIpContent->mProcotol[0];
}

std::__cxx11::string ZIpv4::srcIp() const
{
    if (mIpContent == nullptr || mIpv4Len <= 0)
    {
        return string();
    }

    return getIpFormat(mIpContent->mSrcIP, sizeof(mIpContent->mSrcIP));
}

std::__cxx11::string ZIpv4::dstIp() const
{
    if (mIpContent == nullptr || mIpv4Len <= 0)
    {
        return nullptr;
    }

    return getIpFormat(mIpContent->mDstIP, sizeof(mIpContent->mDstIP));
}

std::__cxx11::string ZIpv4::getIpFormat(const u_char *networkIp, int length) const
{
    if (networkIp == nullptr || length <= 0)
    {
        return string();
    }

    string ip = "";
    for (int i = 0; i < length; ++i)
    {
        // ip的二进制使用unsigned char到int类型转换,转为数字
        int ip_part = networkIp[i];
        // 将数字转为string格式
        ip += std::to_string(ip_part);
        if (i != length-1) // ip最后一个数字后无'.'
        {
            ip = ip + '.';
        }
    }

    return ip;
}

