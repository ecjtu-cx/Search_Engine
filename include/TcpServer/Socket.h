#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

class Socket
{
public:
    Socket();
    explicit Socket(int fd);
    int fd() const;
    void shutdownWrite();
    ~Socket();

private:
    int _sockfd;
};

#endif