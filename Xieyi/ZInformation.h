/* 因为在子线程里，如果要检测运行出现的错误，则需要同步等待初始化时的错误出现，代码编写与性能开销较大，
 * 因此在运行有错误时通过ZInformation类输出提示并在子线程里相应处理
 */

#ifndef ZInformation_H
#define ZInformation_H

#include <string>
#include <QMessageBox>
#include <QString>
#include <QDialog>
using std::string;

class ZInformation : public QObject
{
    Q_OBJECT
public:
    ZInformation(QWidget *parent = nullptr);
    ~ZInformation();

    void promptWarn(const QString& title, const QString &text);
    void promptError(const QString& title, const QString &text);

private:
    QMessageBox *mMessageBox;
};

#endif // ZInformation_H
