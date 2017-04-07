#include "mainwindow.h"
#include "ui_mainwindow.h"

/* 初始化所有任务、工具栏、菜单、用户交换界面和整体的Table界面
 * 工具栏与菜单需要已初始化的任务去初始化
 * 整体的Table界面则需要用户交换界面初始化
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mSetupWiddget = ZSetupWidget::getSetupWidget(this);
    mResponseWidget = ZResponseWidget::getResponseWidget(this);
    // 在Qt的内存管理机制中，主窗口的两个界面要为主界面的子树，并且使用单例模型在其他位置被访问，
    // 因此此处的ZControlTask成员变量使用并在两个界面之后指针初始化
    mControlTask = new ZControlTask;
    setWindowTitle("捕获应用");
    resize(1000,600);
    // 初始化所有任务
    initTasks();
    // 初始化工具栏
    initToolBar();
    // 初始化菜单
    initMenus();
    // 初始化整体的Table界面
    initTable();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (mControlTask != nullptr)
    {
        delete mControlTask;
    }
}

/* 初始化所有需要的交互任务
*/
void MainWindow::initTasks()
{
    initStartTask();
    initStopTask();
    initRestartTask();
    initClearTask();
    initHelpTask();
}

/* 开始任务将初始化开始行为，包括图标、快捷键、说明，同时定义开始的信号槽
*/
void MainWindow::initStartTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mStartAction = new QAction(QIcon(":/picture/start"), "开始", this);
    mStartAction->setShortcut(QKeySequence("ctrl+r"));
    mStartAction->setStatusTip("开始并转到显示界面");
    connect(mStartAction, &QAction::triggered, this, &MainWindow::startSlot);
}

void MainWindow::initStopTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mStopAction = new QAction(QIcon(":/picture/stop"), "停止", this);
    mStopAction->setShortcut(QKeySequence("ctrl+s"));
    mStopAction->setStatusTip("停止捕获数据");
    mStopAction->setDisabled(true); // 初始不使用
    connect(mStopAction, &QAction::triggered, this, &MainWindow::stopSlot);
}

void MainWindow::initRestartTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mRestartAction = new QAction(QIcon(":/picture/restart"), "重启", this);
    mRestartAction->setShortcut(QKeySequence("ctrl+shift+r"));
    mRestartAction->setStatusTip("重新捕获数据同时清空显示界面");
    mRestartAction->setDisabled(true); // 初始不使用
    connect(mRestartAction, &QAction::triggered, this, &MainWindow::restartSlot);
}

void MainWindow::initClearTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mClearAction = new QAction(QIcon(":/picture/clear"), "清空", this);
    mClearAction->setShortcut(QKeySequence("ctrl+c"));
    mClearAction->setStatusTip("清空捕获的数据与显示界面");
    connect(mClearAction, &QAction::triggered, this, &MainWindow::clearSlot);
}

void MainWindow::initHelpTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mHelpAction = new QAction(QIcon(":/picture/help"), "帮助", this);
    mHelpAction->setShortcut(QKeySequence("ctrl+h"));
    mHelpAction->setStatusTip("打开帮助界面");
    connect(mHelpAction, &QAction::triggered, this, &MainWindow::helpSlot);
}

/*
 * 初始化工具条，将所需任务添加到该工具条里
*/
void MainWindow::initToolBar()
{
    QToolBar *toolBar = ui->mainToolBar;
    QList<QAction *> toolBarActions({mStartAction, mStopAction,
                                     mRestartAction, mClearAction});
    toolBar->addActions(toolBarActions);
}

/* 初始化控制与帮助菜单，将所需任务添加到对应菜单里
*/
void MainWindow::initMenus()
{
    QList<QAction *> controlAction({mStartAction, mStopAction, mRestartAction, mClearAction});
    QList<QAction *> helpAction({mHelpAction});

    QMenu *controlMenu = new QMenu("控制", this);
    controlMenu->addActions(controlAction);
    QMenu *helpMenu = new QMenu("帮助", this);
    helpMenu->addActions(helpAction);

    QMenuBar *menuBar = ui->menuBar;
    menuBar->addMenu(controlMenu);
    menuBar->addMenu(helpMenu);
}

void MainWindow::initTable()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mMainWidget = new QTabWidget(this);
    mMainWidget->addTab(mSetupWiddget, "设置");
    mMainWidget->addTab(mResponseWidget, "显示");
    setCentralWidget(mMainWidget);
}

void MainWindow::startSlot()
{
    mMainWidget->setCurrentIndex(1);
    mStartAction->setDisabled(true); // 启动时不可再次使用
    mStopAction->setEnabled(true); // 启动时停止功能可用
    mRestartAction->setEnabled(true); // 启动时重启功能可用

    mControlTask->startTask();
}

void MainWindow::stopSlot()
{
    mStartAction->setEnabled(true); // 停止时开始功能可用
    mStopAction->setDisabled(true); // 停止时停止功能不可用
    mRestartAction->setDisabled(true); // 停止时重启功能不可用

    // 等待任务执行完后停止，并非立刻终止
    mControlTask->stopTask();
}

void MainWindow::clearSlot()
{
    qDebug() << "clear";
    mResponseWidget->clear();
}

/* 重启时，会停止捕获应用，同时清空捕获数据后启动
*/
void MainWindow::restartSlot()
{
    stopSlot();
    clearSlot();
    startSlot();
}

void MainWindow::helpSlot()
{
    qDebug() << "help";
}
