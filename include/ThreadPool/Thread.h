#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>
#include "../myhead.h"
using std::function;
using std::bind;

using ThreadCallback = function<void()>;


class Thread
{
public:
    Thread(ThreadCallback &&cb, const string & name);
    ~Thread();
    void start();
    void join();

private:
    //线程入口函数
    static void *threadFunc(void *arg);

private:
    pthread_t _thid;
    string _name;
    bool _isRunning;
    //要去实现的任务
    ThreadCallback _cb;

};

#endif
