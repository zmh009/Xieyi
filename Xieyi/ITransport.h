#ifndef ITRANSPORT_H
#define ITRANSPORT_H

#define PORT_LENGTH 2

#define DNS_PORT 53
#define HTTP_PORT 80
#define HTTPS_PORT 443

//#include <string>
//using std::string;

typedef unsigned char u_char;
//typedef unsigned int u_int;

class ITransport
{
public:
    virtual const u_char *transportContent() const = 0; //get the Content of transport layer
    virtual int transportLen() const = 0; // get the length of transport layer
    virtual void setTransportContent(const u_char *Content,int length) = 0; // set the Content of transport layer

    virtual const u_char *applicationContent() const = 0; // get the Content of application layer
    virtual int applicationLen() const = 0; // get the Content of application layer

    virtual int srcPort() const = 0;
    virtual int dstPort() const = 0;
};

#endif // ITRANSPORT_H

