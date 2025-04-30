#ifndef _SEARCHENGINESERVER_H
#define _SEARCHENGINESERVER_H
#include "Configuration.h"
#include "../TcpServer/TcpServer.h"
#include "../ThreadPool/ThreadPool.h"
#include "../KeyRecommander/KeyRecommander.h"
#include "../WebPageSearcher/WebPageSearcher.h"
#include "../ProtocolParser.h"
class SearchEngineServer {
public: 
    SearchEngineServer(map<string, string> & cofig);

    void start();
    
    void stop();
    
    void onConnection(const TcpConnectionPtr & conn);
    
    void onMessage(const TcpConnectionPtr & conn);
    
    void onClose(const TcpConnectionPtr & conn);
    
    void doTaskThread(const TcpConnectionPtr&, string &msg);
private: 
    ThreadPool _threadpool;
    TcpServer _tcpserver;
};

#endif //_SEARCHENGINESERVER_H