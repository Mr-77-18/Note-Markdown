//
// Created by frank on 17-9-1.
//

#include <unistd.h>
#include <cassert>

#include <tinyev/EventLoop.h>
#include <tinyev/Logger.h>
#include <tinyev/InetAddress.h>
#include <tinyev/Acceptor.h>

using namespace ev;

namespace
{

int createSocket()
{
    int ret = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (ret == -1)
        SYSFATAL("Acceptor::socket()");
    return ret;
}

}

Acceptor::Acceptor(EventLoop* loop, const InetAddress& local)
        : listening_(false),
          loop_(loop),
          acceptFd_(createSocket()),
          acceptChannel_(loop, acceptFd_),
          local_(local)
{
    int on = 1;
    int ret = ::setsockopt(acceptFd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (ret == -1)
        SYSFATAL("Acceptor::setsockopt() SO_REUSEADDR");
    ret = ::setsockopt(acceptFd_, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if (ret == -1)
        SYSFATAL("Acceptor::setsockopt() SO_REUSEPORT");
    ret = ::bind(acceptFd_, local.getSockaddr(), local.getSocklen());
    if (ret == -1)
        SYSFATAL("Acceptor::bind()");
}

//rpcserver.start()->tcpserver.start()->tcpserver::startinloop()-->tcpserversingle.start()->acceptor.listen()
void Acceptor::listen()
{
    loop_->assertInLoopThread();

    int ret = ::listen(acceptFd_, SOMAXCONN);//监听有没有连接到来

    if (ret == -1)
        SYSFATAL("Acceptor::listen()");

    acceptChannel_.setReadCallback([this](){handleRead();});
    acceptChannel_.enableRead();//里面调用了Channel::update()，将Channel加入到了loop里面,acceptChannel中保存的就是acceptFd_
}


Acceptor::~Acceptor()
{
    ::close(acceptFd_);
}

//这个是接受请求的函数
void Acceptor::handleRead()
{
    loop_->assertInLoopThread();

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    void* any = &addr;
    int sockfd = ::accept4(acceptFd_, static_cast<sockaddr*>(any),
                           &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (sockfd == -1) {
        int savedErrno = errno;
        SYSERR("Acceptor::accept4()");
        switch (savedErrno) {
            case ECONNABORTED:
            case EMFILE:
                break;
            default:
                FATAL("unexpected accept4() error");
        }
    }

    if (newConnectionCallback_) {
        InetAddress peer;
        peer.setAddress(addr);
        //路径：（从eventloop调用的函数开始,Channel::handleEvent()->Acceptor::handleRead()->TcpServerSingle::newConnection()->BaseServer()::onconnection()）
        newConnectionCallback_(sockfd, local_, peer);//这个调用的是TcpServerSingle.newConnection()；
    }
    else ::close(sockfd);
}
