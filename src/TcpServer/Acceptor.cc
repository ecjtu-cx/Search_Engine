#include "../../include/TcpServer/Acceptor.h"

Acceptor::Acceptor(const string & ip, unsigned short port)
: _sock()
, _servAddr(ip, port)
{}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr()
{
    int reuse = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    if(ret < 0)
    {
        perror("setsockopt");
        return;
    }
}

void Acceptor::setReusePort()
{
    int reuse = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
    if(ret < 0)
    {
        perror("setsockopt");
        return;
    }
}

void Acceptor::bind()
{
    int ret = ::bind(_sock.fd(), (struct sockaddr *)_servAddr.getInetAddressPtr(), 
    sizeof(struct sockaddr_in));
    if(ret < 0)
    {
        perror("bind");
        return;
    }
}

void Acceptor::listen()
{
    int ret = ::listen(_sock.fd(), 1024);
    if(ret < 0)
    {
        perror("listen");
        return;
    }
}

int Acceptor::accept()
{
    int peerfd = ::accept(_sock.fd(), nullptr, nullptr);
    if(peerfd < 0)
    {
        perror("accept");
    }
    return peerfd;
}