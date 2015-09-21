#pragma once

/*
 *	真传输层服务端
 */
#ifndef RealTransferServer_h__
#define RealTransferServer_h__

#include "Transfer.h"
#include "ProtocolDefine.h"

class RealTransferServer : public TransferServerProxy
{
public:
	RealTransferServer();
	virtual ~RealTransferServer();

	bool Init( const char* ip = 0, int32 port = GAME_NET_PORT );

	virtual	bool Send(uint32 SessionId, uint32 MsgId, void* pData);
	virtual void SetRecvFunc(SrvRecvFunc pRecvFunc, void* pUserData);

    virtual void DeleteSession(uint32 sessionId);
    virtual bool HasSession(uint32 sessionId) const;

	virtual	void Update();

	void	OnRecv(uint32 SessionId, uint32 MsgId, void* pData);

private:

	SrvRecvFunc				m_pRecvFunc;
	void*					m_pUserDate;
};


#endif // RealTransferServer_h__
