#ifndef ZRESTORE_H
#define ZRESTORE_H

#include <string>
#include "ZProcotol.h"
#include "ZFile.h"
#include "ZSQL.h"
#include "ZBuffer.h"
using std::string;

// 捕获数据明细
struct ZItem
{
    bool mIsBinary;
    int mLength;
    int mSrcPort;
    int mDstPort;
    string mDataType;
    string mCompressionType;
    string mEncodeType;
    string mSrcIp;
    string mDstIp;
    string mData;
    static time_t mIndex; // 使用时间计数类型，唯一性范围不小于时间戳范围，每次使用后递增
};

//--test
#include <QThread>
#include <iostream>
using std::cout;
using std::endl;
//enum RestoreStatusE
//{
//    NORESTORE = 0,
//    RESTORE_TO_FILE,
//    RESTORE_TO_SQL
//};

class ZRestore
{
public:
    ZRestore();
    ~ZRestore();
//    void start(RestoreStatusE status);
    void restoreToFile(const string &dirPath);
    void restoreToSQL(const string &dbName);

    void stop();

    ZItem getItem(const string &networkData);

public:
//    enum RestoreStatusE
//    {
//        NORESTORE = 0,
//        RESTORE_TO_FILE,
//        RESTORE_TO_SQL
//    };

private:
    ZBuffer *mBuffer;
    ZBuffer::readHandlerT mBufferHandler;
    ZMac mMac;
    ZIpv4 mIpv4;
    ZTcp mTcp;
    ZUdp mUdp;
    ZHttp mHttp;
    ZHttps mHttps;
    ZFile mFile;
    bool mRun;
};

#endif // ZRESTORE_H
