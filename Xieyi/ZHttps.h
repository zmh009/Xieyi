#ifndef ZHTTPS_H
#define ZHTTPS_H
#define ENCODE_SSL "SSL"

#include "IApplication.h"

struct HttpsRecord
{
    u_char mContentType[1]; // https exist data if this is 23
    u_char mVersion[2]; // main version and second version
    u_char mLength[2];
    u_char mData[0];
    //the HMAC length is 0 bit , 16 bits or 20 bits
};

class ZHttps : public IApplication
{
public:
    ZHttps();
    ~ZHttps();
    const u_char *applicationContent() const;
    int  applicationLen() const ;

    void setApplicationContent(const u_char *content,int length);

    const u_char *dataContent() const;
    int dataLen() const;

    string dataType() const;
    string compressionType() const;
    string encodeType() const;

    bool isBinary() const;

    bool isAppData();

private:
    HttpsRecord *mHttpsContent;
    int mHttpsLen;
    int mDataLen;
    bool mIsBinary;
    string mDataType;
    string mCompression;
    string mEncode;
};

#endif // ZHTTPS_H
