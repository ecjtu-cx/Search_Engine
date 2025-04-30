#ifndef _MYTASK_H_
#define _MYTASK_H_
#include "../myhead.h"
#include "../TcpServer/TcpServer.h"

class Mytask
{
public:
	Mytask(const std::string &msg, const TcpConnectionPtr & conn)
	: _msg(msg)
	, _conn(conn)
	{}

	//process方法运行在线程池中的某一个子线程中(计算线程)
	void process()
	{	
		cout << "Mytask::process is running " << endl;
		//做业务逻辑的处理
		//decode
		//compute
		//encode
		//假设经过了业务逻辑的处理之后，得到了要返回给
		//对端的结果response，那接下来的操作，就是发送了
		//发送操作一定由一个连接进行的,因此需要一个连接
		std::string response = _msg;
		//问题是:send操作能在这进行吗？
		//答案：不行。因为发送操作只能由IO线程(EventLoop的loop方法)进行
		//此时需要一个通知操作，由计算线程通知IO线程发送数据
		//
		//问题是: 计算线程怎么通知IO线程呢？
		//答案：1. 采用eventfd来操作
		//	    2. 将send函数的执行延迟到IO线程,
		//	       把send函数打包成一个回调函数，
		//	       注册给EventLoop对象,并通知IO
		//	       线程
		//_conn->send(response);
		_conn->sendInLoop(response);
		//cout << "msg:" << _msg << endl;
	}

private:
	std::string  _msg;
	TcpConnectionPtr _conn;
};

#endif