#ifndef _TIMERTASK_H
#define _TIMERTASK_H
#include "../myhead.h"

class TimerTask {
public: 
    virtual ~TimerTask() {}
    virtual void process() = 0;
};

#endif //_TIMERTASK_H