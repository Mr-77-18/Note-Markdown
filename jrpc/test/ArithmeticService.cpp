#include "ArithmeticServiceStub.h"

using namespace jrpc;

class ArithmeticService: public ArithmeticServiceStub<ArithmeticService>
{
  public:
    explicit
      ArithmeticService(RpcServer& server):
        ArithmeticServiceStub(server)
  {}
    void Add(double lhs , double rhs , const UserDoneCallback& cb)
    {
      cb(json::Value(lhs + rhs));
    }
    void Sub(double lhs , double rhs , const UserDoneCallback& cb)
    {
      cb(json::Value(lhs + rhs));
    }
};

int main(int argc, char *argv[])
{
  EventLoop loop;
  InetAddress addr(9877);
  RpcServer rpcServer(&loop , addr);

  ArithmeticService server(rpcServer);

  rpcServer.start();

  loop.loop();
}
