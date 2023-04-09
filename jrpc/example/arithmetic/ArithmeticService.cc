//

#include <example/arithmetic/ArithmeticServiceStub.h>

using namespace jrpc;

class ArithmeticService: public ArithmeticServiceStub<ArithmeticService>//采用public继承：ArithmeticService是一个ArithmeticServiceStub<ArithmeticService>
{
public:
    explicit
    ArithmeticService(RpcServer& server)://基本的应该是rpcserver,先去看看
            ArithmeticServiceStub(server),
            pool_(4)
    {}

    //调用add
    void Add(double lhs, double rhs, const UserDoneCallback& cb)//userDoneCallback表示的是服务完成后应该执行的操作,这个操作对给线程池去做
    {
        pool_.runTask([=](){
            cb(json::Value(lhs + rhs));
        });
    }

    void Sub(double lhs, double rhs, const UserDoneCallback& cb)
    {
        pool_.runTask([=](){
            cb(json::Value(lhs - rhs));
        });
    }

    void Mul(double lhs, double rhs, const UserDoneCallback& cb)
    {
        pool_.runTask([=](){
            cb(json::Value(lhs * rhs));
        });
    }

    void Div(double lhs, double rhs, const UserDoneCallback& cb)
    {
        pool_.runTask([=](){
            cb(json::Value(lhs / rhs));
        });
    }

private:

    ThreadPool pool_;
};


//server翻译为服务器，service翻译成服务，一个服务器可以包含多个服务,所以一个server可以有多个service
int main()
{
    //在本线程创建一个处理事件的框架
    EventLoop loop;
    InetAddress addr(9877);

    //rpcserver->baseserver->tcpserver
    //RpcServer的初始化函数上面也没干，只是调用了BaseServer的构造函数,而且RpcServer只有一个服务列表成员变量
    //BaseServer的构造函数调用了TcpServer的构造函数,并且设置了TcpServer中的Connectioncalllback()以及messagecallback(),设置成BaseServer的onConnection(),以及onMessage();
    //创建了一个rpcServer服务器，但是还不是具体的服务器
    RpcServer rpcServer(&loop, addr);

    ArithmeticService service(rpcServer);//创建一个service,每一个rpcServer可以有多个service

    rpcServer.start();
    loop.loop();
}
