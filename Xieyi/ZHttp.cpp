#include "ZHttp.h"

ZHttp::ZHttp()
{
    mHttpForm.mType = UNKNOW;
    mHttpData = NULL;
    mHttpLen = 0;
    mDataLen = 0;
    mBin = true;
}

ZHttp::~ZHttp()
{
    ;
}

const u_char *ZHttp::applicationContent() const
{
    //Returning mHttpData if http data is unknow
    return mHttpData;
}

int ZHttp::applicationLen() const
{
    // Returning mHttpLen is http data is unknow
    return mHttpLen;
}

void ZHttp::setApplicationContent(const u_char *content, int length)
{
    // 初始化数据
    mHttpForm.mType = UNKNOW;
    mHttpLen = 0;
    mHttpData = NULL;
    mDataLen = 0;
    mBin = true;
    mCompression = "";
    mDataType = "";
    mEncode = "";

    // 如果数据有错误，则清空原有数据，否则设置新数据
    if (content == NULL || length <= 0)
    {
        return ;
    }

    mHttpData = content;
    mHttpLen = length;
    int index = 0;
    bool empty = true;

    //--------------------http row begin----------------//
    mHttpForm.mRowBegin = 0;
    while (index+1 < length )
    {
        if (mHttpData[index] == '\r' && mHttpData[index+1] == '\n')
        {
            mHttpForm.mRowEnd = index;
            break;
        }
        else if (mHttpData[index] != '\0') // all is '\0'
        {
            empty = false;
        }
        ++index;
    }

    // the http data is unknow
    if (empty)
    {
        mHttpForm.mType = EMPTY;
        return ;
    }
    else if (index+1 >= length)
    {
        mHttpForm.mType = UNKNOW;
        //---//
        if (1 || mDataLen == 0 || mDataType.empty() || mCompression.empty())
        {
//            show();
        }
        return ;
    }
    //--------------------http row end------------------//

    //-------------------http head begin----------------//
    index += 2; // jump the '\r' & '\n'
    mHttpForm.mHeadBegin = index < length ? index : 0; //length is so short
    while (index+3 < length)
    {
        if (mHttpData[index] == '\r' && mHttpData[index+1] == '\n' &&
            mHttpData[index+2] == '\r' && mHttpData[index+3] == '\n')
        {
            mHttpForm.mHeadEnd = index;
            break;
        }
        else if (mHttpData[index] == '\r' && mHttpData[index+1] == '\n')
        {
            setCompression(index+2);
            setFormat(index+2);
        }
        ++index;
    }
    //-------------------http head end----------------//

    index += 4; // jump "\r\n\r\n"
    mHttpForm.mBodyBegin = index <= length ? index : 0; // length is so short(0) or http body is empty(length)
    mHttpForm.mBodyEnd = length;
    mDataLen = mHttpLen > mHttpForm.mBodyBegin ? mHttpLen - mHttpForm.mBodyBegin : 0;
    setType();
    return ;
}

/* 获得Http协议的传输数据
 * @return      const u_char*,如果没有数据返回空指针，如果是响应包则返回post数据，如果是请求包或未知则返回整个HTTP数据
 */
const u_char *ZHttp::dataContent() const
{
    if (mHttpData == NULL || mHttpForm.mType == EMPTY)
    {
        return NULL;
    }
    else if (mHttpForm.mType == RESPOND)
    {
        return mHttpData+mHttpForm.mBodyBegin;
        if (mHttpForm.mBodyBegin == 0)
        {
            std::cout << "offset is 0" << std::endl;
        }
    }
    else
    {
        return mHttpData;
    }
}

int ZHttp::dataLen() const
{
    if (mHttpData == NULL || mHttpForm.mType == EMPTY)
    {
        return 0;
    }
    else if (mHttpForm.mType == RESPOND)
    {
        return mDataLen;
    }
    else
    {
        return mHttpLen;
    }
}

const std::__cxx11::string &ZHttp::dataType() const
{
//    if (mHttpData == nullptr || mHttpLen <= 0)
//    {
//        return string();
//    }

    return mDataType;
}

const std::__cxx11::string &ZHttp::compressionType() const
{
//    if (mHttpData == nullptr || mHttpLen <= 0)
//    {
//        return string();
//    }
    return mCompression;
}

// Http协议为明文传输
const std::__cxx11::string &ZHttp::encodeType() const
{
//    if (mHttpData == nullptr || mHttpLen <= 0)
//    {
//        return string();
//    }

    return mEncode;
}

/* 判断是否是二进制数据
 */
bool ZHttp::isBinary() const
{
    if (!mCompression.empty())
    {
        // 如果是压缩文件，则是二进制数据
        return true;
    }
    else
    {
        return mBin;
    }
}

void ZHttp::setType()
{
    if (mHttpData == NULL)
    {
        mHttpForm.mType = UNKNOW;
        return;
    }

//    u_int index = mHttpForm.mHeadBegin;
    u_int index = mHttpForm.mRowBegin;

    //find out ' '
    while (index < mHttpForm.mRowEnd && mHttpData[index] != ' ')
    {
        ++index;
    }
    //ignore ' '
    while (index < mHttpForm.mRowEnd && mHttpData[index] == ' ')
    {
        ++index;
    }

    //judging type is request or respond:
    //if the second column like some special form while such as "1xx" or "2xx" and etc,
    //the type is respond
    if ('1' <= mHttpData[index] && mHttpData[index] <= '5' &&
        '0' <= mHttpData[index+1] && mHttpData[index+1] <= '9' &&
        '0' <= mHttpData[index+2] && mHttpData[index+2] <= '9' &&
        mHttpData[index+3] == ' ')
    {
        mHttpForm.mType = RESPOND;
    }
    else
    {
        mHttpForm.mType = REQUEST;
    }
//    if (mHttpData[index] < '1' && mHttpData[index] > '5')
//    {
//        mHttpForm.mType = REQUEST;
//    }
//    else if (mHttpData[index+1] < '0' && mHttpData[index+1] > '9')
//    {
//        mHttpForm.mType = REQUEST;
//    }
//    else if (mHttpData[index+2] < '0' && mHttpData[index+2] > '9')
//    {
//        mHttpForm.mType = REQUEST;
//    }
//    else if (mHttpData[index+3] != ' ')
//    {
//        mHttpForm.mType = REQUEST;
//    }
//    else
//    {
//        mHttpForm.mType = RESPOND;
//    }
//    return ;
}

bool ZHttp::request()
{
//    setType();
    return mHttpForm.mType == REQUEST;
}

bool ZHttp::respond()
{
//    setType();
    return mHttpForm.mType == RESPOND;
}

bool ZHttp::unknow()
{
    return mHttpForm.mType == UNKNOW;
}

/* 判断是否是二进制数据
 */
//bool ZHttp::binary()
//{

//    if (!mCompression.empty())
//    {
//        // 如果是压缩文件，则是二进制数据
//        return true;
//    }
//    else
//    {
//        return mBin;
//    }
////    return true;
//}

//const std::string &ZHttp::encode()
//{
//    if (mHttpData == NULL)
//    {
//        mCompression = "";
//    }
//    else if (mHttpForm.mType == UNKNOW)
//    {
//        mCompression = "";
//    }
//    return mCompression;
//}

//const std::string &ZHttp::format()
//{
//    if (mHttpData == NULL)
//    {
//        mDataType = "";
//    }
//    else if (mHttpForm.mType == UNKNOW)
//    {
//        mDataType = DUNKNOW;
//    }
//    return mDataType;
//}

void ZHttp::show()
{
    std::cout <<"show bytes begin :"<<std::endl;
    for (u_int i = 0 ; i < mHttpLen; ++i)
    {
        std::cout << mHttpData[i];
    }
    std::cout << std::endl;
//    for (u_int i = 0; i < mHttpForm.mHeadEnd; ++i)
//    {
////        printf("%c",mHttpData[i]);
//        std::cout << mHttpData[i];
//    }
//    std::cout << std::endl;

//    for (u_int i = mHttpForm.mBodyBegin; i < mHttpLen; ++i)
//    {
//        std::cout << std::hex << std::setw(2)<<std::setfill('0')<<mHttpData[i];
//    }
//    std::cout << std::dec << std::setw(0)<<std::setfill(' ')<<std::endl;
}

//void ZHttp::fileType(std::string &suffix)
//{
//    if (mHttpData == NULL || mHttpForm.mType == UNKNOW)
//    {
//        suffix = "";
//        mBin = true;
//        return ;
//    }

//    int count = 0;
//    for (std::list<u_int>::iterator iter = mHttpForm.mHandLine.begin(); iter != mHttpForm.mHandLine.end() && count < 2; ++iter)
//    {
//        if (!strncmp((const char*)mHttpData+*iter,DEncoding,strlen(DEncoding)))
//        {
//            ++count;
//            setEncode(*iter);
//        }
//        else if (!strncmp((const char*)mHttpData+*iter,DType,strlen(DType)))
//        {
//            ++count;
//            setFormat(*iter);
//        }
//    }

//    suffix = mDataType+mEncode;
//    return ;
//}

//int ZHttp::reserveData(const std::string &dir, const std::string &id)
//{
//    if (mHttpData == NULL || dir.empty() || id.empty())
//    {
//        return -1;
//    }

//    std::string path = mDataType+mEncode;
////    fileType(path);

//    path = dir+id+path;
//    std::cout << "path is:"<<path.c_str() << std::endl;
//    reserveFile(path.c_str(),(const char*)mHttpData+mHttpForm.mBodyBegin,mDataLen,mBin);
//    return 0;
//}

void ZHttp::setCompression(u_int index)
{
    if (mHttpData == NULL || mHttpForm.mType == EMPTY || index >= mHttpLen)
    {
        mCompression = "";
        return ;
    }
    else if (strncmp((const char*)mHttpData+index,DEncoding,strlen(DEncoding)))
    {
        return ;
    }

    mCompression = "";
    index += strlen(DEncoding);
    while (mHttpData[index] == ' ') //ignore space
    {
        ++index;
    }
    while (mHttpData[index] != '\r' && mHttpData[index] != ';' && mHttpData[index] != ' ') // get encode of data
    {
        mCompression.push_back(mHttpData[index]);
        ++index;
    }

    // set encode
    std::cout << "mCompression is :"<<mCompression<<std::endl;
    if (mCompression == "gzip")
    {
        mCompression = ".gz";
    }
    else
    {
        // change '/' '\' ':' '?' '"' '|' to '&'
        for (std::string::iterator iter = mCompression.begin(); iter != mCompression.end(); ++iter)
        {
            if (*iter == '/' || *iter == '\\' || *iter == ':' ||\
                    *iter == '?' || *iter == '"' || *iter == '|')
            {
                *iter = '&';
            }
        }
        mCompression = '.'+mCompression;
    }
}


void ZHttp::setFormat(u_int index)
{
    if (mHttpData == NULL || mHttpForm.mType == EMPTY || index >= mHttpLen )
    {
        mDataType = "";
        mBin = true;
        return ;
    }
    else if (strncmp((const char*)mHttpData+index,DType,strlen(DType)))
    {
        return ;
    }

    mDataType = "";
    index += strlen(DType);
    while (mHttpData[index] == ' ') //ignore space
    {
        ++index;
    }
    while (mHttpData[index] != '\r' && mHttpData[index] != ';' && mHttpData[index] != ' ') // get format of data
    {
        mDataType.push_back(mHttpData[index]);
        ++index;
    }

    // set format type
    std::cout << "fromat is :"<<mDataType << std::endl;
    if (mDataType == "text/html")
    {
        mDataType = ".html";
        mBin = false;
    }
    else if (mDataType == "text/xml")
    {
        mDataType = ".xml";
        mBin = false;
    }
    else if (mDataType == "text/css")
    {
        mDataType = ".css";
        mBin = false;
    }
    else if (mDataType == "text/javascript")
    {
        mDataType = ".js";
        mBin = false;
    }
    else if (mDataType == "image/jpeg")
    {
        mDataType = ".jpg";
        mBin = true;
    }
    else if (mDataType == "image/png")
    {
        mDataType = ".png";
        mBin = true;
    }
    else if (mDataType == "image/gif")
    {
        mDataType = ".gif";
        mBin = true;
    }
    else if (mDataType == "application/octet-stream")
    {
        mDataType = ".binary";
        mBin = true;
    }
    else if (mDataType == "application/json")
    {
        mDataType = ".json";
        mBin = false;
    }
    else if (mDataType == "application/x-javascript")
    {
        mDataType = ".js";
        mBin = false;
    }
    else if (mDataType == "application/javascript")
    {
        mDataType = ".js";
        mBin = false;
    }
    else if (mDataType == "application/x-shockwave-flash")
    {
        mDataType = ".swf";
        mBin = true;
    }
    else if (mDataType == "x-json")
    {
        mDataType = ".json";
        mBin = false;
    }
    else
    {
        // change '/' '\' ':' '?' '"' '|' to '&'
        for (std::string::iterator iter = mDataType.begin(); iter != mDataType.end(); ++iter)
        {
            if (*iter == '/' || *iter == '\\' || *iter == ':' ||\
                    *iter == '?' || *iter == '"' || *iter == '|')
            {
                *iter = '&';
            }
        }
        mDataType = '.'+mDataType;
        mBin = true;
    }
}

