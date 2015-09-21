#pragma once

/*
 *	服务端的一个连接，继承使用
 */
#ifndef Session_h__
#define Session_h__

class RealTransferServer;

class Session : public Crown::ISDSession
{
public:
	Session(void);
	virtual ~Session(void);

	virtual void SDAPI SetConnection(Crown::ISDConnection* poConnection);
	virtual void SDAPI OnEstablish(void);
	virtual void SDAPI OnTerminate(void);
	virtual bool SDAPI OnError(int32 nModuleErr, int32 nSysErr);
	virtual void SDAPI OnRecv(const char* pBuf, uint32 dwLen);
	virtual void SDAPI Release(void);

	bool Send(uint32 MsgId, void* pData);

	Crown::ISDConnection*	GetConnection() {return m_poConnection;}

	void	SetId(uint32 id);
	uint32	GetId() {return m_Id;}

	RealTransferServer*	m_pServerProxy;

private:
	Crown::ISDConnection*	m_poConnection;
	uint32					m_Id;				// 该Session的唯一标识id号，等于在SessionManager中的数组下标
	
};

#endif // Session_h__
