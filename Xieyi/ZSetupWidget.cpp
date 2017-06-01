/* 配置界面用于接收过滤器表达式、捕获时使用的网卡名、捕获间隔时间信息，
 * 同时指定存储方式，包括不存储、存储在数据库、存储在文件。
*/

#include "ZSetupWidget.h"

ZSetupWidget* ZSetupWidget::mSelf = nullptr;

// 初始化规则任务、存储任务、布局任务
// 初始化布局任务依赖于之前的任务
ZSetupWidget::ZSetupWidget(QWidget *parent) : QWidget(parent)
{
    initRuleTask();
    initStoreTask();
    initLayout();
}

ZSetupWidget::~ZSetupWidget()
{
    ;
}

bool ZSetupWidget::isNotStore() const
{
    return mNoStore->isChecked();
}

bool ZSetupWidget::isStoreToSQL() const
{
    return mStoreToSQL->isChecked();
}

bool ZSetupWidget::isStoreToFile() const
{
    return mStoreToFile->isChecked();
}

// 获取当前控件显示的文件所在目录路径
QString ZSetupWidget::getDirPath() const
{
    QString dirPath = mDirPath->displayText();
    return dirPath;
}

QString ZSetupWidget::getDbName() const
{
    return mDbName->displayText();
}

// 获取当前控件显示的过滤器表达式
QString ZSetupWidget::getFilter() const
{
    QString filterExpression = mFilter->displayText();
    return filterExpression;
}

// 获取当前控件显示的网卡名
QString ZSetupWidget::getNetcard() const
{
    QString netcardName = mNetcard->displayText();
    return netcardName;
}

// 获得捕获时间间隔文本，长度不能大于TIME_MAX_LENGTH
QString ZSetupWidget::getTime() const
{
    QString time = mTime->displayText();
    return time;
}

ZSetupWidget *ZSetupWidget::getSetupWidget(QWidget *parent)
{
    if (mSelf == nullptr)
    {
        mSelf = new ZSetupWidget(parent);
    }

    return mSelf;
}

// 初始化过滤器控件，设置其默认提示
void ZSetupWidget::initFilterTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mFilter = new QLineEdit(this);
    mFilter->setPlaceholderText("过滤器表达式");
    // 启用清空按钮
    mFilter->setClearButtonEnabled(true);
}

// 初始化网卡控件，设置其默认提示
void ZSetupWidget::initNetcardTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mNetcard = new QLineEdit(this);
    mNetcard->setPlaceholderText("网卡名");
    // 启用清空按钮
    mNetcard->setClearButtonEnabled(true);
}

// 初始化时间控件，设置其默认提示
void ZSetupWidget::initTimeTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mTime = new QLineEdit(this);
    mTime->setPlaceholderText("捕获间隔时间(ms)");
    // 启用清空按钮
    mTime->setClearButtonEnabled(true);
}

// 初始化规则任务,包括过滤器任务、网卡任务、时间任务、
void ZSetupWidget::initRuleTask()
{
    initFilterTask();
    initNetcardTask();
    initTimeTask();
}

void ZSetupWidget::initNoStoreTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mNoStore = new QRadioButton("不存储", this);
    mNoStore->setChecked(true);
}

void ZSetupWidget::initStoreToSQLTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mStoreToSQL = new QRadioButton("存储到数据库", this);
    mDbName = new QLineEdit(this);
    mDbName->setPlaceholderText("默认存储的数据库名是ZRestoreLocation");
    mDbName->setDisabled(true);
    // 将单选操作与文本框是否可以写通过信号槽连接起来
    connect(mStoreToSQL, &QRadioButton::toggled, mDbName, &QLineEdit::setEnabled);
}

void ZSetupWidget::initStoreToFileTask()
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mStoreToFile = new QRadioButton("存储到文件", this);
    mDirPath = new QLineEdit(this);
    mDirPath->setPlaceholderText("默认存储路径是当前路径下的ZRestoreLocation/");
    mDirPath->setDisabled(true); // 初始不可用
    // 将单选操作与文本框是否可以写通过信号槽连接起来
    connect(mStoreToFile, &QRadioButton::toggled, mDirPath, &QLineEdit::setEnabled);
}

// 初始化存储任务
void ZSetupWidget::initStoreTask()
{
    initNoStoreTask();
    initStoreToSQLTask();
    initStoreToFileTask();
}

void ZSetupWidget::initLayout()
{
    // 长垂直规则布局包括过滤器表达式编辑框
    mLongVerticalLayout.addWidget(mFilter);
    // 短水平规则布局包括网卡和间隔时间编辑框
    mShortHorizontalLayout.addWidget(mNetcard);
    mShortHorizontalLayout.addStretch(); // 添加伸展，可平均分配伸展空间
    mShortHorizontalLayout.addWidget(mTime);
    mShortHorizontalLayout.addStretch();
    // 添加到规则布局
    mRuleLayout.addLayout(&mLongVerticalLayout);
    mRuleLayout.addStretch();
    mRuleLayout.addLayout(&mShortHorizontalLayout);

    // 文件存储布局包括文件选择按键和文件路径编辑框
    mFileStoreLayout.addWidget(mStoreToFile);
    mFileStoreLayout.addWidget(mDirPath);
    mFileStoreLayout.addStretch();
    mFileStoreLayout.setSpacing(10);

    // SQL存储布局包括SQL选择按键和数据库名编辑框
    mSQLStoreLayout.addWidget(mStoreToSQL);
    mSQLStoreLayout.addWidget(mDbName);
    mSQLStoreLayout.addStretch();
    mSQLStoreLayout.setSpacing(10);

    //添加到存储布局
    mStoreLayout.addWidget(mNoStore);
    mStoreLayout.addStretch();
    mStoreLayout.addLayout(&mSQLStoreLayout);
    mStoreLayout.addStretch();
    mStoreLayout.addLayout(&mFileStoreLayout);
    mStoreLayout.addStretch();

    // 添加到主布局
    mMainLayout.addLayout(&mRuleLayout, 1);
    mMainLayout.addStretch(1);
    mMainLayout.addLayout(&mStoreLayout, 2);
    mMainLayout.addStretch(1);

    // 设置主布局
    setLayout(&mMainLayout);
}
