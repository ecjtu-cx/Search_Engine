#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_
#include "Socket.h"
#include "InetAddress.h"

class Acceptor
{
private:
    Socket _sock;
    InetAddress _servAddr;
public:
    Acceptor(const string &, unsigned short);
    void ready();
    int accept();
    int fd()
    {
        return _sock.fd();
    }
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
};

#endif