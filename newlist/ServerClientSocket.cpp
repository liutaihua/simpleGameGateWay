#include <stdexcept>
#include <iostream>
#include <fstream>

#include "ServerClientSocket.h"
ClientSocketManager* g_SocketsManager;

SClientSocket::SClientSocket(SOCKET fd,const sockaddr_in * peer) :
  TcpSocket(fd, 4096*10, 4096*50,true,peer),
  m_pProxySocket(0)
{
  m_LastPingTim = time(NULL);
  IsPyTerm=false;
  m_remaining = 0;
  m_Cmd = 0;
  IsGet=false;
  IsLogin=false;
}


SClientSocket::~SClientSocket()
{
   g_SocketsManager->RemoveServerSocket(this);
}

ProxySClientSocket::ProxySClientSocket(SOCKET fd, const sockaddr_in *peer):
    SClientSocket(fd, peer),
    m_pPrimeSocket(0)
{
}

ProxySClientSocket::~ProxySClientSocket()
{
    g_SocketsManager->RemoveServerSocket(this);
}

void ProxySClientSocket::SetPrimeSocket(SClientSocket* p)
{
    m_pPrimeSocket = p;
    DBG("SetPrimeSocket to %s", p);
}

void ProxySClientSocket::OnConnect()
{
    g_SocketsManager->AddServerSocket(this);
    //std::cout<<"SClientSocket OnConnect "<<std::to_string(this->GetHandle())<<std::to_string(GetFd())<<"\n";
}

void ProxySClientSocket::OnDisconnect()
{
    // TODO
    // 上游socket被动断开， 应当传导至下游客户端连接， 一起断开
    // 不是被上游socket断开时， 是普通的prime socket更换proxy socket， 这里不应当传导至下游连接关闭
    g_SocketsManager->RemoveServerSocket(this);
    if (m_pPrimeSocket && m_EngineErrCode != 0)
        m_pPrimeSocket->DisconnectData();
}

void SClientSocket::OnDisconnect()
{
    g_SocketsManager->RemoveServerSocket(this);
    if (m_pProxySocket)
        m_pProxySocket->DisconnectData();
}

void SClientSocket::OnConnect()
{
    g_SocketsManager->AddServerSocket(this);
}


void SClientSocket::DisconnectData()
{
   DBG("SClientSocket::DisconnectData :%d %d",this->GetHandle(),GetFd());
   Disconnect();
}

void SClientSocket::SetProxySocket(ProxySClientSocket *p)
{
    m_pProxySocket = p;
    DBG("set proxy socket to %s", p);
}

void ProxySClientSocket::OnRecvData()
{
    uint32 nub=0;
    char pData[MAX_PACK_LEN];
    while(true)
    {
        ++nub;
        if (nub>200)
        {
            if (IsConnected())
                DisconnectData();
            //GetReadBuffer()->FreeRead();
            DBG("SClientSocket::OnRead erer :%d, nub:%d",this->GetHandle(), nub);
            return;
        }
        if (!m_connected || m_deleted)
            return;
        if (m_remaining==0)
        {
            if(GetReadBuffer()->GetSize() < sizeof(STNetMsgHeader))
                return;
            STNetMsgHeader pstHeader;
            GetReadBuffer()->Read((uint8*)&pstHeader, sizeof(STNetMsgHeader));
            m_Cmd = ntohl(pstHeader.dwMsgID);
            m_remaining = ntohl(pstHeader.dwDataLen);
            if(m_remaining > MAX_PACK_LEN)
            {

                DBG("SClientSocket::OnRead Disconnect 1:%d, msgid:%d, len:%d",this->GetHandle(),
                    m_Cmd, m_remaining);
                if (IsConnected())
                    DisconnectData();
                return;
            }
        }
        if(GetReadBuffer()->GetSize() < m_remaining)
            return;
        if(m_remaining)
        {
            GetReadBuffer()->Read(proxyRecBuff, m_remaining);
            memset(pData,0,sizeof(pData));
            int decodedLen = g_BnFGameProtoEN.Decode(m_Cmd, proxyRecBuff, m_remaining, pData, MAX_PACK_LEN);
            if (decodedLen <= 0)
            {
                //WARN("Protocol error. %d", msgId);
                DBG("SClientSocket::OnRead Disconnect 2:%d",this->GetHandle());
                if (IsConnected())
                    DisconnectData();
                return;
            }

            CnAssert(GetPrimeSocket());
            GetPrimeSocket()->SendPacket((EN_BnFGameProto_MessageID)m_Cmd, pData);
        }
        m_remaining = 0;
        m_Cmd = 0;
    }
}

////网络线程驱动
void SClientSocket::OnRecvData()
{
    uint32 nub=0;
    char pData[MAX_PACK_LEN];
    while(true)
    {
        ++nub;
        if (nub>200)
        {
            if (IsConnected())
                DisconnectData();
            //GetReadBuffer()->FreeRead();
            DBG("SClientSocket::OnRead erer :%d, nub:%d",this->GetHandle(), nub);
            return;
        }
        if (!m_connected || m_deleted)
            return;

        if (m_remaining==0)
        {
            if(GetReadBuffer()->GetSize() < sizeof(STNetMsgHeader))
                return;
            STNetMsgHeader pstHeader;

            GetReadBuffer()->Read((uint8*)&pstHeader, sizeof(STNetMsgHeader));
            m_Cmd = ntohl(pstHeader.dwMsgID);
            m_remaining = ntohl(pstHeader.dwDataLen);

            if(!GetProxySocket() && !IsLogin && m_Cmd != MSG_JUMP_SCENE_REQ) {
                uint32 serverid = m_Cmd;
                uint32 port = m_remaining;
                DBG("serverid: %d, ares port: %d", serverid, port);
                ProxySClientSocket* psocket;
                psocket = CreateProxySocket<ProxySClientSocket>("172.26.64.10", port).release();
                if (!psocket) {
                    DBG("can not connect to %d", port);
                    DisconnectData();
                    return;
                }
                psocket->SetPrimeSocket(this);
                SetProxySocket(psocket);
                DBG("init upstream socket ...%s %s", this, GetProxySocket());
                m_remaining = 0;
                continue;
//                return;
            }


            if(m_remaining > MAX_PACK_LEN)
            {

                DBG("SClientSocket::OnRead Disconnect 1:%d, msgid:%d, len:%d",this->GetHandle(),
                    m_Cmd, m_remaining);
                if (IsConnected())
                    DisconnectData();
                return;
            }
        }
        if(GetReadBuffer()->GetSize() < m_remaining)
            return;
        if(m_remaining)
        {
            GetReadBuffer()->Read(RecBuff, m_remaining);
            memset(pData,0,sizeof(pData));
            int decodedLen = g_BnFGameProtoEN.Decode(m_Cmd, RecBuff, m_remaining, pData, MAX_PACK_LEN);
            if (decodedLen <= 0)
            {
                //WARN("Protocol error. %d", msgId);
                DBG("SClientSocket::OnRead Disconnect 2:%d",this->GetHandle());
                if (IsConnected())
                    DisconnectData();
                return;
            }
            switch(m_Cmd) {
            case MSG_LOGIN_REQ: {
                CLI_GS_LOGIN_REQ* req = static_cast<CLI_GS_LOGIN_REQ*>((void*)pData);

                IsLogin = true;
                m_LoginReq = req;

                char str_buf[1024];
                memset(str_buf, 0, 1024);
                snprintf(str_buf, 1024,
                         "SessionKey=%s&UserId=%s&TableId=%d&major=%d&minor=%d&revision=%d&%s",
                         req->SessionKey, req->UserId,
                         req->TableId, req->major,
                         req->minor, req->revision, MD5_KEY.c_str());
                DBG("user require login %d, %d, %s", req->UserId, req->TableId, req->SessionKey);

                //                pck = new ByteBuffer(sizeof(uint32)*2+asize);
                //                *pck<< (5000 + m_fd);
                //                *pck<< m_Cmd;
                memcpy(m_LoginData, pData, MAX_PACK_LEN);
                g_SocketsManager->SetUserIdSession((uint64)(req->UserId), this);
            }
                break;
//            case MSG_TALK_REQ: {
//                /*
//                 * 把第一次登陆的所保存的req data， 直接发给新场景登陆， 同时把upstream socket更换成新的
//                 */
//                CLI_GS_TALK_REQ* tr = static_cast<CLI_GS_TALK_REQ*>((void*)pData);
//                DBG("MSG_TALK_REQ npcid: %d %d", tr->TargetId, tr->MessageId());

//                GetProxySocket()->Disconnect();

//                ProxySClientSocket* ps;
//                ps = CreateProxySocket<ProxySClientSocket>("172.26.64.10", 15921).release();
//                if (!ps) {
//                    DBG("can not connect");
//                    DisconnectData();
//                    return;
//                }
//                ps->SetPrimeSocket(this);
//                SetProxySocket(ps);
//                GetProxySocket()->SendPacket(MSG_LOGIN_REQ, m_LoginData);
//            }
//                break;
            case MSG_JUMP_SCENE_REQ: {
                CLI_GS_JUMP_SCENE_REQ* rq = static_cast<CLI_GS_JUMP_SCENE_REQ*>((void*)pData);
                DBG("recv the jump scene REQ: %d %s %d %d", MSG_JUMP_SCENE_REQ, rq->Host, rq->Port, rq->UserId);

                ProxySClientSocket* ps;
                ps = CreateProxySocket<ProxySClientSocket>(rq->Host, rq->Port).release();
                if (!ps) {
                    DBG("can not connect");
                    DisconnectData();
                    return;
                }
                DBG("create new proxy socket %s", ps);
                SClientSocket* tmpSC = g_SocketsManager->GetSessionWithUserID((uint64)(rq->UserId));
                if (!tmpSC) {
                    DBG("not found sclientSocket with userid %d", rq->UserId);
                    return;
                }
                if (tmpSC->GetProxySocket())
                    tmpSC->GetProxySocket()->Disconnect();
                ps->SetPrimeSocket(tmpSC);
                tmpSC->SetProxySocket(ps);
                tmpSC->GetProxySocket()->SendPacket(MSG_LOGIN_REQ, tmpSC->m_LoginData);

                return;
            }
            default :{
                //                CLI_GS_LOGIN_REQ* req = static_cast<CLI_GS_LOGIN_REQ*>(m_LoginData);
                //                DBG("ffffff %s", req);
            }
            }

            CnAssert(GetProxySocket());
            GetProxySocket()->SendPacket((EN_BnFGameProto_MessageID)m_Cmd, pData);
        }
        m_remaining = 0;
        m_Cmd = 0;
    }

}
//游戏主线程驱动
void SClientSocket::SendPacket(EN_BnFGameProto_MessageID msgId, void *pData)
{
    if (!m_connected || m_deleted){
        DBG("already dis connect when SendPacket %d %d %d %s", m_connected, m_deleted, m_fd, this);
        Disconnect();
        if (GetProxySocket())
            GetProxySocket()->Disconnect();
        ProxySClientSocket* tmpSC = dynamic_cast<ProxySClientSocket*>(this);
        if (tmpSC)
            tmpSC->GetPrimeSocket()->Disconnect();
        return;
    }

        char sBuff[MAX_PACK_LEN];
        memset(sBuff,0,sizeof(sBuff));
        char* pBody = sBuff+ sizeof(STNetMsgHeader);
        uint32 bodyMaxLen = MAX_PACK_LEN - sizeof(STNetMsgHeader);
        int encodedLen = g_BnFGameProtoEN.Encode(msgId, pData, pBody, bodyMaxLen);
        if(encodedLen <= 0)
        {
            // 发送数据不对，断开连
           // DisconnectData();
            DBG("have err encodedLen <= 0 exceed max pack length...msgid:%d",msgId);
            return ;
        }
        int32 totalLen = encodedLen + sizeof(STNetMsgHeader);
        STNetMsgHeader* pTempHeader = reinterpret_cast<STNetMsgHeader*>(sBuff);
        pTempHeader->dwMsgID = htonl(msgId);
        pTempHeader->dwDataLen = htonl(encodedLen);
        if (totalLen > MAX_PACK_LEN)
        {
            // 发送数据不对，断开连
           // if (IsConnected())
           // DisconnectData();
            DBG("have err totalLen > MAX_PACK_LEN exceed max pack length...");
            return ;
        }

        bool rv;
        LockWriteBuffer();
        if(totalLen> 0)
         {
           rv=Write((uint8*)sBuff, (uint32)totalLen);
           if(!rv)
           if (IsConnected())
           {
             DBG("write failed disconnect ...");
             DisconnectData();
           }
         }
        UnlockWriteBuffer();
}


void SClientSocket::AckLogin(uint32 sessionId)
{
    IsPyTerm=true;
    GS_CLI_LOGIN_ACK pkg;
    pkg.Id = -1;
    pkg.ErrCode = 0;
//    SendNetMsg(sessionId, MSG_LOGIN_ACK, &pkg);
    g_SocketsManager->Send(sessionId, MSG_LOGIN_ACK, &pkg);
}

void ClientSocketManager::Send(uint32 sessionId, uint32 msgId, void *pData)
{
    serverSocketLock.Acquire();
    if (m_ClientsMap.has(sessionId))
    {
         SClientSocket*  tempClient=m_ClientsMap[sessionId];
         tempClient->SendPacket(static_cast<EN_BnFGameProto_MessageID>(msgId), pData);
    }
    else
    {
       //DBG(" send failed: cannot find sessionid: %d  msgid:%d", sessionId, msgId);
    }
    serverSocketLock.Release();
}


void SClientSocket::SetHandle(uint32 uhandle)
{
    m_handle=uhandle;
}


uint32 SClientSocket::GetHandle()
{
   return m_handle;
}


void SClientSocket::OnPing(uint32 data)
{
    GS_CLI_PING_ACK pkg;
    pkg.Data = data;
    SendPacket(MSG_PING_ACK, &pkg);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

ClientSocketManager::ClientSocketManager(const char * ListenAddress, uint32 Port)
{

  CreateSocketEngine();
  new SocketDeleter;
  sSocketEngine.SpawnThreads();
  loop_counter = 0;
  SocketID=5000;
  //DataClient=NULL;
  DataPort=0;
  UNIXTIME = time(NULL);
  if  (CreateListenSocket<SClientSocket>(ListenAddress, Port))
  {
    DBG("server run okok");
  }
  else
  {
    //LOG_D << "server no run "<< std::endl;
    exit(0);
  }
}


ClientSocketManager::~ClientSocketManager()
{

//    for (auto it = m_ClientsMap.begin(); it != m_ClientsMap.end(); ++it)
//    {
//        SClientSocket* tempClient = it->second;
//       if (tempClient->IsConnected())
//         tempClient->Disconnect();
//    }
//    m_ClientsMap.clear();
}


void ClientSocketManager::Update()
{
  if (loop_counter>4200000000)
     loop_counter=0;
   //断开练级
  if(!(++loop_counter % 10))
  {
    //删除连接
     sSocketDeleter.Update();
   }
  //检查线程
  if(!(loop_counter%10000))
          ThreadPool.IntegrityCheck();

  if(!(loop_counter%20))
  {
       UNIXTIME = time(NULL);
  }
}


bool ClientSocketManager::HasSession(uint32 sessionId) const
{
    if (m_ClientsMap.has(sessionId))
    {
      return true;
    }
    return false;
}

SClientSocket* ClientSocketManager::GetSession(uint32 sessionId)
{
    return m_ClientsMap[sessionId];
}

void ClientSocketManager::SetUserIdSession(uint64 userId, SClientSocket* s)
{
    m_UserIdSessionMap[userId] = s;
}


SClientSocket* ClientSocketManager::GetSessionWithUserID(uint64 userId)
{
    return m_UserIdSessionMap[userId];
}

void   ClientSocketManager::AddServerSocket(SClientSocket * sock)
{
    serverSocketLock.Acquire();

    ++SocketID;
    sock->SetHandle(SocketID);
    if (SocketID>4200000000)
        SocketID=5000;
    m_ClientsMap.insert(std::make_pair(sock->GetHandle(), sock));
    serverSocketLock.Release();
}

void   ClientSocketManager::RemoveServerSocket(SClientSocket * sock)
{
    serverSocketLock.Acquire();
    m_ClientsMap.erase(sock->GetHandle());
    serverSocketLock.Release();

}



