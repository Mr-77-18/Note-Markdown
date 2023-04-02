//
// Created by frank on 17-9-1.
//

#ifndef TINYEV_TCPSERVERTHREAD_H
#define TINYEV_TCPSERVERTHREAD_H

#include <unordered_set>

#include <tinyev/Callbacks.h>
#include <tinyev/Acceptor.h>

namespace ev
{

class EventLoop;
class TcpServerSingle : noncopyable//给我感觉是：这个TcpServerSingle有点像WebServer里面的Server，当连接请求到来的时候，创建了Httpdata这个具体的服务
{
public:
    TcpServerSingle(EventLoop *loop, const InetAddress &local);

    void setConnectionCallback(const ConnectionCallback &cb)
    { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb)
    { messageCallback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback &cb)
    { writeCompleteCallback_ = cb; }

    void start();

private:
    void newConnection(int connfd, const InetAddress &local, const InetAddress &peer);

    void closeConnection(const TcpConnectionPtr &conn);

    typedef std::unordered_set<TcpConnectionPtr> ConnectionSet;

    EventLoop *loop_;
    Acceptor acceptor_;//它含有Channel
    ConnectionSet connections_;//得到的连接,基本类型是TcpConnection
    ConnectionCallback connectionCallback_;//接受连接应该做的事情
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
};

}

#endif //TINYEV_TCPSERVERTHREAD_H
