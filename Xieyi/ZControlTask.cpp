#include "ZControlTask.h"

ZControlTask::ZControlTask()
{
    // 获得配置界面，以根据选项启动任务
    mSetupWidget = ZSetupWidget::getSetupWidget();

    // 设置为允许跨线程调用信号槽，子线程里产生提示消息时可避免跨线程调用产生的崩溃
    connect(this, &ZControlTask::promptWarn, &mInformation, &ZInformation::promptWarn);
    connect(this, &ZControlTask::promptError, &mInformation, &ZInformation::promptError);
}

ZControlTask::~ZControlTask()
{
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

    mBehaviorAnalysis.stop();
    mResponseTask.wait();
}

int ZControlTask::startCapture()
{
    // 获取捕获数据应用的配置信息
    QString netcard = mSetupWidget->getNetcard();
    QString filter = mSetupWidget->getFilter();
    QString waitTime = mSetupWidget->getTime();

    // 设置捕获数据应用的配置信息
    mCapture.setNetcard(netcard.toStdString());
    mCapture.setFilter(filter.toStdString());
    if (waitTime.length() < TIME_MAX_LENGTH)
    {
        u_int time = waitTime.toUInt();
        if (time <= 0)
        {
            time = OPTION_WAIT_TIME;
        }
        mCapture.setWaitTime(time);
    }

    mCaptureTask.setFunction(
                [&]()
                {
                   if (mCapture.start() < 0)
                   {
                       mErrorTitle = "捕获停止";
                       mErrorText = mCapture.getError();
                       emit this->promptError(mErrorTitle, mErrorText);
                   }
                }); // lamdba可调用对象
    mCaptureTask.start(); // 启动线程
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
    return 0;
}

int ZControlTask::startResponse()
{
    mResponseTask.setFunction([&](){ // lamdba可调用对象
        if (0 > mBehaviorAnalysis.start())
        {
            mErrorTitle = "行为分析停止";
            mErrorText = mBehaviorAnalysis.getError().c_str();
            emit this->promptError(mErrorTitle, mErrorText);
        }
    });
    mResponseTask.start();
    return 0;
}
