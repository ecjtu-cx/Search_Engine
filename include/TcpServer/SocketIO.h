#ifndef _SOCKETIO_H_
#define _SOCKETIO_H_
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

class SocketIO
{
public:
    SocketIO(int);
    int recvn(char *, int);
    int readLine(char *, int);
    int sendn(const char *, int);
    int readjson(char *, int);
private:
    int _fd;
};

#endif