2-19:\
今天分析了EventLoop::loop()函数的基本原理;它获取来自Epoll的关于有事件触发的Channl的信息，然后调用事件处理函数.以及有一些pending（即外加的函数）
这些还没有分析.\
然后分析了多线程在哪里创建的，之后继续分析多线程，比如注册的函数是哪一个，它们都干了什么等.\
:smile: 明天继续加油

2-26
关于webserver的多线程：注意是server需要多线程来服务，所以任务分配应该放在server::readhandler()里面;EventLoop本身并不关心多线程。服务自己选择要不要多线程

---

分成几个部分去阅读
1. 线程部分,包括线程池
2. 日志部分
3. httpdata部分
4. 服务部分server
5. loop部分

---

Main.cpp分析：\
主要部分：
```c++
EventLoop mainloop;
Server myHTTPServer(&mainloop , threadNum , port);
myHTTPServer.start();
mainloop.loop();
`````

1. EventLoop mainloop;就创建了一个空的eventloop而已
```mermaid
classDiagram
	class EventLoop
`````
2. Server myHTTPServer(&mainloop , threadNum , port);创建了一个server，和一个EventLoopThreadPool,而且线程没真正创建起来。

3. myHTTPServer.start();这个时候就创建起线程了。


thread->eventloopthread\

threadpool->eventloopthreadpool\

channel->httpdata\

server\
