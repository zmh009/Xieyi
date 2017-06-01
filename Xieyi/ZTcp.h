#ifndef ZTCP_H
#define ZTCP_H

#include "ITransport.h"

#define URG 0x20
#define ACK 0x10
#define PSH 0x08
#define RET 0x04
#define SYN 0x02
#define FIN 0x01

struct TcpForm
{
    u_char mSrcPort[2];//the source port(16)
    u_char mDesPort[2];//the destination port(16)
    u_char mSequence[4];//the sequence number(32)
    u_char mAcknowledge[4];//the acknowledge number(32)
    u_char mHeadLenAndSign[2];//the length of head(4) and reserve(6) and sign(6,U A P R S F)
    u_char mWindowSize[2];//the Window size(16)
    u_char mChecksum[2];//the checksum(16)
    u_char mUrgentPointer[2];//the urgent pointer(16)
    u_char mData[0];//the data or option and data
};

class ZTcp : public ITransport
{
public:
    ZTcp();
    ~ZTcp();

    const u_char *transportContent() const;
    int transportLen() const;

    void setTransportContent(const u_char *Content,int length);

    const u_char *applicationContent() const;
    int applicationLen() const;

    int srcPort() const;
    int dstPort() const;

    long ackId();
    long seqId();
    u_char sign();

protected:
    long bytesToNum(const u_char *bytes, int count) const;

private:
    TcpForm *mTcpContent;
    int mTcpLen;
    int mApplicationLen;
};

#endif // ZTCP_H
