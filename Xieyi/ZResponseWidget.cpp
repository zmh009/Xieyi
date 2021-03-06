#include "ZResponseWidget.h"

ZResponseWidget* ZResponseWidget::mSelf = nullptr;

ZResponseWidget::ZResponseWidget(QWidget *parent) : QWidget(parent)
{
    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    mInformationArea = new QTableWidget(0,4,this);
    mInformationArea->setHorizontalHeaderLabels(QStringList{"时间","类型","行为","大小"}); // 设置水平表头标签
    mInformationArea->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置为不可编辑
    mInformationArea->setSelectionMode(QAbstractItemView::NoSelection); // 设置为不可选取

    // 通过传入this，使用Qt的内存管理特性可防止内存泄露
    QVBoxLayout *showLayout = new QVBoxLayout(this);
    showLayout->addWidget(mInformationArea);
    setLayout(showLayout); // 设置布局
}

ZResponseWidget::~ZResponseWidget()
{
    ;
}

ZResponseWidget *ZResponseWidget::getResponseWidget(QWidget *parent)
{
    if (mSelf == nullptr)
    {
        mSelf = new ZResponseWidget(parent);
    }
    return mSelf;
}

// 当窗口大小变化时产生该事件并捕捉重写
void ZResponseWidget::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    // 设置每列的宽度
    mInformationArea->setColumnWidth(0, size.width()/10*2);
    mInformationArea->setColumnWidth(1, size.width()/10*2);
    mInformationArea->setColumnWidth(2, size.width()/10*5);
    mInformationArea->setColumnWidth(3, size.width()/10*1);
}

void ZResponseWidget::add(const string &time, const string &type, const string &behavior, unsigned int size)
{
    int row = mInformationArea->rowCount();
    mInformationArea->insertRow(row);

    // 会转交QTableWidgetItme的所有权，避免内存泄露
    QString timeShow = QString(time.c_str());
    mInformationArea->setItem(row, 0, new QTableWidgetItem(timeShow));
    QString typeShow = QString(type.c_str());
    mInformationArea->setItem(row, 1, new QTableWidgetItem(typeShow));
    QString behaviorShow = QString(behavior.c_str());
    mInformationArea->setItem(row, 2, new QTableWidgetItem(behaviorShow));
    QString sizeShow = QString::number(size);
    mInformationArea->setItem(row, 3, new QTableWidgetItem(sizeShow));
}

void ZResponseWidget::clear()
{
    mInformationArea->clearContents();
    mInformationArea->setRowCount(0);
}
