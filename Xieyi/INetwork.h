#ifndef INETWORK_H
#define INETWORK_H

#include <string>
using std::string;

typedef unsigned char u_char;

class INetwork
{
public:
    virtual void setNetworkContent(const u_char *content, int length) = 0; // set the Content of network layer

    virtual const u_char *networkContent() const = 0; //get the Content of network layer
    virtual int networkLen() const = 0; // get the length of network layer

    virtual const u_char *transportContent() const = 0; // get the Content of transport layer
    virtual int transportLen() const = 0; // get the length of transport layer
    virtual int transportType() const = 0;

    virtual string srcIp() const = 0;
    virtual string dstIp() const = 0;
};

#endif // INETWORK_H
