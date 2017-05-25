/*
 * 对网络行为的响应
 */

#ifndef ZBEHEVIORRESPONSE_H
#define ZBEHEVIORRESPONSE_H

#include <string>
#include <fstream>
using std::string;
using std::ofstream;

// test--
#include <QDebug>

class ZBeheviorResponse
{
public:
    ZBeheviorResponse();
    ~ZBeheviorResponse(){}

    int record(const string &information, const string &filePath);
    int refuseByIp(const string &ip);
};

#endif // ZBAHEVIORHANDLE_H
