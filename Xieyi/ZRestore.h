/* 恢复数据，支持以太网里IP下TCP、UDP协议，恢复一个应用层的数据段，对于HTTP、HTTPS有特殊的恢复方式。
 * HTTP：
 * 请求包为普通恢复
 * 响应包为恢复POST数据，如果POST数据为空则恢复响应头
 * HTTPS：
 * 以二进制方式恢复
 */
#ifndef ZRESTORE_H
#define ZRESTORE_H
#define SEQUENCE_KEY    "sequence"
#define DATA_TYPE_KEY   "dataType"
#define ENCODE_TYPE_KEY         "encodeType"
#define COMPRESSION_TYPE_KEY    "compressionType"

#include <string>
#include <map>
#include <ctime>
#include "ZProcotol.h"
#include "ZFile.h"
#include "ZSQL.h"
#include "ZBuffer.h"
#include "ZInformation.h"
using std::string;
using std::map;
//typedef string DataIndexT;
//typedef time_t
typedef map<string, string> RecordT;

// 捕获数据明细
struct ZItem
{
    ZItem();
    enum RecombineE{NOT_RECOMBINE, RECOMBINEING, FINISH} mRecombine; // 重组状态，有无须重组、重组中、结束
    bool mIsBinary; // 是否为二进制数据
//    int  mIsFinal;  // 是否为一次会话的结束
    int mLength;    // 捕获数据的长度
    int mSrcPort;   // 捕获数据的源端口
    int mDstPort;   // 捕获数据的目的端口
    string mDataType;   // 捕获数据的数据类型,以文件后缀名表示
    string mCompressionType;    // 捕获数据的压缩类型,以文件后缀名表示
    string mEncodeType; // 捕获数据的加密类型,以文件后缀名表示
    string mSrcIp;  // 捕获数据的源IP
    string mDstIp;  // 捕获数据的目的IP
    string mData;   // 捕获的数据
//    string mStorageIndex;  // 数据的存储索引
    string mSequence; // 数据的序号
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

// 除去捕获的空数据报文?
class ZRestore
{
public:
    ZRestore();
    ~ZRestore();
//    void start(RestoreStatusE status);
    int restoreToFile(string dirPath);
    int restoreToSQL(string dbName);

    void stop();

    string getError() const;

    ZItem getItem(const string &networkData);

public:
//    enum RestoreStatusE
//    {
//        NORESTORE = 0,
//        RESTORE_TO_FILE,
//        RESTORE_TO_SQL
//    };
private:
//    void getIndex();
    void setRecombineIndex(ZItem& item);
    time_t getUniqueTime();
    void clearHistory();
    const string stringAdd(const string &str1, const string &str2) const;
//    INetwork *updateDatalinkItem(ZItem& item);
//    ITransport *updateNetworkItem(ZItem& item);
//    IApplication *updataTransportItem(ZItem& item);
//    void updataApplicationItem(ZItem& item);

private:
    ZBuffer *mBuffer;
    ZBuffer::readHandlerT mBufferHandler;
    ZMac mMac;
    ZIpv4 mIpv4;
    ZTcp mTcp;
    ZUdp mUdp;
    ZHttp mHttp;
    ZHttps mHttps;
    bool mRun;

//    static time_t mIndex; // 存储的索引,以支持数据分开存储,使用时间戳表示唯一的索引
    map<string, RecordT> mHistoryRecord; // 通过数据的IP和端口号映射历史记录，以支持数据重组
    map<string, time_t> mIndexPool; // 通过数据的IP和端口号映射数据的索引，以支持数据重组
    map<string, string> mTypePool; // 通过数据的IP和端口号映射数据类型,以支持数据恢复

    string mError;
};

#endif // ZRESTORE_H
