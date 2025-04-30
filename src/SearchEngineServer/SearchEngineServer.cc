#include "../../include/SearchEngineServer/SearchEngineServer.h"
#include "../../include/SearchEngineServer/Mytask.h"
#include "../../include/Cache/CacheManager.h"


SearchEngineServer::SearchEngineServer(map<string, string> & config)
: _threadpool(atoi((config["threads_num"]).c_str()), atoi((config["server_port"]).c_str()))
, _tcpserver(config["server_ip"], atoi((config["server_port"]).c_str()))
{}

void SearchEngineServer::start() {
    CacheManager::getCachemanager();
    _threadpool.start();

    using namespace std::placeholders;
    _tcpserver.setAllCallbacks(
        std::bind(&SearchEngineServer::onConnection, this, _1),
        std::bind(&SearchEngineServer::onMessage, this, _1),
        std::bind(&SearchEngineServer::onClose, this, _1));

    _tcpserver.start();
}

void SearchEngineServer::stop() {
    return;
}

void SearchEngineServer::onConnection(const TcpConnectionPtr &conn)
{
    cout << conn->toString() << " has connected!" << endl;
}

// onMessage的实现时间，必须要控制ms级别
void SearchEngineServer::onMessage(const TcpConnectionPtr &conn)
{
    cout << "recving" << endl;
    //进行业务逻辑的处理
    std::string msg = conn->receive();
    msg.pop_back();
    //将msg交给计算线程
    _threadpool.addTask(std::bind(&SearchEngineServer::doTaskThread, this, conn, msg));

    //可以根据实际情况，进行修改
    //关键字 ==》 关键字推荐的处理
    //       ==》 网页查询
}

void SearchEngineServer::onClose(const TcpConnectionPtr &conn)
{
    cout << conn->toString() << " has closed!" << endl;
}

void SearchEngineServer::doTaskThread(const TcpConnectionPtr& conn, string & msg)
{
    cout << "doTaskThread is running " << endl;
    
    json recvj = ProtocolParser::doParse(msg);
    json sendj;
    vector<string> vec;
    switch ((int)(recvj["cmd"]))
    {
        case KeyRecommand:
        {
            KeyRecommander _keyrecommander(recvj["word"]);
            sendj = _keyrecommander.get_serialize();
            break;
        }
        case WebPageSearch:
        {
            WebPageSearcher _webpagesearcher;
            _webpagesearcher.doQuery(recvj["word"], sendj);
            break;
        }
    }
    cout << sendj << endl;
    string response = ProtocolParser::Json2string(sendj);
    conn->sendInLoop(response);
}