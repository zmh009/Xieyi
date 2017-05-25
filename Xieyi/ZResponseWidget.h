#ifndef ZRESPONSEWIDGET_H
#define ZRESPONSEWIDGET_H

#include <string>
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QHeaderView>
using std::string;

class ZResponseWidget : public QWidget
{
    Q_OBJECT
public:
    ~ZResponseWidget();
    static ZResponseWidget* getResponseWidget(QWidget *parent = 0);

signals:

public slots:
    void add(const std::__cxx11::string &time, const string &type, const string &behavior, unsigned int size); // 添加一行信息
    void clear(); // 清除所有信息

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    explicit ZResponseWidget(QWidget *parent = 0);

private:
    QTableWidget *mInformationArea;
    static ZResponseWidget *mSelf;
};

#endif // ZRESPONSEWIDGET_H
