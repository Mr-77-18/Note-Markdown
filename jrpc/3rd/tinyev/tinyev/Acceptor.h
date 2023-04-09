//
// Created by frank on 17-9-1.
//

#ifndef TINYEV_ACCEPTOR_H
#define TINYEV_ACCEPTOR_H

#include <memory>

#include <tinyev/noncopyable.h>
#include <tinyev/InetAddress.h>
#include <tinyev/Channel.h>

namespace ev
{

class EventLoop;

class Acceptor:noncopyable
{
public:
    Acceptor(EventLoop* loop, const InetAddress& local);
    ~Acceptor();

    bool listening() const
    { return listening_; }

    void listen();

    void setNewConnectionCallback(const NewConnectionCallback& cb)//这是一个函数形参
    { newConnectionCallback_ = cb; }

private:
    void handleRead();

    bool listening_;
    EventLoop* loop_;
    const int acceptFd_;
    Channel acceptChannel_;
    InetAddress local_;
    NewConnectionCallback newConnectionCallback_;//这个在tcpserversingle中被设置
};

}



#endif //TINYEV_ACCEPTOR_H
