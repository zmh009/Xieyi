#ifndef ZHTTP_H
#define ZHTTP_H

#define HTTP_COMPRESSION_KEY "Content-Encoding:"
#define HTTP_TYPE_KEY "Content-Type:"
#define HTTP_LENGTH_KEY "Content-Length:"
#define COMPRESSION_GZIP "gz"
#define TYPE_HTML   "html"
#define TYPE_XML    "xml"
#define TYPE_CSS    "css"
#define TYPE_JS     "js"
#define TYPE_JPG    "jpg"
#define TYPE_PNG    "png"
#define TYPE_GIF    "gif"
#define TYPE_JSON   "json"
#define TYPE_SWF    "swf"
#define TYPE_BINARY "binary"
#define TYPE_REQUEST    "HttpRequest"
#define TYPE_RESPOND    "HttpRespond"

//#define DUNKNOW "unknow"

extern "C"
{
    #include <time.h>
}
#include <iostream>
#include <list>
#include <string>
#include <cstring>
//#include <fstream>
#include "IApplication.h"
using std::string;
using std::list;

#include <cstdio>
#include <iomanip>

enum HttpType{UNKNOW = 0,REQUEST,RESPOND,EMPTY};

struct HttpForm
{
//    void clear()
//    {
//        mRowBegin = 0;
//        mRowEnd = 0;
//        mHeadBegin = 0;
//        mHeadEnd = 0;
//        mBodyBegin = 0;
//        mBodyEnd = 0;
//        mType = UNKNOW;
//    }

    u_int mRowBegin;
    u_int mRowEnd;
    u_int mHeadBegin;
    u_int mHeadEnd;
    u_int mBodyBegin;
    u_int mBodyEnd;
    HttpType mType;
};

class ZHttp : public IApplication
{
public:
    ZHttp();
    virtual ~ZHttp();

    virtual const u_char *applicationContent() const; //get the Content of application layer
    virtual int applicationLen() const; // get the length of application layer

    virtual void setApplicationContent(const u_char *content,int length); // set the Content of application layer

    virtual const u_char *dataContent() const; // get the Content of data
    virtual int dataLen() const; // get the length of data

    virtual string dataType() const;
    virtual string compressionType() const;
    virtual string encodeType() const;

    virtual bool isBinary() const; // judge the data is binary or not

    bool isRequest() const; // judge the packet is request or not
    bool isRespond() const; // judge the packet is respond or not
    bool isUnknow() const;  // judge the data is unknow or not
//    bool binary();  // judge the data is binary or not

//    const std::string &encode();
//    const std::string &format();

    void show();
private:
    void setType(); // set type is request or respond
//    void setEncode(u_int index); // get the encode of http data,and it's a section of fileType
    void setCompression(u_int index); // get the encode of http data,and it's a section of fileType
    void setFormat(u_int index); // the function is get the format of http data,and it's a section of fileType
    void setContentLen(u_int index); // 设置当前HTTP协议传输POST数据的总长度
    bool isEmpty() const; // 判断当前HTTP协议是否为空数据
    bool isEmptyRespond() const; // 判断当前HTTP协议的POST数据是否为空

private:
    HttpForm mHttpForm;
    const u_char *mHttpData;
    u_int mHttpLen;
    u_int mDataLen; // 当前HTTP协议的数据长度
    u_int mContentLen; // 当前HTTP协议要传输的POST数据总长度
    bool mBin;
//    std::string mEncode;
    string mCompression;
//    std::string mFormat;
    string mDataType;
    string mEncode;
    const list<string> mHttpRequestMethod; // 存储HTTP协议请求方法，以判断是否为请求报文
    const list<string> mHttpProtocol; // 存储HTTP协议版本，以判断是否为响应报文
};

#endif // ZHTTP_H
