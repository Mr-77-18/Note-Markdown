//
// Created by frank on 17-12-30.
//

#ifndef JRPC_RPCSERVICE_H
#define JRPC_RPCSERVICE_H

#include <jackson/Value.h>

#include <jrpc/server/Procedure.h>

namespace jrpc
{


class RpcService: noncopyable//函数似乎保存在procedure当中
{
public:
    void addProcedureReturn(std::string_view methodName, ProcedureReturn* p)
    {
        assert(procedureReturn_.find(methodName) == procedureReturn_.end());
        procedureReturn_.emplace(methodName, p);
    }

    void addProcedureNotify(std::string_view methodName, ProcedureNotify *p)
    {
        assert(procedureNotfiy_.find(methodName) == procedureNotfiy_.end());
        procedureNotfiy_.emplace(methodName, p);
    }

    void callProcedureReturn(std::string_view methodName,
                             json::Value& request,
                             const RpcDoneCallback& done);
    void callProcedureNotify(std::string_view methodName, json::Value& request);

private:
    typedef std::unique_ptr<ProcedureReturn> ProcedureReturnPtr;
    typedef std::unique_ptr<ProcedureNotify> ProcedureNotifyPtr;
    typedef std::unordered_map<std::string_view, ProcedureReturnPtr> ProcedureReturnList;//std::string_view是字符串的一个视图，并不含有字符串本身,比std::string的效率要高
    typedef std::unordered_map<std::string_view, ProcedureNotifyPtr> ProcedureNotifyList;

    ProcedureReturnList procedureReturn_;
    ProcedureNotifyList procedureNotfiy_;
};

}


#endif //JRPC_RPCSERVICE_H
