#include "ZBuffer.h"

ZBuffer* ZBuffer::mSelf = nullptr;

ZBuffer::ZBuffer(int readerCount) : mBufferPool(readerCount)
{
    mCurrentCount = 0;
}

ZBuffer::~ZBuffer()
{

}

ZBuffer *ZBuffer::getBuffer(int readerCount)
{
    if (mSelf == nullptr)
    {
        mSelf = new ZBuffer(readerCount);
    }
    return mSelf;
}

ZBuffer::readHandlerT ZBuffer::openRead()
{
    if (mCurrentCount < mBufferPool.size())
    {
        return mCurrentCount++;
    }
    else
    {
        return -1;
    }

}

void ZBuffer::closeRead(const ZBuffer::readHandlerT &handler)
{
    ;
}

//
ZBuffer::networkDataT ZBuffer::read(const readHandlerT &handler)
{
    bufferT &buffer = mBufferPool[handler];
    if (buffer.empty())
    {
        return networkDataT();
    }
    networkDataT data = buffer.front();
    buffer.pop();
    return data;
}

//void ZBuffer::closeRead(const readHandlerT &handler)
//{
//    mBufferPool.erase(handler);
//}

//int ZBuffer::addData(const u_char *networkData)
//{
//    qDebug()<<"in addData u_char";
//    QThread::sleep(1);
//    return 0;
//}

//int ZBuffer::addData(const ZNetworkData &networkData)
//{
////    qDebug()<<"in addData networkData, add length is "<< networkData.length();
////    QThread::sleep(1);
//    for (auto &each : mBufferPool)
//    {
//        qDebug() << "add length is "<< networkData.length();
//        each.push(networkData);
//    }
//    return 0;
//}

int ZBuffer::addData(const string &networkData)
{
    for (auto &each : mBufferPool)
    {
        each.push(networkData);
    }
    return 0;
}

ZBuffer::recycle::~recycle()
{
    if (ZBuffer::mSelf != nullptr)
    {
        delete ZBuffer::mSelf;
        ZBuffer::mSelf = nullptr;
    }
}
