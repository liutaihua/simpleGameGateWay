#include "TransferPCH.h"
#include "RealTransferServer.h"
#include "SessionManager.h"


RealTransferServer::RealTransferServer() :
m_pRecvFunc(0), 
    m_pUserDate(0)
{
    SessionManager::Init();
    SessionManager::GetInstance()->SetServerProxy(this);
}

RealTransferServer::~RealTransferServer()
{
    SessionManager::Release();
}

void RealTransferServer::SetRecvFunc( SrvRecvFunc pRecvFunc, void* pUserData )
{
    m_pRecvFunc = pRecvFunc;
    m_pUserDate = pUserData;
}

bool RealTransferServer::Init( const char* ip, int32 port )
{
    return SessionManager::GetInstance()->StartListen(ip, port);	
}

bool RealTransferServer::Send( uint32 SessionId, uint32 MsgId, void* pData )
{
    BT_PROFILE_FUNC;
    return SessionManager::GetInstance()->Send(SessionId, MsgId, pData);
}

void RealTransferServer::Update()
{
    BT_PROFILE_FUNC;
    SessionManager::GetInstance()->Update();
}

void RealTransferServer::OnRecv( uint32 SessionId, uint32 MsgId, void* pData )
{
    BT_PROFILE_FUNC;
    if(m_pRecvFunc)
        m_pRecvFunc(SessionId, MsgId, pData, m_pUserDate);
    else
        CnVerify(false);
}

void RealTransferServer::DeleteSession(uint32 sessionId)
{
    SessionManager::GetInstance()->ReleaseSession(sessionId);
}

bool RealTransferServer::HasSession(uint32 sessionId) const
{
    return SessionManager::GetInstance()->HasSession(sessionId);
}
