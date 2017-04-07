#ifndef ZNetworkData_H
#define ZNetworkData_H
#define DEFAULT_SIZE 65535

#include <string>
using std::string;
typedef unsigned char u_char;
typedef unsigned int u_int;

class ZNetworkData
{
public:
    ZNetworkData();
    ZNetworkData(size_t size);
    ZNetworkData(const ZNetworkData &other);
    ~ZNetworkData();
    ZNetworkData& operator = (const ZNetworkData &other);

    void setNetworkData(const u_char *networkData, size_t length);
    u_int length() const;

//    const string toStdString() const;
//    const string toInformation() const;
private:
    u_char *mNetworkData;
    size_t mSize;
    size_t mLength;
};

#endif // ZNetworkData_H
