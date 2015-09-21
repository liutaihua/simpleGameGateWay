#pragma once
#include <Transfer/ProtocolDefine.h>

//#include <boost/program_options/variables_map.hpp>
#include <sys/time.h>
#include <time.h>
#include <list>
#include "md5.h"
#include <sys/timeb.h>
#include "newlist/ByteBuffer.h"
#include "newlist/Mutex.h"
#include "newlist/FastQueue.h"

const int32 ERR_LOGIN_PLAYER_EXIST = -2;    // 玩家已经存在
const int32 ERR_LOGIN_TABLE_FULL = -3;     // 桌子已满
const int32 ERR_LOGIN_VERSION_NOMATCH = -4; // 版本不匹配
const int32 ERR_LOGIN_DATA_ERROR = -5;      // 玩家数据错误
const int32 ERR_LOGIN_GAME_OVER = -6;      // 副本已经结束
const int32 ERR_ADD_SPEED = -7;      // 客户端加速
const int32 ERR_LOGIN_MD5_ERROR = -8;      // 客户端加速

class GameHolder
{
public:
  GameHolder();
  virtual ~GameHolder();//

//  virtual bool	Init();
  virtual void	Update();
  virtual void	UpdateData();
//  virtual void  UpdateWorlds();
//  void          UpdateWorldsDel(GameWorld *tWorld);
  virtual void	Uninit();

//  GameWorld* GetWorld(uint32 RommId);
//  static GameHolder* GetGameHolder();
//  void MqSend(byte* buffer, int len);
//  void OnUserDisConnected(uint64 userId);
//  void KickAllUser();
//  void KickUser(uint64 userid);
//  void BroadCastChatMsg(const std::string &content);
//  void CleanTodayAbyssValue();
//  void AddStamin(uint32 value);
//  void SaveAllData();
//  void ReloadPyModule();
//  static void EnableConditionDebug() {Logger::GetInstance().SetLevel(LOGLEVEL_CONDITION_DEBUG);}
//  static void DisableConditionDebug() {Logger::GetInstance().SetLevel(LOGLEVEL_DEBUG);}
//  static void SetLogLevel(LogLevel level) {Logger::GetInstance().SetLevel(level);}

  //void OnRecv(uint32 SessionId, uint32 MsgId, void* pData);
  void SendNetMsg(uint32 SessionId, uint32 MsgId, void* pData);

//  void    OnPlayerLogin(uint32 sessionId, uint64 userId);
//  void    OnPlayerLogout(uint64 userId);
//  void    OnRemoveWorld(GameWorld * world);
//  void    OnRemovePlayer(Player * play);
//  void    OnRemoveSession(uint32 SessionId);
  //用户登陆认证新流程
  void OnRecvNew(uint32 SessionId, uint32 MsgId, void* pData);

//  Player* GetPlayer(uint64 userid);
//  std::list<uint64> GetAllPlayer();
  void   SetServerStartTime(uint32 val);
  uint32  GetServerStartTime() { return m_ServerStartTime; }

  //自带锁消息队列 处理用户登陆前和db发送过来的消息
  FastQueue<ByteBuffer*, Mutex> m_DataQueue;
  uint32  m_ServerStartTime;
private:
  // 网络事件

  void OnSessionDisconnected( uint32 sessionId );

  timeval time_cursor;
  timeval time_cursor_last;
  Mutex m_Lock;
  enum { MAX_WORLD_COUNT = MAX_PLAYER_NUM };

 // std::set<uint32> m_PySessionIds;


  uint32	m_FrameCount;
  std::unordered_map<uint64, uint32> m_UserIdToSessionId;

};

//#endif
