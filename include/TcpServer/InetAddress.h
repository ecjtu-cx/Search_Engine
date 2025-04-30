#ifndef _INETADDRESS_H_
#define _INETADDRESS_H_
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using std::string;

class InetAddress
{
private:
    struct sockaddr_in _addr;
public:
    InetAddress(const string &ip, unsigned short port)
    {
        memset(&_addr, 0, sizeof(_addr));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = inet_addr(ip.c_str());
        _addr.sin_port = htons(port);
    }
    InetAddress(const struct sockaddr_in &addr)
    : _addr(addr)
    {}
    string ip() const
    {
        return string(inet_ntoa(_addr.sin_addr));
    }
    unsigned short port() const
    {
        return ntohs(_addr.sin_port);
    }
    const struct sockaddr_in * getInetAddressPtr()
    {
        return &_addr;
    }
};

#endif