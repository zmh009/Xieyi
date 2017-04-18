#include "ZHttp.h"

ZHttp::ZHttp()
    : mHttpRequestMethod({"GET", "POST",
                         "HEAD","PUT","DELETE","OPTIONS","TRACE","CONNECT"}),
      mHttpProtocol({"HTTP/1.1","HTTP/1.0","HTTP/0.9"})
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
    // Returning mHttpData if http data is unknow
    return mHttpData;
}

int ZHttp::applicationLen() const
{
    // Returning mHttpLen if http data is unknow
    return mHttpLen;
}

void ZHttp::setApplicationContent(const u_char *content, int length)
{
    // 清除历史数据
    mHttpForm.mType = EMPTY;
    mHttpLen = 0;
    mHttpData = NULL;
    mDataLen = 0;
    mContentLen = 0;
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
            u_int keyPos = index+2;
            setCompression(keyPos);
            setFormat(keyPos);
            setContentLen(keyPos);
        }
        ++index;
    }
    //-------------------http head end----------------//

    index += 4; // jump "\r\n\r\n"
    mHttpForm.mBodyBegin = index <= length ? index : 0; // length is so short(0) or http body is empty(length)
    mHttpForm.mBodyEnd = length;
    mDataLen = mHttpLen > mHttpForm.mBodyBegin ? mHttpLen - mHttpForm.mBodyBegin : 0;
    setType();
    //--test
    std::cout << "type is " << mHttpForm.mType << std::endl;
    if (mDataType == "")
    {
        mDataType == "";
    }
    if (mDataLen <= 5)
    {
        show();
        std::cout << "mDataLen(" <<mDataLen<<") mHttpLen("<<mHttpLen<<")"<<std::endl;
    }

    return ;
}

/* 获得Http协议的传输数据
 * @return      const u_char*,如果没有数据返回空指针，如果是响应包且有POST数据则返回post数据，如果是请求包或未知或无POST数据则返回整个HTTP数据
 */
const u_char *ZHttp::dataContent() const
{
    if (isEmpty())
    {
        return NULL;
    }
    else if (isRespond() && !isEmptyRespond())
    {
        // 响应报文POST数据非空时恢复POST数据
        return mHttpData+mHttpForm.mBodyBegin;
//        if (mHttpForm.mBodyBegin == 0)
//        {
//            std::cout << "offset is 0" << std::endl;
//        }
    }
    else
    {
        // 请求包或未知或无POST数据
        return mHttpData;
    }
}

int ZHttp::dataLen() const
{
    if (isEmpty())
    {
        return 0;
    }
    else if (isRespond() && !isEmptyRespond())
    {
        // 响应报文POST数据非空时恢复POST数据
        return mDataLen;
    }
    else
    {
        return mHttpLen;
    }
}

/* 获得Http协议的传输数据
 * @return      string,如果没有数据返回空，如果是请求包或是响应包但无POST数据则返回表示请求或响应的类型，如果是响应包且有POST数据则返回post数据类型，如果是未知则返回空
 */
std::__cxx11::string ZHttp::dataType() const
{
//    if (mHttpData == nullptr || mHttpLen <= 0)
//    {
//        return string();
//    }

    // test
//    if (mDataType == "" && mDataLen !=0 && mHttpData != nullptr)
//    {
//        string type = "";
//    }
    // --test end

//    // 在数据类型为空时提示是请求或应答报文
//    if (mDataType.empty())
//    {
//        if (request())
//        {
//            return string(".HttpRequst");
//        }
//        else if (respond())
//        {
//            return string(".HttpRespond");
//        }
//        else if (unknow())
//        {
//            if (mDataLen != 0)
//            {
//                string str("123");
//            }
//            return string(".HttpBinary");
//        }
//        else
//        {
//            return string(".EMPTY");
//        }
//    }
//    // 在数据类型为空时判断是否为请求报文
//    if (mDataType.empty() && request())
//    {
//        return string(".HttpRequst");
//    }
//    else
//    {
//        return mDataType;
//    }
    if (isEmpty() || isUnknow())
    {
        // 没有数据或未知返回空
        return string();
    }
    else if (isRequest())
    {
        // 请求包
        return string(TYPE_REQUEST);
    }
    else if (isEmptyRespond())
    {
        // 响应包但无POST数据
        return string(TYPE_RESPOND);
    }
    else
    {
        // 响应包且有POST数据
        return mDataType;
    }
}

std::__cxx11::string ZHttp::compressionType() const
{
//    if (mHttpData == nullptr || mHttpLen <= 0)
//    {
//        return string();
//    }
    // --test
    if (mCompression == "")
    {
        string type = "";
    }
    // -- test end
    return mCompression;
}

// Http协议为明文传输
std::__cxx11::string ZHttp::encodeType() const
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
//    u_int index = mHttpForm.mRowBegin;
    const char* http = (const char*)mHttpData;

    // 判断是否为响应报文的依据是判断在HTTP行是否有协议版本号
    for (const string &protocol : mHttpProtocol)
    {
        if (!strncmp(http, protocol.c_str(), protocol.size()))
        {
            mHttpForm.mType = RESPOND;
            return ;
        }
    }

    // 判断是否为请求报文的依据是判断在HTTP行是否有请求方法
    for (const string &method : mHttpRequestMethod)
    {
        if (!strncmp(http, method.c_str(), method.size()))
        {
            mHttpForm.mType = REQUEST;
            return ;
        }
    }

    mHttpForm.mType = UNKNOW;
    return ;
//    //find out ' '
//    while (index < mHttpForm.mRowEnd && mHttpData[index] != ' ')
//    {
//        ++index;
//    }
//    //ignore ' '
//    while (index < mHttpForm.mRowEnd && mHttpData[index] == ' ')
//    {
//        ++index;
//    }

//    //judging type is request or respond:
//    //if the second column like some special form while such as "1xx" or "2xx" and etc,
//    //the type is respond
//    if ('1' <= mHttpData[index] && mHttpData[index] <= '5' &&
//        '0' <= mHttpData[index+1] && mHttpData[index+1] <= '9' &&
//        '0' <= mHttpData[index+2] && mHttpData[index+2] <= '9' &&
//        mHttpData[index+3] == ' ')
//    {
//        mHttpForm.mType = RESPOND;
//    }
//    else
//    {
//        mHttpForm.mType = REQUEST;
//    }

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

bool ZHttp::isRequest() const
{
//    setType();
    return mHttpForm.mType == REQUEST;
}

bool ZHttp::isRespond() const
{
//    setType();
    return mHttpForm.mType == RESPOND;
}

bool ZHttp::isUnknow() const
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
//        if (!strncmp((const char*)mHttpData+*iter,HTTP_COMPRESSION_KEY,strlen(HTTP_COMPRESSION_KEY)))
//        {
//            ++count;
//            setEncode(*iter);
//        }
//        else if (!strncmp((const char*)mHttpData+*iter,HTTP_TYPE_KEY,strlen(HTTP_TYPE_KEY)))
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
    if (mHttpData == NULL || index >= mHttpLen)
    {
        mCompression = "";
        return ;
    }
    else if (strncmp((const char*)mHttpData+index,HTTP_COMPRESSION_KEY,strlen(HTTP_COMPRESSION_KEY)))
    {
        return ;
    }

    mCompression = "";
    index += strlen(HTTP_COMPRESSION_KEY);
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
        mCompression = COMPRESSION_GZIP;
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
//        mCompression = '.'+mCompression;
    }
}


void ZHttp::setFormat(u_int index)
{
    if (mHttpData == NULL || index >= mHttpLen )
    {
        mDataType = "";
        mBin = true;
        return ;
    }
    else if (strncmp((const char*)mHttpData+index,HTTP_TYPE_KEY,strlen(HTTP_TYPE_KEY)))
    {
        return ;
    }

    mDataType = "";
    index += strlen(HTTP_TYPE_KEY);
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
        mDataType = TYPE_HTML;
        mBin = false;
    }
    else if (mDataType == "text/xml")
    {
        mDataType = TYPE_XML;
        mBin = false;
    }
    else if (mDataType == "text/css")
    {
        mDataType = TYPE_CSS;
        mBin = false;
    }
    else if (mDataType == "text/javascript")
    {
        mDataType = TYPE_JS;
        mBin = false;
    }
    else if (mDataType == "image/jpeg")
    {
        mDataType = TYPE_JPG;
        mBin = true;
    }
    else if (mDataType == "image/png")
    {
        mDataType = TYPE_PNG;
        mBin = true;
    }
    else if (mDataType == "image/gif")
    {
        mDataType = TYPE_GIF;
        mBin = true;
    }
    else if (mDataType == "application/octet-stream")
    {
        mDataType = TYPE_BINARY;
        mBin = true;
    }
    else if (mDataType == "application/json")
    {
        mDataType = TYPE_JSON;
        mBin = false;
    }
    else if (mDataType == "application/x-javascript")
    {
        mDataType = TYPE_JS;
        mBin = false;
    }
    else if (mDataType == "application/javascript")
    {
        mDataType = TYPE_JS;
        mBin = false;
    }
    else if (mDataType == "application/x-shockwave-flash")
    {
        mDataType = TYPE_SWF;
        mBin = true;
    }
    else if (mDataType == "x-json")
    {
        mDataType = TYPE_JSON;
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

void ZHttp::setContentLen(u_int index)
{
    if (mHttpData == NULL || index >= mHttpLen )
    {
        mContentLen = 0;
        return ;
    }
    else if (strncmp((const char*)mHttpData+index,HTTP_LENGTH_KEY,strlen(HTTP_LENGTH_KEY)))
    {
        return ;
    }

    // 从HTTP的KEY之后读取数据
    index += strlen(HTTP_LENGTH_KEY);
    // 忽略空格，以读取所需数据
    while (mHttpData[index] == ' ')
    {
        ++index;
    }

    // 遍历数字部分以计算当前HTTP传输POST数据的总长度
    while ('0' <= mHttpData[index] && mHttpData[index] <= '9')
    {
        mContentLen *= 10;
        mContentLen += mHttpData[index] - '0';
        ++index;
    }
}

bool ZHttp::isEmpty() const
{
    return  mHttpData == nullptr ||
            mHttpForm.mType == EMPTY;
}

bool ZHttp::isEmptyRespond() const
{
    return  mHttpData != nullptr &&
            mHttpForm.mType == RESPOND &&
            mContentLen == 0 &&
            mDataLen == 0;
}

