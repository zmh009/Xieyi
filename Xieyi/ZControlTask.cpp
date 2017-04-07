#include "ZControlTask.h"

ZControlTask::ZControlTask() :
    mResponseTask([](){qDebug() << "response";})
{
    mSetupWidget = ZSetupWidget::getSetupWidget();
}

ZControlTask::~ZControlTask()
{
//    endTask(mCaptureTask);
//    endTask(mRestoreTask);
//    endTask(mResponseTask);
    stopTask();
}

void ZControlTask::startTask()
{
    startCapture();
    startRestore();
    startResponse();
}

// 等待所有任务执行完毕
void ZControlTask::stopTask()
{
    mCapture.stop();
    mCaptureTask.wait();

    mRestore.stop();
    mRestoreTask.wait();

    mResponse.stop();
    mResponseTask.wait();
}

int ZControlTask::startCapture()
{
    // 获取捕获数据应用的配置信息
    QString netcard = mSetupWidget->getNetcard();
    QString filter = mSetupWidget->getFilter();
    QString waitTime = mSetupWidget->getTime();

    // 设置捕获数据应用的配置信息
    if (!netcard.isEmpty())
    {
        mCapture.setNetcard(netcard.toStdString().c_str());
    }
    if (!filter.isEmpty())
    {
        mCapture.setFilter(filter.toStdString().c_str());
    }
    if (!waitTime.isEmpty() && waitTime.length() < TIME_MAX_LENGTH)
    {
        mCapture.setWaitTime(waitTime.toUInt());
    }

    mCaptureTask.setFunction([&](){mCapture.start();}); // lamdba可调用对象
    mCaptureTask.start(); // 启动线程
    return 0;
}

int ZControlTask::startRestore()
{
    if (mSetupWidget->isStoreToFile()) // 存储到文件
    {
        mRestoreTask.setFunction([&](){ // lamdba可调用对象，要获取存储的位置
            QString dirPath = mSetupWidget->getDirPath();
            mRestore.restoreToFile(dirPath.toStdString());
        });
        // 启动线程
        mRestoreTask.start();
    }
    else if (mSetupWidget->isStoreToSQL()) // 存储到数据库
    {
        mRestoreTask.setFunction([&](){ // lamdba可调用对象，要获取存储的位置
            QString dbName = mSetupWidget->getDbName();
            mRestore.restoreToSQL(dbName.toStdString());
        });
        // 启动线程
        mRestoreTask.start();
    }

    return 0;
}

int ZControlTask::startResponse()
{
    mResponseTask.start();
    return 0;
}

//void ZControlTask::endTask(const ZTask &task)
//{
//    // end action
////    task.end();
//}
