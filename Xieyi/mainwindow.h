/*
 * 用于布局每个UI模块，包括工具栏、输入交互模块、显示交互模块，整体是一个Table
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QKeySequence>
#include "ZSetupWidget.h"
#include "ZResponseWidget.h"
#include "ZControlTask.h"

// Debug
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 初始化每个UI模块
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initTasks();
    void initStartTask();
    void initStopTask();
    void initRestartTask();
    void initClearTask();
    void initHelpTask();
    void initToolBar();
    void initMenus();
    void initTable();

private slots:
    void startSlot();
    void stopSlot();
    void clearSlot();
    void restartSlot();
    void helpSlot();

private:
    Ui::MainWindow *ui;
    // 使用指针并在相应位置初始化，使用QT的内存特性管理
    ZControlTask *mControlTask;
    ZSetupWidget *mSetupWiddget;
    ZResponseWidget *mResponseWidget;
    QAction *mStartAction;
    QAction *mStopAction;
    QAction *mRestartAction;
    QAction *mClearAction;
    QAction *mHelpAction;
    QMenu *mControlMenu;
    QMenu *mHelpMenu;
    QTabWidget *mMainWidget;
};

#endif // MAINWINDOW_H
