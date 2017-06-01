#ifndef ZTASK_H
#define ZTASK_H

#include <functional>
#include <string>
#include <QThread>
using std::function;
using std::string;
typedef function<void ()> FunctionT;

class ZTask : public QThread
{
    Q_OBJECT
public:
    ZTask();
    ~ZTask();
    ZTask(const FunctionT &task);

    virtual void run();
    void setFunction(const FunctionT &task);

private:
    FunctionT mFunction;
};

#endif // ZTASK_H
