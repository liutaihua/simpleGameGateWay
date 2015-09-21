#include "TransferPCH.h"
#include "BoostSessionManagerNew.h"
//#include "BoostTransferServer.h"
#include "SharedResouces.h"
#include "Transfer.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "ByteBuffer.h"
#include "GameHolder.h"
#include "GameWorld.h"
#include "Player.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::bind;
using std::ofstream;
using std::ios;


BoostSessionManagerNew* g_game_Sockets=NULL;

BoostSessionClient::BoostSessionClient(boost::asio::io_service& io_service, DisconnectedCallback dcb, ReceiveCallback rcb) :
    m_Disconnected(false), m_Socket(io_service),m_Dcb(dcb), m_Rcb(rcb), m_Sending(false)
{
    gettimeofday(&m_LastPingTim,NULL);
    gettimeofday(&m_LoginPingTim,NULL);
    IsPyTerm=false;
    IsLogin=false;
    m_Player=NULL;
    m_GameWorld=NULL;
    m_WorldId=0;
}

BoostSessionClient::~BoostSessionClient()
{
    ByteBuffer * pck;
    while((pck = _queue.Pop()))
        delete pck;
}

void BoostSessionClient::Start()
{
    boost::system::error_code e;
    // Turn off the Nagle algorithm.
    m_Socket.set_option(ip::tcp::no_delay(true), e);
    if (e)
    {
        LOG_D <<"error 111 " << e.message() << std::endl;
        Disconnect();
        return;
    }
    gettimeofday(&m_LoginPingTim,NULL);
    PostData(LOGIN, 0,0);
    StartRead();
    DBG("Start :%d",  m_handle);
}


void BoostSessionClient::StartRead()
{
    async_read(m_Socket, buffer(RecBuff, sizeof(STNetMsgHeader)),
            boost::bind(&BoostSessionClient::ReadHeader, shared_from_this(),
                    placeholders::error,
                    placeholders::bytes_transferred));
}

void BoostSessionClient::ReadHeader(const boost::system::error_code& error, size_t )
{
    if (IsDisconnected())
        return;

    if (error)
    {
        Disconnect();
        return;
    }

    const STNetMsgHeader* pstHeader = reinterpret_cast<STNetMsgHeader*>(RecBuff);
    uint32 msgId = ntohl(pstHeader->dwMsgID);
    uint32 bodyLen = ntohl(pstHeader->dwDataLen);
    //CnAssert(bodyLen <= MAX_PACK_LEN);

    if(!(bodyLen <= MAX_PACK_LEN))
    {
        boost::asio::ip::tcp::endpoint remote_ep = m_Socket.remote_endpoint();
        boost::asio::ip::address remote_ad = remote_ep.address();
        std::string s = remote_ad.to_string();

        DBG("[maybe attack]body length exceed max details: %d, %d, %s", msgId, bodyLen, s);
        Disconnect();
        return;
    }

    async_read(m_Socket, buffer(RecBuff, bodyLen),
            boost::bind(&BoostSessionClient::ReadBody, shared_from_this(),
                    static_cast<EN_BnFGameProto_MessageID>(msgId), bodyLen,
                    placeholders::error,
                    placeholders::bytes_transferred));
}

void BoostSessionClient::ReadBody(EN_BnFGameProto_MessageID msgId, uint32 bodyLen,
                const boost::system::error_code& error, size_t bytesTransferred)
{
    if (IsDisconnected())
        return;
    if (error)
    {
        LOG_D << "error 444" << error.message() << std::endl;
        Disconnect();
        return;
    }
    try
    {

        CnVerify(bytesTransferred == bodyLen);
        char pData[MAX_PACK_LEN];
        //坑啊 g_BnFGameProto.Decode 缓冲区必须定义 MAX_PACK_LEN 否则返回为-1
        int decodedLen = g_BnFGameProto.Decode(msgId, RecBuff, bodyLen, pData, MAX_PACK_LEN);
        if (decodedLen <= 0)
        {
            WARN("Protocol error. %d", msgId);
            Disconnect();
            return;
        }

        if (!g_ProtoRecord.empty())
        {
            //ofstream myFile;

            //myFile.open(g_ProtoRecord, ios::app | ios::binary);
            //myFile.write(RecBuff, bodyLen + sizeof(STNetMsgHeader));
            // myFile.close();
        }

        //LOG_D << "recv MsgId: " << msgId << " Len : " << bodyLen << std::endl;
        PostData(msgId, pData,decodedLen);
        StartRead();
    }
    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionClient ReadBody:"<< e.what() << std::endl;
    }
}


int BoostSessionClient::Send(EN_BnFGameProto_MessageID msgId, void *pData)
{
    if (IsDisconnected())
        return 0;
    //临时加密缓冲
    char sBuff[MAX_PACK_LEN];
    char* pBody = sBuff+ sizeof(STNetMsgHeader);
    uint32 bodyMaxLen = MAX_PACK_LEN - sizeof(STNetMsgHeader);
    int encodedLen = g_BnFGameProto.Encode(msgId, pData, pBody, bodyMaxLen);
    if(encodedLen <= 0)
        throw std::runtime_error("Encode error.");

    int32 totalLen = encodedLen + sizeof(STNetMsgHeader);
    STNetMsgHeader* pTempHeader = reinterpret_cast<STNetMsgHeader*>(sBuff);
    pTempHeader->dwMsgID = htonl(msgId);
    pTempHeader->dwDataLen = htonl(encodedLen);
    //buff->LenUsed = totalLen;
    //LOG_D << "MsgId:" << msgId << "Len:"<< totalLen << std::endl;
    if (totalLen > MAX_PACK_LEN)
    {
        // 发送数据不对，断开连
        Disconnect();
        DBG("exceed max pack length...");
        return -1;
    }

     if (m_Sending)
     {
       //正在发送中加密后的数据保存到数据缓冲种
       queueLock.Acquire();
        ByteBuffer * pck = new ByteBuffer(totalLen);
        if(totalLen) pck->append((const uint8*)sBuff, totalLen);
        _queue.Push(pck);
        queueLock.Release();

        return 1;
     }
     else
     {
       m_Sending = true;
       //复制到发送缓冲
       memcpy(sendBuff,sBuff,totalLen);
       async_write(m_Socket, buffer(sendBuff, totalLen),
                  boost::bind(&BoostSessionClient::OnWritten, shared_from_this(),placeholders::error));
     }
}

void BoostSessionClient::OnWritten(const boost::system::error_code& error)
{
    if (IsDisconnected())
     {
        m_Sending = false;
        return;
     }

    if (m_Disconnected)
     {
        m_Sending = false;
        return;
     }

    if (error)
    {
        LOG_D << "error 222 " << error.message() << std::endl;
        m_Sending = false;
        Disconnect();
        return;
    }
   queueLock.Acquire();
   //队列为空不再发送
   if(!_queue.HasItems())
    {
        m_Sending = false;
        queueLock.Release();
        gettimeofday(&m_LastPingTim,NULL);
        return;
    }
   try
   {
       uint32 ssize=0;
       ByteBuffer * pck;
       pck = _queue.Pop();
       //复制到发送缓冲
       ssize=pck->size();
       memcpy(sendBuff,pck->contents(),pck->size());
       //删除缓冲队列现在发送的
       delete pck;
       m_Sending = true;
       queueLock.Release();
       async_write(m_Socket, buffer(sendBuff, ssize),
                   boost::bind(&BoostSessionClient::OnWritten, shared_from_this(), placeholders::error));
       gettimeofday(&m_LastPingTim,NULL);
   }
   catch(std::exception& e)
   {
       LOG_E <<"BoostSessionClient OnWritten:"<< e.what() << std::endl;
   }
}

void BoostSessionClient::Disconnect()
{

    if(m_Disconnected)
        return;
    try
    {

        ByteBuffer * pck;
        while((pck = _queue.Pop()))
            delete pck;
        PostData(LOGOUT, 0,0);

         m_Disconnected = true;
        //shutdown() would change the `error` if an error occurred, instead of throwing an exception.
        boost::system::error_code error;
        m_Socket.shutdown(ip::tcp::socket::shutdown_both, error);
        m_Socket.close();
        //打印消息放后面把 不然还以为是2次断开
        DBG("Disconnect :%d",this->m_handle);
        m_Dcb();
    }
    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionClient Disconnect:"<< e.what() << std::endl;
    }

}

//自带锁消息队列能保证多线程安全
void BoostSessionClient::PostData(uint32 msgId, void *pData,uint32 asize)
{
    try
    {
        if(m_Disconnected)
            return;

        ByteBuffer * pck=NULL;
        switch(msgId)
        {
        case MSG_PING_REQ:
        {
            CLI_GS_PING_REQ* req = static_cast<CLI_GS_PING_REQ*>(pData);
            OnPing(req->Data);
            gettimeofday(&m_LastPingTim,NULL);
        }
            break;
        case LOGIN:
        {
            pck = new ByteBuffer(sizeof(uint32)*2+asize);
            *pck<< m_handle;
            *pck<< msgId;
            if (m_pGameHolder!=NULL)
                ((GameHolder*)m_pGameHolder)->m_DataQueue.Push(pck);
            else
                delete pck;
        }
            break;
        case LOGOUT:
        {
            pck = new ByteBuffer(sizeof(uint32)*2+asize);
            *pck<< m_handle;
            *pck<< msgId;
            if (m_pGameHolder!=NULL)
                ((GameHolder*)m_pGameHolder)->m_DataQueue.Push(pck);
            else
                delete pck;
        }
            break;
        default:
        {
            gettimeofday(&m_LastPingTim,NULL);
            //m_Rcb(msgId, pData);
            if (!IsLogin)
            {
                pck = new ByteBuffer(sizeof(uint32)*2+asize);
                *pck<< m_handle;
                *pck<< msgId;
                pck->append((char*)pData,asize);
                if (m_pGameHolder!=NULL)
                    ((GameHolder*)m_pGameHolder)->m_DataQueue.Push(pck);
                else
                    delete pck;
            }
            //登陆后则有世界消息直接投递到世界
            //本来是到人的只是我们的世界有处理消息先这样处理把
            if (IsLogin)
            {
                pck = new ByteBuffer(sizeof(uint32)*2+asize);
                *pck<< m_handle;
                *pck<< msgId;
                pck ->append((char*)pData,asize);
                if (m_Player!=NULL)
                    ((Player*)m_Player)->m_DataQueue.Push(pck);
                else
                    delete pck;
            }
            break;
        }
        }
    }
    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionManagerNew CheckDisconnected:"<< e.what() << std::endl;
    }
}


void BoostSessionClient::SetHandle(uint32 uhandle)
{
    m_handle=uhandle;
}


uint32 BoostSessionClient::GetHandle()
{
   return m_handle;
}


void BoostSessionClient::OnPing(uint32 data)
{
    GS_CLI_PING_ACK pkg;
    pkg.Data = data;
    Send(MSG_PING_ACK, &pkg);
}


////////////////////////////////////////////////////////////////////////////////////////////////

BoostSessionManagerNew::BoostSessionManagerNew(short port, byte threadCount) :
    m_LastSessionIndex(5000),
	m_Port(port),
    m_AcceptCount(0),
    m_ThreadCount(threadCount),
	m_Service(SharedResouces::GetInstance().IOService()),
    m_Acceptor(m_Service)
{
    m_pGameHolder=NULL;
    gettimeofday(&m_LastPingTim,NULL);
    tcp::endpoint endpoint(tcp::v4(), port);
    m_Acceptor.open(endpoint.protocol());
    m_Acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    try
    {
        m_Acceptor.bind(endpoint);
        m_Acceptor.listen();
    }
    catch (const boost::system::system_error& e)
    {
        LOG_E << e.what() << std::endl;
        exit(0);
    }
    m_ThreadCount=0;
    CreateThreads();
}

BoostSessionManagerNew::~BoostSessionManagerNew()
{
   CnDelete SharedResouces::GetInstancePtr();
    if (m_ThreadCount)
        m_Service.stop();

    for (std::size_t i = 0; i < m_Threads.size(); ++i)
        m_Threads[i]->join();

    for (auto it = m_ClientsMap.begin(); it != m_ClientsMap.end(); ++it)
    {
        Crown::shared_ptr<BoostSessionClient>& tempClient = it->second;
        tempClient->Disconnect();
        tempClient.reset();
    }
    m_ClientsMap.clear();
}

void BoostSessionManagerNew::KickAll()
{
    DBG("holy shit...full kick all");
    for (auto it = m_ClientsMap.begin(); it != m_ClientsMap.end();it++)
    {
        Crown::shared_ptr<BoostSessionClient>& tempClient = it->second;
        tempClient->Disconnect();
        tempClient.reset();
    }
    m_ClientsMap.clear();
    StartAccept();
}

void BoostSessionManagerNew::StartAccept()
{
    ++m_LastSessionIndex;
    while (m_ClientsMap.has(m_LastSessionIndex))
    {
        ++m_LastSessionIndex;
        if (m_LastSessionIndex>4200000000)
            m_LastSessionIndex=5000;
    }

    // FIXME: 无法处理超过2000人的情况
    BoostSessionClient::DisconnectedCallback dcb = boost::bind(&BoostSessionManagerNew::Delete, this, m_LastSessionIndex);
    BoostSessionClient::ReceiveCallback rcb = boost::bind(&BoostSessionManagerNew::OnRecv, this, m_LastSessionIndex, _1, _2);
    //BoostSessionClient* newSession =new BoostSessionClient(m_Service, dcb, rcb);
    auto newSession = boost::make_shared<BoostSessionClient>(m_Service, dcb, rcb);
    //DBG("session %d start accept...", m_LastSessionIndex);
    m_AcceptCount += 1;
    if (m_ClientsMap.has(m_LastSessionIndex))
    {
    //应该是没有
        newSession.reset();
    }
    else
    {
     //保存到hash表里自己用handle标记句柄
      newSession->SetHandle(m_LastSessionIndex);
      newSession->m_pGameHolder=m_pGameHolder;
      m_Acceptor.async_accept(newSession->Socket(),
                   bind(&BoostSessionManagerNew::OnAccecpted, this, newSession, placeholders::error));
    }
}

void BoostSessionManagerNew::OnAccecpted(boost::shared_ptr<BoostSessionClient> session, const boost::system::error_code& error)
{
    try
    {
        if (error)
        {
            DBG("OnAccecpted erer");
            LOG_D << "error in accept " << error.message() ;
            WARN("accept error and cannot find session:%d...", session->GetHandle());
            session.reset();
        }
        else
        {
            session->Start();
            m_Lock.Acquire();
            m_ClientsMap.insert(std::make_pair(m_LastSessionIndex, session));
            m_Lock.Release();
        }

    }
    catch(std::exception& e)
    {
        LOG_E <<"OnAccecpted:"<< e.what() << std::endl;
    }
    StartAccept();
}

void BoostSessionManagerNew::CreateThreads()
{
    for (std::size_t i = 0; i < m_ThreadCount; ++i)
    {
        Crown::shared_ptr<boost::thread> thread(CnNew boost::thread(boost::bind(&BoostSessionManagerNew::run, this)));
        m_Threads.push_back(thread);
    }
}

void BoostSessionManagerNew::OnRecv(uint32 sessionId, uint32 msgId, void *pData)
{
    OnMyRecv(sessionId, msgId, pData);
}


void BoostSessionManagerNew::OnMyRecv(uint32 sessionId, uint32 msgId, void *pData)
{
    CnVerify(m_pRecvFunc);
    m_pRecvFunc(sessionId, msgId, pData, m_pGameHolder);
}

void BoostSessionManagerNew::run()
{
    while(ChildThreadRunning)
    {
        Update();
        CheckDisconnected();
        Crown::SDSleep(10);
    }
}
void BoostSessionManagerNew::Update()
{
    try
    {
        m_Service.poll();
    }
    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionManagerNew Update:"<< e.what() << std::endl;
    }
    catch(...)
    {
        LOG_E << "unknown error in transfer..." << std::endl;
    }
}

void BoostSessionManagerNew::Send(uint32 sessionId, uint32 msgId, void *pData)
{
    if (m_ClientsMap.has(sessionId))
    {
         Crown::shared_ptr<BoostSessionClient>&  tempClient=m_ClientsMap[sessionId];
         tempClient->Send(static_cast<EN_BnFGameProto_MessageID>(msgId), pData);
    }
    else
    {
        return;
        //DBG(" send failed: cannot find sessionid: %d  msgid:%d", sessionId, msgId);
    }
}

void BoostSessionManagerNew::Disconnect(uint32 sessionId)
{
    try
    {
        if (m_ClientsMap.has(sessionId))
        {
            Crown::shared_ptr<BoostSessionClient>& tempClient=m_ClientsMap[sessionId];
            tempClient->Disconnect();
        }
    }
    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionManagerNew Disconnect:"<< e.what() << std::endl;
    }
}


void BoostSessionManagerNew::DeleteSession(uint32 sessionId)
{
    DBG("delete session from boost transfer %d", sessionId);
    Disconnect(sessionId);
}

//只断开连接删除统一在循环里5分钟后删除
void BoostSessionManagerNew::Delete(uint32 sessionId)
{
    try
    {
        if (m_ClientsMap.has(sessionId))
        {
            Crown::shared_ptr<BoostSessionClient>& tempClient=m_ClientsMap[sessionId];
            tempClient->Disconnect();
        }
    }
    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionManagerNew Delete:"<< e.what() << std::endl;
    }
}

Crown::shared_ptr<BoostSessionClient>& BoostSessionManagerNew::GetSession(uint32 sessionId)
{
     return m_ClientsMap[sessionId];
}


bool BoostSessionManagerNew::HasSession(uint32 sessionId) const
{
    if (m_ClientsMap.has(sessionId))
    {
      return true;
    }
    return false;
}

void BoostSessionManagerNew::SetRecvFunc(SrvRecvFunc func, void* pUserData)
{
    m_pRecvFunc = func;
    m_pGameHolder = pUserData;
    StartAccept();
}


void BoostSessionManagerNew::freeSession(uint32 sessionId)
{
    try
    {
        m_Lock.Acquire();
        for (auto it = m_ClientsMap.begin(); it != m_ClientsMap.end(); )
        {
            if (it->first==sessionId)
            {
                Crown::shared_ptr<BoostSessionClient>& tempClient = it->second;
                m_ClientsMap.erase(it);
               //  tempClient.reset();
                m_Lock.Release();
                return;
            }
            else
            {
               it++;
            }
        }
         m_Lock.Release();
    }

    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionManagerNew CheckDisconnected:"<< e.what() << std::endl;
    }
    catch(...)
    {
        LOG_E << "unknown error in boost session manager" << std::endl;
    }
}



//每一秒检测一次
void BoostSessionManagerNew::CheckDisconnected()
{
    gettimeofday(&time_cursor,NULL);
    int take_time=0;
    take_time= (int32((time_cursor.tv_sec - m_LastPingTim.tv_sec) * 1000) + int32((time_cursor.tv_usec - m_LastPingTim.tv_usec) / 1000));
    if (take_time<1000)
      return;
    try
    {

        m_Lock.Acquire();
        gettimeofday(&m_LastPingTim,NULL);
        for (auto it = m_ClientsMap.begin(); it != m_ClientsMap.end(); ++it)
        {
            Crown::shared_ptr<BoostSessionClient>& tempClient = it->second;
            take_time=0;
            if (!tempClient->IsDisconnected())
                take_time= (int32((time_cursor.tv_sec - tempClient->m_LastPingTim.tv_sec) * 1000) + int32((time_cursor.tv_usec - tempClient->m_LastPingTim.tv_usec) / 1000));
            //一分钟没有收到消息或者发送不成功主动断开
            if (take_time>600000)
                tempClient->Disconnect();
            //后续用断开连接消息驱动删除
        }

        //用户没有连接
        for (auto it = m_ClientsMap.begin(); it != m_ClientsMap.end(); ++it)
        {
            Crown::shared_ptr<BoostSessionClient>& tempClient = it->second;
            take_time=0;
            if (!tempClient->IsDisconnected() && !tempClient->IsLogin && !tempClient->IsPyTerm)
                take_time= (int32((time_cursor.tv_sec - tempClient->m_LoginPingTim.tv_sec) * 1000) + int32((time_cursor.tv_usec - tempClient->m_LoginPingTim.tv_usec) / 1000));
            //20秒没有登陆成功或者表示为服务器则断开连接
            if (take_time>20000)
                tempClient->Disconnect();
            //后续用断开连接消息驱动删除
        }

        //清理死连接速度不能快保证队列数据处理完毕 5分钟够了最多占用的内存其他的不影响
        for (auto it = m_ClientsMap.begin(); it != m_ClientsMap.end(); )
        {
            Crown::shared_ptr<BoostSessionClient>& tempClient = it->second;
            take_time=0;
            if (tempClient->IsDisconnected())
                take_time= (int32((time_cursor.tv_sec - tempClient->m_LoginPingTim.tv_sec) * 1000) + int32((time_cursor.tv_usec - tempClient->m_LoginPingTim.tv_usec) / 1000));
            //2分钟后删除对象
            if (take_time>1200000)
            {
                it=m_ClientsMap.erase(it);
               // tempClient.reset();
            }
            else
            {
               it++;
            }
        }
        m_Lock.Release();
    }
    catch(std::exception& e)
    {
        LOG_E <<"BoostSessionManagerNew CheckDisconnected:"<< e.what() << std::endl;
    }
    catch(...)
    {
        LOG_E << "unknown error in boost session manager" << std::endl;
    }
}

BoostSessionManagerNew*	BoostSessionManagerNew::CreateTransferServer(int32 port)
{
        BoostSessionManagerNew* pNewRealTransferServer = CnNew BoostSessionManagerNew(port, 0);
        g_game_Sockets=pNewRealTransferServer;
        return pNewRealTransferServer;
}
