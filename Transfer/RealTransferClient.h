/*
 *	真实传输层客户端
 */

#ifndef RealTransferClient_h__
#define RealTransferClient_h__

#ifdef WIN32

#include "Transfer.h"

class TransferSocket;

class RealTransferClient : public TransferClientProxy
{
	friend class TransferSocket;
public:
	RealTransferClient();
	virtual ~RealTransferClient();

	bool	Init(const char* ip, int32 port);

	virtual	bool Send(uint32 MsgId, void* pData);
	virtual void SetRecvFunc(CliRecvFunc pRecvFunc, void* pUserData);

	virtual	void Update();
private:
	void	OnReceive(uint32 MsgId, void* pData);

	std::string				m_pConnectIpStr;
    int32                   m_Port;

	TransferSocket*		m_pSdSock;			//  收发网络包的对象

	CliRecvFunc				m_pRecvFunc;
	void*					m_pUserDate;

};

#endif // WIN32

#endif // RealTransferClient_h__
