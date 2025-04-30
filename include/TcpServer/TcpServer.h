#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_
#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer
{
public:
    TcpServer(const string & ip, unsigned short port);
    void setAllCallbacks(TcpConnectionCallback &&,
                    TcpConnectionCallback &&,
                    TcpConnectionCallback &&);
    void start();
    void stop();
    EventLoop * getloop();
private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif