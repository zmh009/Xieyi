#include "ZTask.h"

// 初始任务为空
ZTask::ZTask() : mFunction([](){;})
{
    ;
}

ZTask::~ZTask()
{
    ;
}

ZTask::ZTask(const FunctionT &task) : mFunction(task)
{
    ;
}

void ZTask::run()
{
    mFunction();
}

void ZTask::setFunction(const FunctionT &task)
{
    mFunction = task;
}

//void ZTask::stop()
//{

//}
