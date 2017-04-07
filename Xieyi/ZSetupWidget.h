/*
 * 用于用户配置行为，属于输入模块的窗口
*/

#ifndef ZSetupWidget_H
#define ZSetupWidget_H
#define TIME_MAX_LENGTH 9

//#include <string>
#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
//using std::__cxx11::string;
//#include <QGridLayout>

class ZSetupWidget : public QWidget
{
    Q_OBJECT
public:
    ~ZSetupWidget();

    bool isNotStore() const;
    bool isStoreToSQL() const;
    bool isStoreToFile() const;

    QString getDirPath() const;
    QString getDbName() const;
    QString getFilter() const;
    QString getNetcard() const;
    QString getTime() const;

    // 通过传入指针，使用Qt的内存管理特性可防止内存泄露
    static ZSetupWidget* getSetupWidget(QWidget *parent = nullptr);

private:
    ZSetupWidget(QWidget *parent = 0);
    void initFilterTask();
    void initNetcardTask();
    void initTimeTask();
    void initRuleTask();

    void initNoStoreTask();
    void initStoreToSQLTask();
    void initStoreToFileTask();
    void initStoreTask();

    void initLayout();

private:
    QLineEdit *mFilter;
    QLineEdit *mNetcard;
    QLineEdit *mTime;
    QLineEdit *mDirPath;
    QLineEdit *mDbName;

    QRadioButton *mNoStore;
    QRadioButton *mStoreToSQL;
    QRadioButton *mStoreToFile;

    QVBoxLayout mMainLayout; // 主布局
    QVBoxLayout mRuleLayout; // 规则布局
    QVBoxLayout mLongVerticalLayout; // 长垂直规则布局
    QHBoxLayout mShortHorizontalLayout; // 短水平规则布局
    QVBoxLayout mStoreLayout; // 存储布局
    QHBoxLayout mFileStoreLayout; // 文件存储布局
    QHBoxLayout mSQLStoreLayout; // SQL存储布局

    static ZSetupWidget *mSelf;
};

#endif // ZSetupWidget_H
