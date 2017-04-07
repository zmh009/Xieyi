#ifndef ZCONTROLTASK_H
#define ZCONTROLTASK_H

#include <QDebug>
#include "ZTask.h"

//#include "ZPcapOption.h"
#include "ZCapture.h"
#include "ZSetupWidget.h"
#include "ZResponseWidget.h"
#include "ZRestore.h"
#include "ZResponse.h"

class ZControlTask
{
public:
    ZControlTask();
    ~ZControlTask();

    void startTask();
    void stopTask();

private:
    // 底层应用功能
    int startCapture();
    int startRestore();
    int startResponse();

    //void endTask(const ZTask &task);

    ZTask mCaptureTask;
    ZTask mRestoreTask;
    ZTask mResponseTask;

    ZCapture mCapture;
    ZRestore mRestore;
    ZResponse mResponse;
    ZSetupWidget *mSetupWidget;
};

#endif // ZCONTROLTASK_H
