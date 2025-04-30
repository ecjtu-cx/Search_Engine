#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <functional>
#include <memory>
#include <string>
#include <sstream>


using std::string;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const 
TcpConnectionPtr &)>;
class EventLoop;
class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int, EventLoop *);
    string receive();
    void send(const string &);
    void sendInLoop(const string &);
    void shutdown();
    string toString() const;
    void setAllCallback(const TcpConnectionCallback &, 
    const TcpConnectionCallback &, const TcpConnectionCallback &);
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
    bool isClosed() const;
 private:
    InetAddress getLocalAddress();
    InetAddress getPeerAddress();
private:
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddress;
    InetAddress _peerAddress;
    bool _isShutdownWrite;
    EventLoop * _loop;
    TcpConnectionCallback _onConnection; 
    TcpConnectionCallback _onMessage; 
    TcpConnectionCallback _onClose;
};

#endif