#include "ZNetworkData.h"

ZNetworkData::ZNetworkData() : ZNetworkData(DEFAULT_SIZE)
{
    ;
}

ZNetworkData::ZNetworkData(size_t size)
{
    mNetworkData = new u_char[size];
    mSize = size;
    mLength = 0;
}

ZNetworkData::ZNetworkData(const ZNetworkData &other) : ZNetworkData(other.mSize)
{
}

ZNetworkData::~ZNetworkData()
{
    if (mNetworkData != nullptr)
    {
        delete mNetworkData;
    }
}

ZNetworkData &ZNetworkData::operator =(const ZNetworkData &other)
{
    if (&other != this)
    {
        if (mNetworkData != nullptr)
        {
            delete mNetworkData;
        }
        mNetworkData = new u_char[other.mSize];
        mLength = other.mLength;
        mSize = other.mSize;
    }

    return *this;
}

// 按字节拷贝
void ZNetworkData::setNetworkData(const u_char *networkData, size_t length)
{
    if (networkData == nullptr)
    {
        mLength = 0;
        return ;
    }

    mLength = length <= mSize ? length : mSize;
    for (size_t i = 0; i < mLength; ++i)
    {
        mNetworkData[i] = networkData[i];
    }
}

u_int ZNetworkData::length() const
{
    return mLength;
}

//const std::__cxx11::string ZNetworkData::toStdString() const
//{
//    return string(mNetworkData, mLength);
//}

//
//const std::__cxx11::string ZNetworkData::toInformation() const
//{
//    string information;
//    for (size_t i = 0; i < mLength; ++i)
//    {
//        information.push_back(mNetworkData[i]);
//    }
//    return information;
//}
