
#ifndef BoostSessionManagerNew_H
#define BoostSessionManagerNew_H
#include "ProtocolDefine.h"
#include <sdcontainers.h>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <queue>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Mutex.h"
#include "FastQueue.h"
#include "sdhashmap.h"
#include "ByteBuffer.h"
//class BoostTransferServer;

class BoostSessionClient : public boost::enable_shared_from_this<BoostSessionClient>
{
public:
    typedef std::function<void()> DisconnectedCallback;
    typedef std::function<void(uint32 msgId, void* pData)> ReceiveCallback;

    BoostSessionClient(boost::asio::io_service &io_service, DisconnectedCallback dcb, ReceiveCallback rcb);
    virtual ~BoostSessionClient();

    boost::asio::ip::tcp::socket& Socket()
    {
        return m_Socket;
    }

    void Start();
    int Send(EN_BnFGameProto_MessageID msgId, void* pData);
    void Disconnect();
    //size_t GetBuffCount() const { return m_Buffs.size(); }
    bool IsDisconnected() { return m_Disconnected; }
    void SetHandle(uint32 uhandle);
    uint32 GetHandle();
    bool IsPyTerm;
    bool IsLogin;
    ReceiveCallback m_Rcb;
    void* m_Player;
    void* m_GameWorld;
    void* m_pGameHolder;
    timeval m_LastPingTim;
    timeval m_LoginPingTim;
    uint32  m_WorldId;
private:
    void StartRead();
    void ReadHeader(const boost::system::error_code& error, size_t bytesTransferred);
    void ReadBody(EN_BnFGameProto_MessageID msgId, uint32 bodyLen,
            const boost::system::error_code& error, size_t bytesTransferred);

    void PostData(uint32 msgId, void* pData,uint32 asize);
    void OnWritten(const boost::system::error_code &error);

    void OnPing(uint32 data);
    bool m_Disconnected;
    DisconnectedCallback m_Dcb;


    boost::asio::ip::tcp::socket m_Socket;
    bool m_Sending;
    char sendBuff[MAX_PACK_LEN];
    char RecBuff[MAX_PACK_LEN];
    FastQueue<ByteBuffer*, DummyLock> _queue;
    Mutex queueLock;
    uint32  m_handle;
};



class BoostSessionManagerNew
{
public:
    BoostSessionManagerNew(short port, byte threadCount = 0);
    virtual ~BoostSessionManagerNew();

	void StartAccept();
    void KickAll();
	void Update();

    void run();

	void Send(uint32 sessionId, uint32 msgId, void *pData);
    void Disconnect(uint32 sessionId);
    bool HasSession(uint32 sessionId) const;
    Crown::shared_ptr<BoostSessionClient>& GetSession(uint32 sessionId);
    void DeleteSession(uint32 sessionId);
private:
    void Delete(uint32 sessionId);
    void OnAccecpted(boost::shared_ptr<BoostSessionClient> session,const boost::system::error_code& error);
    void CreateThreads();


    void OnRecv(uint32 sessionId, uint32 msgId, void *pData);
private:
    Mutex m_Lock;
    uint32 m_LastSessionIndex;
	int m_Port;
    byte m_ThreadCount;
    int m_AcceptCount;
    std::vector<Crown::shared_ptr<boost::thread> > m_Threads;

    //BoostTransferServer* m_Proxy;
	boost::asio::io_service& m_Service;
    std::unique_ptr<boost::asio::io_service> m_ServiceReceiver; // 多线程时用于接收数据
	boost::asio::ip::tcp::acceptor m_Acceptor;

    CnHashMap<uint32, boost::shared_ptr<BoostSessionClient> >m_ClientsMap;		// <Id, Object>

    SrvRecvFunc m_pRecvFunc;
    void* m_pGameHolder;
    timeval m_LastPingTim,time_cursor;
 public:
   void OnMyRecv(uint32 sessionId, uint32 msgId, void *pData);
   static BoostSessionManagerNew* CreateTransferServer(int32 port);
   void SetRecvFunc(SrvRecvFunc func, void* pUserData);
   void freeSession(uint32 sessionId);
   //对死连接或者没有20秒没有完成身份的连接断开
   //每一秒检测一次
   void CheckDisconnected();
};


extern  BoostSessionManagerNew* g_game_Sockets;
#endif /* BOOSTSESSIONMANAGER_H_ */
