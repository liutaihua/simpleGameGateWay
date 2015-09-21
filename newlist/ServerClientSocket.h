#ifndef __LOGON_COMM_SERVER_H
#define __LOGON_COMM_SERVER_H
#include "Transfer/ProtocolDefine.h"
#include "Network.h"
//#include <sdcontainers.h>
#include "sdu/sdtype.h"
#include <queue>
#include "Mutex.h"
//#include "FastQueue.h"
//#include "sdhashmap.h"
#include "ByteBuffer.h"
#include "GameHolder.h"
//#include "tcpproxy.h"

//const std::string MD5_KEY = "8ff74cafd631ccfec07c58bb255834a6";

typedef struct STNetMsgHeader
{
    uint32	dwMsgID;			// 包头标示
    uint32	dwDataLen;		// 数据包长度(不包括包头)
} STNetMsgHeader;

typedef struct STNetIPAddress
{
    uint32 ServerId;
    uint32 Port;
} STNetIPAddress;

#define MAX_PACK_LEN		8192

class ClientSocketManager;
class SClientSocket;
class ProxySClientSocket;
extern  ClientSocketManager* g_SocketsManager;
class SClientSocket : public TcpSocket
{

  bool m_Sending;
  char sendBuff[MAX_PACK_LEN];
  char RecBuff[MAX_PACK_LEN];
//  FastQueue<ByteBuffer*, DummyLock> _queue;
  Mutex queueLock;
  uint32  m_handle;

private:
  bool IsLogin;
  CLI_GS_LOGIN_REQ* m_LoginReq;
  ProxySClientSocket* m_pProxySocket;

protected:
  uint32 m_remaining,m_Cmd;

public:
  char m_LoginData[MAX_PACK_LEN];

  bool IsPyTerm,IsGet;
  time_t m_LastPingTim;

public:
  SClientSocket(SOCKET fd, const sockaddr_in * peer);
  ~SClientSocket();
  virtual void OnRecvData();
  virtual void OnDisconnect();
  virtual void OnConnect();

  // Disconnect the socket.
  void DisconnectData();
  void SendPacket(EN_BnFGameProto_MessageID msgId, void *pData);
  void SetProxySocket(ProxySClientSocket* p);
  ProxySClientSocket* GetProxySocket() const {return m_pProxySocket;}

  void HandlePacket();

  void AckLogin(uint32 sessionId);

  void OnPing(uint32 data);

  void SetHandle(uint32 uhandle);

  uint32 GetHandle();

};


class ProxySClientSocket : public SClientSocket
{

public:
    ProxySClientSocket(SOCKET fd, const sockaddr_in * peer);
    ~ProxySClientSocket();

private:
    char proxySendBuff[MAX_PACK_LEN];
    char proxyRecBuff[MAX_PACK_LEN];
    SClientSocket* m_pPrimeSocket;

public:

    void SetPrimeSocket(SClientSocket* p);
    virtual void OnRecvData();
    virtual void OnDisconnect();
    virtual void OnConnect();
    SClientSocket* GetPrimeSocket() const {return m_pPrimeSocket;}
};

class ClientSocketManager
{
public:

  ClientSocketManager(const char * ListenAddress, uint32 Port);
  virtual ~ClientSocketManager();

  void Send(uint32 sessionId, uint32 msgId, void *pData);
  bool HasSession(uint32 sessionId) const;
  SClientSocket* GetSession(uint32 sessionId);

  void SetUserIdSession(uint64, SClientSocket*);
  SClientSocket* GetSessionWithUserID(uint64);
  void Update();
private:
  uint32 DataPort;
  uint32 loop_counter;
  uint32 SocketID;
  uint32 m_LastSessionIndex;
  int m_Port;

  CnHashMap<uint32, SClientSocket* >m_ClientsMap;
  CnHashMap<uint64, SClientSocket* >m_UserIdSessionMap;

  Mutex serverSocketLock;


  timeval m_LastPingTim,time_cursor;



public:

  void   AddServerSocket(SClientSocket * sock);
  void   RemoveServerSocket(SClientSocket * sock);

public:
  Mutex serverDataLock;
};




#endif
