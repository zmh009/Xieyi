/*
 * 获取网络流量信息，并以此分析出上网行为，作出响应，不关注应用层数据，因为在恢复模块已有处理
 */

#ifndef ZBEHAVIORANALYSIS_H
#define ZBEHAVIORANALYSIS_H
#define CONFIGURATION_PATH "./behaviorAnalysis.conf"
#define INTERVAL_TIME 1
#define MAX_COMMUNICATION_TIMES (~(1 << 31))
#define MIN_COMMUNICATION_TIMES 0
#define LOG_PATH "./logfile.log"

#include <map>
#include <string>
#include <libconfig.h++>
#include <ctime>
#include "ZProcotol.h"
#include "ZBuffer.h"
#include "ZBeheviorResponse.h"
#include "ZResponseWidget.h"
using std::map;
using std::string;
using libconfig::Config;

class ZBehaviorAnalysis
{
    struct ZDataItem;
    struct ZCommunicationItem;

public:
    ZBehaviorAnalysis();
    ~ZBehaviorAnalysis();

    int start();
    inline int stop(){mRun = false;return 0;}
    inline string getError() const {return mError;}

private:
    // 得到待分析的网络数据
    string getNetworkData(){return mBuffer->read(mReadHandle);}
    // 得到流量详情
    ZDataItem getItem(const string &networkData);

    // 记录网络行为
    int recordBehavior(const ZDataItem &dataItem);

    // 在用户界面上显示流量
    int showOnGUi(const ZDataItem &dataItem);

    // 访问量过量响应
    int moreVisitResponse(const ZDataItem &dataItem);

    int readConfiguration(const string &configurationPath);
private:
    // 存储数据详情，以用于数据分析
    struct ZDataItem
    {
        ZDataItem(){mLength = 0; mSrcPort = 0; mDstPort = 0;}
        int mLength;    // 捕获数据的总长度
        int mSrcPort;   // 捕获数据的源端口
        int mDstPort;   // 捕获数据的目的端口
        string mSrcIp;  // 捕获数据的源IP
        string mDstIp;  // 捕获数据的目的IP
        string mSrcMac; // 捕获数据的源MAC
        string mDstMac; // 捕获数据的目的MAC
        string mProcotolType; // 捕获数据的协议类型
    }/* mDataItem*/;

    // 每次网络通信的详情
    struct ZCommunicationItem
    {
        ZCommunicationItem(){mCount = 0; mTimeStamp = 0;}
        int mCount; // 在1秒内通信的总次数
        time_t mTimeStamp; // 第一次通信的时间戳
    }/* mCommunicationItem*/;

    ZBuffer* mBuffer;  // 存储网络数据的缓冲区
    ZBuffer::readHandlerT mReadHandle; // 缓冲区读句柄
    ZBeheviorResponse mBehaviorResponse;
    bool mRun;
    string mError;

    int mMaxCommunicationTimes; // 最大通信次数，配置文件读取
    int mMinCommunicationTimes; // 最小通信次数，配置文件读取
    string mLogPath; // 日志文件路径，配置文件读取

    // 因为MAC地址不易改变，因此使用MAC地址标识每次的通信
    map<string, ZCommunicationItem> mRecord;
};

#endif // ZBEHAVIORANALYSIS_H
