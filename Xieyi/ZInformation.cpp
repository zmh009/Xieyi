#include "ZInformation.h"

ZInformation::ZInformation(QWidget *parent)
{
    mMessageBox = new QMessageBox(parent);
    mMessageBox->setStandardButtons(QMessageBox::Ok);
}

ZInformation::~ZInformation()
{
    delete mMessageBox;
}

void ZInformation::promptWarn(const QString &title, const QString &text)
{
    // Qt 警告对话框
//    QMessageBox::warning(nullptr, QString(title.c_str()), QString(text.c_str()));
    mMessageBox->setWindowTitle(title);
    mMessageBox->setText(text);
    mMessageBox->setIcon(QMessageBox::Warning);
    mMessageBox->show();
}

void ZInformation::promptError(const QString &title, const QString &text)
{
    // Qt 警告对话框
//    QMessageBox::critical(nullptr, QString(title.c_str()), QString(text.c_str()));
    mMessageBox->setWindowTitle(title);
    mMessageBox->setText(text);
    mMessageBox->setIcon(QMessageBox::Critical);
    mMessageBox->show();
}
