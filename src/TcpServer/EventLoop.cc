#include "../../include/TcpServer/EventLoop.h"
#include <iostream>

EventLoop::EventLoop(Acceptor & acceptor)
: _epfd(createEpollFd())
, _evtfd(createEventfd())
, _acceptor(acceptor)
, _evtList(1024)
, _isLooping(false)
{
    addEpollReadFd(_acceptor.fd());
	addEpollReadFd(_evtfd);
}

EventLoop::~EventLoop()
{
    close(_epfd);
}

void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping) {
        waitEpollFd();
    }
}

void EventLoop::runInLoop(Functor && cb)
{
    {
        MutexLockGuard guard(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::setAllCallback(TcpConnectionCallback &&cb1,
    TcpConnectionCallback &&cb2, TcpConnectionCallback &&cb3)
{
    _onConnection = std::move(cb1);
    _onMessage = std::move(cb2);
    _onClose = std::move(cb3);
}

void EventLoop::waitEpollFd()
{
    int nready = 0;
    do {
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 500000);
    } while(-1 == nready && errno == EINTR);
    if(-1 == nready)
    {
        perror("epoll_wait");
        return;
    } else if (nready == 0){
        puts("epoll_wait timeout.");
    } else {
        if(nready ==(int)_evtList.size()) {
            _evtList.resize(2 * nready);
        }
        for(int idx = 0; idx < nready; idx++)
        {
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd())
            {
                if(_evtList[idx].events & EPOLLIN)
                    handleNewConnection();
            } else if (fd == _evtfd)
            {
                handleRead(); //把eventfd的计数器缓存区清空
                #ifndef DEBUG
                std::cout << "doPendingFunctors" << std::endl;
                #endif
                doPendingFunctors();
            } else {
                if(_evtList[idx].events & EPOLLIN)
                    handleMessage(fd);
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);
    TcpConnectionPtr conn(new TcpConnection(peerfd, this));
    conn->setAllCallback(_onConnection, _onMessage, _onClose);

    _conns.insert(std::make_pair(peerfd, conn));

    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    //查找TcpConnection对象
	//fd --> TcpConnection;
	auto iter = _conns.find(fd);
	if(iter != _conns.end()) {
		//0.先判断该连接是否断开
		auto isClosed = iter->second->isClosed();
		if(!isClosed) {
			//1. 正常消息的处理
			iter->second->handleMessageCallback();
		} else {
		//2. 连接断开
			iter->second->handleCloseCallback();
            delEpollReadFd(fd);
			_conns.erase(iter);
		}
	}
}

int EventLoop::createEpollFd()
{
    int epfd = epoll_create(1);
    if(epfd < 0)
    {
        perror("epoll_create");
    }
    return epfd;
}

int EventLoop::createEventfd()
{
    int evfd = eventfd(0,0);
    if(evfd < 0)
    {
        perror("eventfd");
    }
    return evfd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event value;
    value.events = EPOLLIN;
    value.data.fd = fd;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &value);
    if (ret < 0) {
        perror("epoll_ctl");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event value;
    value.events = EPOLLIN;
    value.data.fd = fd;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &value);
    if(ret < 0) {
        perror("epoll_ctl");
        return;
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = write(_evtfd, &one, sizeof(one));
    if(ret != sizeof(uint64_t))
    {
        perror("write");
    }
}

void EventLoop::handleRead()
{
    uint64_t howmany = 0;
    int ret = read(_evtfd, &howmany, sizeof(howmany));
    if(ret != sizeof(uint64_t))
    {
        perror("read");
    }
}

void EventLoop::doPendingFunctors()
{
    vector<Functor> tmp;
    {
        MutexLockGuard guard(_mutex);
        _pendingFunctors.swap(tmp);
    }
    for(auto & f: tmp)
    {
        f();
    }
}