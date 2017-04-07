#ifndef ZIPV4_H
#define ZIPV4_H
//#define IP_LENGTH 4
// IP协议procotol字段代表,数据包中信息的类型
#define ICMP_TYPE 1
#define TCP_TYPE 6
#define UDP_TYPE 17

#include <stdlib.h>
#include <string>
#include <INetwork.h>
//#include <iostream>

//#include <cstdio>
//#include <iomanip>
//using namespace std;
using std::string;

struct Ipv4Form
{
    u_char mVersionAndHeadLen[1]; //the version(4) and the lengther of head(4)
    u_char mTOS[1];     //the service type(8)
    u_char mLength[2]; //the all length(16)
    u_char mId[2];     //the id of this packet(16)
    u_char mSignAndMove[2];    //the sign of IPv4(3) and the length of moving(13)
    u_char mTTL[1];     //the Time To Live(8)
    u_char mProcotol[1]; //the type of Procotol(8)
    u_char mHeadChecksum[2];  // the head checksum(16)
    u_char mSrcIP[4];         // the source IP(32)
    u_char mDstIP[4];         // the destination IP(32)
    u_char mTransport[0];        // the data of transport layer
};

class ZIpv4 : public INetwork
{
public:
    ZIpv4();
     ~ZIpv4();
    void setNetworkContent(const u_char *Content, int length);

    const u_char *networkContent() const;
    int networkLen() const;

    const u_char *transportContent() const;
    int transportLen() const;
    int transportType() const;

    string srcIp() const;  //return source IP
    string dstIp() const;  //return destination IP

//    int getDataType();
//    int getIpLength();
private:
    string getIpFormat(const u_char *networkIp, int length) const;
private:
    Ipv4Form *mIpContent;
    int mIpv4Len;
    int mTransportLen;
};

#endif // ZIPV4_H

#ifndef ZIPV4_H
#define ZIPV4_H
#include <iostream>
#include "INetwork.h"

#include <cstdio>
#include <iomanip>
using namespace std;

struct Ipv4Form
{
    u_char mVersionAndHeadLen[1]; //the version(4) and the lengther of head(4)
    u_char mTOS[1];     //the service type(8)
    u_char mLength[2]; //the all length(16)
    u_char mId[2];     //the id of this packet(16)
    u_char mSignAndMove[2];    //the sign of IPv4(3) and the length of moving(13)
    u_char mTTL[1];     //the Time To Live(8)
    u_char mProcotol[1]; //the type of Procotol(8)
    u_char mHeadChecksum[2];  // the head checksum(16)
    u_char mSrcIP[4];         // the source IP(32)
    u_char mDesIP[4];         // the destination IP(32)
    u_char mTransport[0];        // the data of transport layer
};

class ZIpv4:public INetwork
{
public:
    ZIpv4();
    virtual ~ZIpv4();
    virtual const u_char *networkContent() const;
    virtual u_int networkLen() const;

    virtual void setNetworkContent(const u_char *Content, u_int length);

    virtual const u_char *transportContent() const;
    virtual u_int transportLen() const;

    const u_char *srcIP();  //return source IP
    const u_char *desIP();  //return destination IP
private:
    Ipv4Form *mIpContent;
    u_int mIpv4;
    u_int mTransportLen;
};

#endif // ZIPV4_H

