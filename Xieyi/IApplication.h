#ifndef IAPPLICATION
#define IAPPLICATION
#define TYPE_NORMAL "txt"
#define TYPE_OTHER  "other"

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

    virtual string dataType() const = 0;
    virtual string compressionType() const = 0;
    virtual string encodeType() const = 0;

    virtual bool isBinary() const = 0;
};

#endif // IAPPLICATION

