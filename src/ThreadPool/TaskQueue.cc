#include "../../include/ThreadPool/TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{}

TaskQueue::~TaskQueue()
{}

bool TaskQueue::empty() const
{
    return _que.size() == 0;
}

bool TaskQueue::full() const
{
    return _queSize == _que.size();
}

void TaskQueue::push(Elem && value)
{
    {
    MutexLockGuard autoLock(_mutex);
    while(full())
    {
        _notFull.wait();
    }
    _que.push(value);
    }
    _notEmpty.notify();
}

Elem TaskQueue::pop()
{
    Elem tmp;
    MutexLockGuard autoLock(_mutex);
    while(_flag && empty())
    {
        _notEmpty.wait();
    }
    if(_flag) {
        tmp = _que.front();
        _que.pop();
        _notFull.notify();

        return tmp;
    } else {
        return nullptr;
    }
}

void TaskQueue::wakeup()
{
    _flag = false;
    _notEmpty.notifyAll();
}