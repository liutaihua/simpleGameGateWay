#pragma once

#ifndef Transfer_h__
#define Transfer_h__

#include "GameDefine.h"
#include "sdu.h"

// 客户端代理的Recv函数
typedef void (*CliRecvFunc)(uint32 MsgId, void* pData, void* pUserData);

class TransferClientProxy
{
public:
	TransferClientProxy() {}
	virtual ~TransferClientProxy() {}

	virtual	bool Send(uint32 MsgId, void* pData) = 0;
	virtual void SetRecvFunc(CliRecvFunc, void* ) = 0;

	virtual	void Update() = 0;
};

// 服务端代理的Recv函数
typedef void (*SrvRecvFunc)(uint32 SessionId, uint32 MsgId, void* pData, void* pUserData);

class TransferServerProxy
{
public:
	TransferServerProxy() {}
	virtual ~TransferServerProxy() {}

	virtual	bool Send(uint32 SessionId, uint32 MsgId, void* pData) = 0;
	virtual void SetRecvFunc(SrvRecvFunc, void* ) = 0;
    virtual void DeleteSession(uint32 sessionId) = 0;
    virtual bool HasSession(uint32 sessionId) const = 0;
	virtual	void Update() = 0;
};

TransferClientProxy*	CreateTransferClientProxy(bool IsReal, const char* ip, int32 port = 0);
TransferServerProxy*	CreateTransferServerProxy(bool IsReal, int32 port);

// record protocol data into a file, empty string denotes not recording.
extern std::string g_ProtoRecord;

#endif // Transfer_h__
