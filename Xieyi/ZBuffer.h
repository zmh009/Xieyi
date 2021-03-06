#ifndef ZBUFFER_H
#define ZBUFFER_H
#define READER_COUNT 2

#include <string>
#include <queue>
#include <vector>
using std::string;
using std::queue;
using std::vector;

class ZBuffer
{
public:
    typedef int readHandlerT;
    typedef string networkDataT;
    typedef queue<networkDataT> bufferT;
public:
    ~ZBuffer();
    static ZBuffer *getBuffer(int readerCount = READER_COUNT);

    readHandlerT openRead();
    void closeRead(const readHandlerT &handler);

    networkDataT read(const readHandlerT &handler);
    int addData(const string &networkData);
private:
    ZBuffer(int readerCount);

private:
    // 单例模式的回收成员
    class recycle
    {
        ~recycle();
    };
    static recycle rle;
private:
    vector<bufferT> mBufferPool;
    int mCurrentCount;
    static ZBuffer *mSelf;
};

#endif // ZBUFFER_H
