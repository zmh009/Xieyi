#include "ZHttps.h"

ZHttps::ZHttps()
{
    mHttpsContent = NULL;
    mHttpsLen = 0;
    mDataLen = 0;
    mIsBinary = true;
    mDataType = "";
    mCompression = "";
    mEncode = ".SSL";
}

ZHttps::~ZHttps()
{
    ;
}

const u_char *ZHttps::applicationContent() const
{
    if (mHttpsContent == NULL || mHttpsLen <= 5)
    {
        return NULL;
    }

    return (const u_char *)mHttpsContent;
}

int ZHttps::applicationLen() const
{
    return mHttpsLen;
}

void ZHttps::setApplicationContent(const u_char *content, int length)
{
    // 初始化数据
    mHttpsContent = NULL;
    mHttpsLen = 0;
    mDataLen = 0;
    mIsBinary = true;
    mDataType = "";
    mCompression = "";
    mEncode = ".SSL";

    // 如果数据有错误，则清空原有数据，否则设置新数据
    if (content == NULL || length <= 5) //the length of ssl head is more then 5
    {
        return ;
    }

    mHttpsContent = (HttpsRecord *)content;
    mHttpsLen = length;
    mDataLen = (mHttpsContent->mLength[0] << 8) + mHttpsContent->mLength[1];
    return ;
}

const u_char *ZHttps::dataContent() const
{
    if (mHttpsContent == NULL || mDataLen <= 0)
    {
        return NULL;
    }

    return mHttpsContent->mData;
}

int ZHttps::dataLen() const
{
    return mDataLen;
}

const std::__cxx11::string &ZHttps::dataType() const
{
//    if (mHttpsContent == nullptr || mDataLen <= 0)
//    {
//        return mDataType;
//    }

    return mDataType;
}

const std::__cxx11::string &ZHttps::compressionType() const
{
//    if (mHttpsContent == nullptr || mHttpsLen <= 0)
//    {
//        return mCompression;
//    }

    return mCompression;
}

const std::__cxx11::string &ZHttps::encodeType() const
{
//    if (mHttpsContent == nullptr || mHttpsLen <= 0)
//    {
//        return mEncode;
//    }

    return mEncode;
}

bool ZHttps::isBinary() const
{
    return mIsBinary;
}

bool ZHttps::isAppData()
{
    if (mHttpsContent == NULL || mDataLen <= 0)
    {
        return false;
    }

    return (mHttpsContent->mContentType[0])*1 == 23;
}

//int ZHttps::reserveData(const std::string &dir, const std::string &id)
//{
//    if (mHttpsContent == NULL || mDataLen <= 0 || id.empty())
//    {
//        return -1;
//    }

//    std::string path = dir+id+".binary";
//    std::cout << "https:"<<path.c_str()<<std::endl;
//    return reserveFile(path.c_str(),(const char*)mHttpsContent->mData,mDataLen,true);
//}

