#ifndef ZCONTROLTASK_H
#define ZCONTROLTASK_H

#include <QDebug>

#include <QObject>
#include <QThread>
#include "ZTask.h"

//#include "ZPcapOption.h"
#include "ZCapture.h"
#include "ZSetupWidget.h"
#include "ZResponseWidget.h"
#include "ZRestore.h"
//#include "ZResponse.h"
#include "ZBehaviorAnalysis.h"
#include "ZInformation.h"


class ZControlTask : public QObject
{
    Q_OBJECT
public:
    ZControlTask();
    ~ZControlTask();

    void startTask();
    void stopTask();

signals:
//    void
    void promptWarn(const QString& title, const QString& text);
//    void promptWarn(const char* title, const char* text);
    void promptError(const QString& title, const QString& text);
//    void promptError(const char* title, const char* text);

private:
    // 底层应用功能
    int startCapture();
    int startRestore();
    int startResponse();

    //void endTask(const ZTask &task);

//    QThread mCaptureThread;
//    QThread mRestoreThread;
//    QThread mResponseThread;

    ZTask mCaptureTask;
    ZTask mRestoreTask;
    ZTask mResponseTask;

    ZCapture mCapture;
    ZRestore mRestore;
//    ZResponse mResponse;
    ZBehaviorAnalysis mBehaviorAnalysis;

    ZSetupWidget *mSetupWidget;

    ZInformation mInformation;

    QString mErrorTitle;
    QString mErrorText;
};

#endif // ZCONTROLTASK_H
