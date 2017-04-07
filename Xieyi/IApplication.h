#ifndef IAPPLICATION
#define IAPPLICATION
#define NORMAL_TYPE ".txt"
//#define HTML_TYPE   "html"
//#define BINARY_TYPE "binary" // 不以此为文件后缀，表示数据类型并控制操作方式
//#define JS_TYPE     "js"
//#define CSS_TYPE    "css"

#include <string>
using std::string;

typedef unsigned char u_char;
typedef unsigned int u_int;

class IApplication
{
public:
    virtual const u_char *applicationContent() const = 0; //get the Content of application layer
    virtual int applicationLen() const = 0; // get the length of application layer

    virtual void setApplicationContent(const u_char *content,int length) = 0; // set the Content of application layer

    virtual const u_char *dataContent() const = 0; // get the Content of data
    virtual int dataLen() const = 0; // get the length of data
//    virtual void setDataContent(const u_char *Content) = 0; // set the Content of data
    virtual const string &dataType() const = 0;
    virtual const string &compressionType() const = 0;
    virtual const string &encodeType() const = 0;

    virtual bool isBinary() const = 0;
};

#endif // IAPPLICATION

