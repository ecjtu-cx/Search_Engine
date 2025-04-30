#ifndef _TIMERMANAGER_H
#define _TIMERMANAGER_H
#include "TimerTask.h"

class TimerManager 
{
public: 
    static TimerManager *getTimerManager();
    static void init();
    static void destroy(); 
    void initTimer(int delay, int interval);

    void start();
    void stop();
    void attach(TimerTask*);
    void detach(TimerTask*);


private: 
    TimerManager();
    ~TimerManager() {}

    int createTimerfd();
    void setTimerfd(int delay, int interval);
    void handleRead();

    int _timerfd;
    int _delay;
    int _interval;
    bool _isStarted;

    static TimerManager *_tmanager;
    static pthread_once_t _once;
    vector<TimerTask*> _wheelList;
};

#endif //_TIMERMANAGER_H