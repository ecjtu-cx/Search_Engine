#ifndef _ECHOSERVER_H_
#define _ECHOSERVER_H_
#include "../bo_threadpool/ThreadPool.h"
#include "TcpServer.h"

class EchoServer
{
public:
    EchoServer(unsigned short, const string &);
    void start();
    void onConnection(const TcpConnectionPtr &);
    void onMessage(const TcpConnectionPtr &);
    void onClose(const TcpConnectionPtr &);
private:
    ThreadPool _threadpool;
    TcpServer _server;
};

#endif