#include "ZMac.h"

ZMac::ZMac()
{
    mFormat = nullptr;
    mMacLen = 0;
    mNetworkLen = 0;
}

ZMac::~ZMac()
{

}

void ZMac::setLinkContent(const u_char *content, int length)
{
    if (content == nullptr || length < ENTHERNET_LINKLAYOUT_OFFSET)
    {
        mFormat = nullptr;
        mMacLen = 0;
        mNetworkLen = 0;
        return ;
    }

    mFormat = (MacFormat*)content;
    mMacLen = ENTHERNET_LINKLAYOUT_OFFSET;
    mNetworkLen = length - ENTHERNET_LINKLAYOUT_OFFSET;
    return ;
}

int ZMac::linkLen()
{
    return mMacLen;
}

const u_char *ZMac::networkContent()
{
    if (mFormat == nullptr || mNetworkLen <= 0)
    {
        return nullptr;
    }

    return mFormat->mNetworkContent;
}

int ZMac::networkLen()
{
    return mNetworkLen;
}

std::__cxx11::string ZMac::srcMac()
{
    if (mFormat == nullptr || mMacLen <= 0)
    {
        return string();
    }

    const u_char *srcMacData = mFormat->mSrcMac;
    return getMacAddress(srcMacData);
}

std::__cxx11::string ZMac::dstMac()
{
    if (mFormat == nullptr || mMacLen <= 0)
    {
        return string();
    }

    const u_char *dstMacData = mFormat->mDstMac;
    return getMacAddress(dstMacData);
}

int ZMac::netkworkType()
{
    if (mFormat == nullptr || mNetworkLen <= 0)
    {
        return -1;
    }

    // 类型码第13、14字节组成的整型值
    const u_char *type = mFormat->mNetworkType;
    int typeFormat = (type[0] << 8) + type[1];
    return typeFormat;
}

//获得MAC地址的格式化表示
std::__cxx11::string ZMac::getMacAddress(const u_char *mac)
{
    if (mac == nullptr)
    {
        return string();
    }

    string macAddress = "";
    for (size_t i = 0; i < MAC_ADDRESS_LENGTH; ++i)
    {
        // 将一个字节的前4位与后4位转为字符类型，并在每个字节之间添加':'字符，拼接为string类型
        macAddress += (mac[i] & 0xF0) + '0';
        macAddress += (mac[i] & 0x0F) + '0';
        if (i != MAC_ADDRESS_LENGTH)
        {
            macAddress += ':';
        }
    }
    return macAddress;
}
