//
// Created by frank on 17-12-30.
//

#include <jrpc/Exception.h>
#include <jrpc/server/RpcService.h>

using namespace jrpc;

//TcpConnection::handleRead()->BaseServer::onMessage()->BaseServer::handleMessage()->RpcServer::handleRequest()->RPcServer::handleSingleRequest()->RpcService::callProcedureReturn()
void RpcService::callProcedureReturn(std::string_view methodName,
                                     json::Value& request,
                                     const RpcDoneCallback& done)
{
    auto it = procedureReturn_.find(methodName);//找到方法，procedure是一个类模板，需要一个函数作为模板参数,后面要调用it
    if (it == procedureReturn_.end()) {
        throw RequestException(RPC_METHOD_NOT_FOUND,
                               request["id"],
                               "method not found");
    }
    it->second->invoke(request, done);
};

void RpcService::callProcedureNotify(std::string_view methodName, json::Value& request)
{
    auto it = procedureNotfiy_.find(methodName);
    if (it == procedureNotfiy_.end()) {
        throw NotifyException(RPC_METHOD_NOT_FOUND,
                              "method not found");
    }
    it->second->invoke(request);
};
