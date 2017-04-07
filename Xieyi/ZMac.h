#ifndef ZMAC_H
#define ZMAC_H
#define ENTHERNET_LINKLAYOUT_OFFSET 14
#define MAC_ADDRESS_LENGTH 6
// 网络层协议码
#define IP_TYPE     0x0800
#define ARP_TYPE    0x0806

#include <string>
using std::string;

typedef unsigned char u_char;

struct MacFormat
{
    const u_char mSrcMac[6];
    const u_char mDstMac[6];
    const u_char mNetworkType[2];
    const u_char mNetworkContent[0];
};

class ZMac
{
public:
    ZMac();
    ~ZMac();

    void setLinkContent(const u_char *content, int length);
    int linkLen();

    const u_char *networkContent();
    int networkLen();

    string srcMac();
    string dstMac();
    int netkworkType();

private :
    string getMacAddress(const u_char* mac);

private:
    MacFormat *mFormat;
    int mMacLen;
    int mNetworkLen;
};

#endif // ZMAC_H
