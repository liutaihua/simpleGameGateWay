#include "GameHolder.h"

//#include "Player.h"

#include <sys/timeb.h>
#include "newlist/Network.h"
#include "newlist/ServerClientSocket.h"


timeval g_timeval;
int32 g_update_time;
GameHolder* g_game_holder = NULL;
std::string g_ServerRootUrl;

GameHolder::GameHolder() :
    m_FrameCount(0),
    m_ServerStartTime(0)
{
    gettimeofday(&g_timeval,NULL);
    g_SocketsManager->SetRecvFunc(this);
}

GameHolder::~GameHolder()
{
    ByteBuffer * pck;
    while((pck = m_DataQueue.Pop()))
        delete pck;
}


void GameHolder::Uninit()
{
    if(g_SocketsManager)
    {
        CnDelete g_SocketsManager;
        g_SocketsManager = nullptr;
    }

    m_UserIdToSessionId.clear();
}



void GameHolder::Update()
{
    timeval new_time_val;
    timeval time_cursor;
    timeval time_cursor_last;

    gettimeofday(&new_time_val,NULL);
    g_update_time = int32((new_time_val.tv_sec - g_timeval.tv_sec) * 1000)
            + int32((new_time_val.tv_usec - g_timeval.tv_usec) / 1000);
    g_timeval = new_time_val;

    gettimeofday(&time_cursor,NULL);
    gettimeofday(&time_cursor_last,NULL);

    // 传输层Update 起码
    //CnVerify(g_SocketsManager);
    g_SocketsManager->Update();

    UpdateData();
}



void GameHolder::SendNetMsg(uint32 SessionId, uint32 MsgId, void* pData)
{
//    CnVerify(g_SocketsManager);
    g_SocketsManager->Send(SessionId, MsgId, pData);
}


void   GameHolder::SetServerStartTime(uint32 val)
{
  m_ServerStartTime = val;
}


//每次先取一个
void GameHolder::UpdateData()
{
    uint32 sessionId;
    uint32 MsgId;
    uint32 asize;
    char Data[MAX_PACK_LEN];
    ByteBuffer * pbuffer=NULL;
    uint32 gameholder_consume_time = 0;

    timeval temp_time_start;
    timeval temp_time_end;
    gettimeofday(&temp_time_start,NULL);

    try
    {
        for (auto i=0;i<5;i++)
        {
            pbuffer=m_DataQueue.Pop();
            if (pbuffer)
            {
                memset(Data,0,sizeof(Data));
                *pbuffer>>sessionId;
                *pbuffer>>MsgId;
                asize=pbuffer->size()-pbuffer->rpos();
                pbuffer->read((uint8 *)Data,asize);
                delete pbuffer;
                m_Lock.Acquire();
                OnRecvNew(sessionId,MsgId,Data);
                m_Lock.Release();
            }
            else
                return;
        }
    }
    catch(std::exception& e)
    {
        //LOG_E <<"GameHolder UpdateData:"<< e.what() << std::endl;
    }
    gettimeofday(&temp_time_end,NULL);
    gameholder_consume_time += (int32((temp_time_end.tv_sec - temp_time_start.tv_sec) * 1000)
                           + int32((temp_time_end.tv_usec - temp_time_start.tv_usec) / 1000));
    if (gameholder_consume_time > 200) {
        DBG("gameholder update data takes a long time: %d", gameholder_consume_time);
    }

}


//只处理未标记登陆的用户
void GameHolder::OnRecvNew( uint32 sessionId, uint32 MsgId, void* pData )
{
  try
  {

    if (!g_SocketsManager->HasSession(sessionId))
      return;
    SClientSocket * BoostSC=g_SocketsManager->GetSession(sessionId);

    int64 worldId;
    // 网络消息处理函数
    switch(MsgId)
      {
      case UINT32_MAX:	// UINT32_MAX 表示玩家断开连
        break;
//      case 0:
//        //DBG("OnConnected sid=%d", sessionId);
//        //OnSessionConnected(sessionId);
//        break;
      case MSG_LOGIN_REQ:		// 玩家请求登陆
        {
          //std::cout<<"gameHolder process MSG_LOGIN_REQ"<<"\n";
          CLI_GS_LOGIN_REQ* req = static_cast<CLI_GS_LOGIN_REQ*>(pData);

          char str_buf[1024];
          memset(str_buf, 0, 1024);
          snprintf(str_buf, 1024,
                   "SessionKey=%s&UserId=%s&TableId=%d&major=%d&minor=%d&revision=%d&%s",
                   req->SessionKey, req->UserId,
                   req->TableId, req->major,
                   req->minor, req->revision, MD5_KEY.c_str());
          std::string input_str = str_buf;
          //std::cout<<"user require login "<<input_str<<"\n";
          if (strncmp(req->SessionKey, "HELLO", sizeof(req->SessionKey) - 1) != 0 && 
                  strncmp(req->SessionKey, "i_am_robot", sizeof(req->SessionKey) - 1) != 0)
            {
              DBG("user require login %d, %d, %s", req->UserId, req->TableId, req->SessionKey);
            }
          std::string server_key = md5(input_str);
          req->KEY[PROTOCOL_KEY_LEN - 1] = 0;
          std::string client_key = req->KEY;
          if (server_key != client_key)
            {
              //std::cout<<"client_key incorrect"<<"\n";
              GS_CLI_LOGIN_ACK pkg;
              pkg.ErrCode = ERR_LOGIN_MD5_ERROR;       //客户端加速
              pkg.Id = -1;
              SendNetMsg(sessionId, MSG_LOGIN_ACK, &pkg);
              return;
            }
          // 版本检测
          if(!(req->major == VERSION_MAJOR && req->minor == VERSION_MINOR && req->revision == VERSION_REVISION))
            {   // 版本不匹配
              //std::cout<<"client version incorrect"<<"\n";
              GS_CLI_LOGIN_ACK pkg;
              pkg.ErrCode = ERR_LOGIN_VERSION_NOMATCH;       // 版本不匹配
              pkg.Id = -1;
              SendNetMsg(sessionId, MSG_LOGIN_ACK, &pkg);
              return;
            }
          //为服务内数据交互连接
          if (strcmp(req->UserId, "0") == 0 || strcmp(req->UserId, "0") == 19922945)
            {
              //标记为服务端
              //std::cout<<"user login success as a terminus test"<<"\n";
              BoostSC->m_WorldId=4200000001;
              BoostSC->IsPyTerm=true;
              GS_CLI_LOGIN_ACK pkg;
              pkg.Id = -1;
              pkg.ErrCode = 0;
              SendNetMsg(sessionId, MSG_LOGIN_ACK, &pkg);
              return;
            }
          else
            {
              //std::cout<<"user login success as a terminus test 333"<<"\n";

              BoostSC->m_WorldId=sessionId;
              BoostSC->IsCreateWorld=true;
              BoostSC->IsLogin=true;


//              OnWorld_PlayerLogin(sessionId, atoll(req->UserId), req->SessionKey, tempTableid);

            }
          break;
        }
        //服务间数据传递的连接到此结束
      case MSG_COMMAND_REQ:
        {
          auto req = static_cast<CLI_GS_COMMAND_REQ*>(pData);
          if (BoostSC->IsPyTerm)
            {
//              GMCommand::GetInstance().Execute(req->Command, nullptr);
            }
          else if (BoostSC->IsLogin)
            {
              //登陆后的消息不在走这里了
//              if (gameWD)
//                gameWD->OnRecv(sessionId, MsgId, pData);
              return;
            }
          break;
        }
      default:
        {
        switch (MsgId) {
        case MSG_MOVE_REQ:
            GS_CLI_MOVE_ACK moveAck;
            moveAck.X = 0;
            moveAck.Y = 0;
            SendNetMsg(sessionId, MSG_MOVE_ACK, &moveAck);
            break;
        case MSG_CASTING_REQ:
            GS_CLI_CASTING_ACK skillAck;
            skillAck.SkillId = 1;
            skillAck.SkillSeq = 0;
            skillAck.ErrorId = 0;
            SendNetMsg(sessionId, MSG_CASTING_ACK, &skillAck);
            break;
        default:
            break;
        }
        }
      }

  }
  catch(std::exception& e)
  {
    //LOG_E <<"GameHolder OnRecvNew:"<< e.what() << std::endl;
  }
}

