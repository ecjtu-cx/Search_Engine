#ifndef __CACHETIMERTASK_H__
#define __CACHETIMERTASK_H__

#include "TimerTask.h"

class CacheTimerTask
: public TimerTask
{
public:
    virtual ~CacheTimerTask() {}
    void process() override;
};


#endif