#ifndef ZTASK_H
#define ZTASK_H

#include <QThread>
#include <functional>
using std::function;

class ZTask : public QThread
{
public:
    ZTask();
    ~ZTask();
    ZTask(const function<void ()> &task);

    virtual void run();
    void setFunction(const function<void ()> &task);

//    void stop();

private:
    function<void ()> mFunction;
};

#endif // ZTASK_H
