#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_
#include "InetAddress.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "../ThreadPool/MutexLock.h"
#include "../myhead.h"
#include <sys/epoll.h>
#include <errno.h>
#include <map>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::map;
using std::vector;

using Functor = std::function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor &);
    ~EventLoop();
    void loop();
    void runInLoop(Functor && cb);
    void unloop();
    void setAllCallback(TcpConnectionCallback &&cb1, 
                        TcpConnectionCallback &&cb2,
                        TcpConnectionCallback &&cb3);
private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int);
    int createEpollFd();
    void addEpollReadFd(int);
    void delEpollReadFd(int);
    int createEventfd();
    void wakeup();
    void handleRead();
    void doPendingFunctors();
private:
    int _epfd;
    int _evtfd;
    Acceptor & _acceptor;
    map<int, shared_ptr<TcpConnection>> _conns;
    vector<struct epoll_event> _evtList;
    bool _isLooping;
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
    vector<Functor> _pendingFunctors;
    MutexLock _mutex; 
};

#endif