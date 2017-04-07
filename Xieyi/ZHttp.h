#ifndef ZHTTP_H
#define ZHTTP_H

#define DEncoding "Content-Encoding:"
#define DType "Content-Type:"
#define DUNKNOW ".unknow"

#include "IApplication.h"
#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <fstream>
extern "C"
{
    #include <time.h>
}

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

    virtual const string &dataType() const;
    virtual const string &compressionType() const;
    virtual const string &encodeType() const;

    virtual bool isBinary() const; // judge the data is binary or not

    bool request(); // judge the packet is request or not
    bool respond(); // judge the packet is respond or not
    bool unknow();  // judge the data is unknow or not
//    bool binary();  // judge the data is binary or not

//    const std::string &encode();
//    const std::string &format();

    void show();
protected:
    void setType(); // set type is request or respond
//    void setEncode(u_int index); // get the encode of http data,and it's a section of fileType
    void setCompression(u_int index); // get the encode of http data,and it's a section of fileType
    void setFormat(u_int index); // the function is get the format of http data,and it's a section of fileType
private:
    HttpForm mHttpForm;
    const u_char *mHttpData;
    u_int mHttpLen;
    u_int mDataLen;
    bool mBin;
//    std::string mEncode;
    std::string mCompression;
//    std::string mFormat;
    std::string mDataType;
    std::string mEncode;
};

#endif // ZHTTP_H
