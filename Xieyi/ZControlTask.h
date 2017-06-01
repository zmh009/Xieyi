#ifndef ZCONTROLTASK_H
#define ZCONTROLTASK_H

#include <QDebug>

#include <QObject>
#include <QThread>
#include "ZTask.h"

#include "ZCapture.h"
#include "ZSetupWidget.h"
#include "ZResponseWidget.h"
#include "ZRestore.h"
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
    void promptWarn(const QString& title, const QString& text);
    void promptError(const QString& title, const QString& text);

private:
    // 底层应用功能
    int startCapture();
    int startRestore();
    int startResponse();

    ZTask mCaptureTask;
    ZTask mRestoreTask;
    ZTask mResponseTask;

    ZCapture mCapture;
    ZRestore mRestore;
    ZBehaviorAnalysis mBehaviorAnalysis;

    ZSetupWidget *mSetupWidget;

    ZInformation mInformation;

    QString mErrorTitle;
    QString mErrorText;
};

#endif // ZCONTROLTASK_H
