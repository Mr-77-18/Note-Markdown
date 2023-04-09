//
// Created by frank on 17-9-1.
//

#include <tinyev/Logger.h>
#include <tinyev/TcpConnection.h>
#include <tinyev/TcpServerSingle.h>
#include <tinyev/EventLoop.h>
#include <tinyev/TcpServer.h>

using namespace ev;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& local)
        : baseLoop_(loop),
          numThreads_(1),//线程数量
          started_(false),
          local_(local),
          threadInitCallback_(defaultThreadInitCallback),
          connectionCallback_(defaultConnectionCallback),//创建BaseServer的时候设置了
          messageCallback_(defaultMessageCallback)//创建BaseServer的时候设置了
{
    INFO("create TcpServer() %s", local.toIpPort().c_str());
}

TcpServer::~TcpServer()
{
    for (auto& loop: eventLoops_)
        if (loop != nullptr)
            loop->quit();
    for (auto& thread: threads_)
        thread->join();
    TRACE("~TcpServer()");
}

void TcpServer::setNumThread(size_t n)
{
    baseLoop_->assertInLoopThread();
    assert(n > 0);
    assert(!started_);
    numThreads_ = n;
    eventLoops_.resize(n);
}

void TcpServer::start()
{
    if (started_.exchange(true))
        return;

    baseLoop_->runInLoop([=](){startInLoop();});//???
}

//rpcServer.start()的时候调用,此时rpcServer的service已经有一个service了
void TcpServer::startInLoop()
{
    INFO("TcpServer::start() %s with %lu eventLoop thread(s)",
         local_.toIpPort().c_str(), numThreads_);

    baseServer_ = std::make_unique<TcpServerSingle>(baseLoop_, local_);//创建一个TcpServerSingle

    //这里我一直在想为什么回调参数会有一个对象呢（我以为这个回调函数是被eventloop中调用的，其实不是，这个回调函数是在TcpServerSingle中的newConnection中被调用的，这个newConnection才是在Eventloop中被回调的函数（猜测）
    baseServer_->setConnectionCallback(connectionCallback_);//这个被设置成了BaseServer里面的connectionCallback
    baseServer_->setMessageCallback(messageCallback_);//这个被设置成了BaseServer里面的messageCallback
    baseServer_->setWriteCompleteCallback(writeCompleteCallback_);

    threadInitCallback_(0);
    baseServer_->start();//跟进去发现在里面调用了Channel::update()将channel加入到了loop里面

    //创建线程
    for (size_t i = 1; i < numThreads_; ++i) {
        auto thread = new std::thread(std::bind(
                &TcpServer::runInThread, this, i));
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while (eventLoops_[i] == nullptr)
                cond_.wait(lock);
        }
        threads_.emplace_back(thread);
    }


}

void TcpServer::runInThread(size_t index)
{
    EventLoop loop;
    TcpServerSingle server(&loop, local_);

    server.setConnectionCallback(connectionCallback_);
    server.setMessageCallback(messageCallback_);
    server.setWriteCompleteCallback(writeCompleteCallback_);

    {
        std::lock_guard<std::mutex> guard(mutex_);
        eventLoops_[index] = &loop;
        cond_.notify_one();
    }

    threadInitCallback_(index);
    server.start();
    loop.loop();
    eventLoops_[index] = nullptr;
}
