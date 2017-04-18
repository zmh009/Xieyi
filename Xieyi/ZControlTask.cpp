#include "ZControlTask.h"

ZControlTask::ZControlTask() :
    mResponseTask([](){qDebug() << "response";})
{
    // 获得配置界面，以根据选项启动任务
    mSetupWidget = ZSetupWidget::getSetupWidget();

    // 设置为允许跨线程调用信号槽，子线程里产生提示消息时可避免跨线程调用产生的崩溃
    connect(this, &ZControlTask::promptWarn, &mInformation, &ZInformation::promptWarn);
    connect(this, &ZControlTask::promptError, &mInformation, &ZInformation::promptError);

    // 移动任务的执行位置为线程位置，以防止跨线程调用信号槽而产生的崩溃
//    mCaptureTask.moveToThread(&mCaptureThread);
//    mRestoreTask.moveToThread(&mRestoreThread);
//    mResponseTask.moveToThread(&mResponseThread);
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
//    mCaptureThread.wait();

    mRestore.stop();
    mRestoreTask.wait();
//    mRestoreThread.wait();

    mResponse.stop();
    mResponseTask.wait();
//    mResponseThread.wait();
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

    mCaptureTask.setFunction(
                [&]()
                {
                   if (mCapture.start() < 0)
                   {
                       mErrorTitle = "捕获停止";
                       mErrorText = mCapture.getError();
                       emit this->promptError(mErrorTitle, mErrorText);
                       std::cout << "show end" << endl;
                   }
                }); // lamdba可调用对象
    mCaptureTask.start(); // 启动线程
//    mCaptureThread.start();// 启动线程
    return 0;
}

int ZControlTask::startRestore()
{
    if (mSetupWidget->isStoreToFile()) // 存储到文件
    {
        mRestoreTask.setFunction([&](){ // lamdba可调用对象，要获取存储的位置
            QString dirPath = mSetupWidget->getDirPath();
            if (0 > mRestore.restoreToFile(dirPath.toStdString()))
            {
                mErrorTitle = "恢复停止";
                mErrorText = mRestore.getError().c_str();
                emit this->promptError(mErrorTitle, mErrorText);
            }
        });
    }
    else if (mSetupWidget->isStoreToSQL()) // 存储到数据库
    {
        mRestoreTask.setFunction([&](){ // lamdba可调用对象，要获取存储的位置
            QString dbName = mSetupWidget->getDbName();
            if ( 0 > mRestore.restoreToSQL(dbName.toStdString()))
            {
                mErrorTitle = "恢复停止";
                mErrorText = mRestore.getError().c_str();
                emit this->promptError(mErrorTitle, mErrorText);
            }
        });

    }
    // 启动线程
    mRestoreTask.start();
//    mRestoreThread.start();
    return 0;
}

int ZControlTask::startResponse()
{
    mResponseTask.start();
//    mResponseThread.start();
    return 0;
}

//void ZControlTask::endTask(const ZTask &task)
//{
//    // end action
////    task.end();
//}
