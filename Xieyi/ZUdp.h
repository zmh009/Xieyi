#ifndef ZUDP_H
#define ZUDP_H

#include <iostream>
#include "ITransport.h"

struct UdpForm
{
    u_char mSrcPort[2];
    u_char mDesPort[2];
    u_char mDataLen[2];
    u_char mCheckSum[2];
    u_char mData[0];
};

class ZUdp : public ITransport
{
public:
    ZUdp();
    ~ZUdp();

    const u_char *transportContent() const; //get the Content of transport layer
    int transportLen() const;
    void setTransportContent(const u_char *Content,int length); // set the Content of transport layer

    const u_char *applicationContent() const; // get the Content of application layer
    int applicationLen() const;

    int srcPort() const;
    int dstPort() const;
private:
    int getPortFormat(const u_char *bytes) const;

private:
    UdpForm *mUdpContent;
    int mUdpLen;
    int mApplicationLen;
};

#endif // ZUDP_H
