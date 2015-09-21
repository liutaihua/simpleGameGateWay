#include "TransferPCH.h"
#include "Transfer.h"
#include "FakeTransferClient.h"
#include "FakeTransferServer.h"
#include "BoostTransferClient.h"
#include "RealTransferClient.h"
#include "BoostTransferServer.h"
#include "RealTransferServer.h"
#include "ProtocolDefine.h"

void InitFlashCheckThread();



std::string g_ProtoRecord = "";


TransferClientProxy*	CreateTransferClientProxy(bool IsReal, const char* ip, int32 port)
{
	TransferClientProxy*	pNewTransferClientProxy = nullptr;
	if(IsReal)
	{	// 真传输层客户端
		BoostTransferClient* pNewRealTransferClient = CnNew BoostTransferClient;
		//RealTransferClient*	pNewRealTransferClient = CnNew RealTransferClient;
		if(pNewRealTransferClient->Init(ip, port))
		{
			pNewTransferClientProxy = pNewRealTransferClient;
		}
		else
		{	// 初始化失败
			CnVerify(false);
			CnDelete pNewRealTransferClient;
		}
	}
	else
	{	// 伪传输层客户端
		pNewTransferClientProxy = CnNew FakeTransferClient;

		CnVerify(!g_FakeTransferClient);
		g_FakeTransferClient = static_cast<FakeTransferClient*>(pNewTransferClientProxy);
	}
	return pNewTransferClientProxy;
}
//
TransferServerProxy*	CreateTransferServerProxy(bool IsReal, int32 port)
{
	InitFlashCheckThread();
    return NULL;
	TransferServerProxy* pNewTransferServerProxy = nullptr;
	if(IsReal)
	{
// sdnet does not have a transfer server implementation using kqueue for mac os.
#ifdef USE_SDNET
		// 真传输层服务端
		RealTransferServer* pNewRealTransferServer = CnNew RealTransferServer;
		if(pNewRealTransferServer->Init(0, port))
		{
			pNewTransferServerProxy = pNewRealTransferServer;
		}
		else
		{	// 初始化失败
			CnDelete pNewRealTransferServer;
		}
#else
		BoostTransferServer* pNewRealTransferServer = CnNew BoostTransferServer;
		if (pNewRealTransferServer->Init(port))
		{
			pNewTransferServerProxy = pNewRealTransferServer;
		}
		else
		{
			CnDelete pNewRealTransferServer;
		}
#endif
	}
	else
	{	// 伪传输层服务端
		pNewTransferServerProxy = CnNew FakeTransferServer;

		g_FakeTransferServer = static_cast<FakeTransferServer*>(pNewTransferServerProxy);
	}

	return pNewTransferServerProxy;
}

bool IsThreadRunning = true;

SDTHREAD_DECLARE(AuthenticateFlashThread)(void *)
{
    int ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket < 0)
	{
		DBG("flash thread socket failed");
		SDTHREAD_RETURN(0);
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("0.0.0.0");
	service.sin_port = htons(843);

	if (::bind( ListenSocket, (sockaddr*) &service, sizeof(service)) < 0)
	{
		DBG("flash thread bind failed");
#ifdef WIN
		closesocket(ListenSocket);
#else
		close(ListenSocket);
#endif
		SDTHREAD_RETURN(0);
	}

	if (listen( ListenSocket, SOMAXCONN ) < 0)
	{
		DBG("flash thread listen failed!");
		close(ListenSocket);
		SDTHREAD_RETURN(0);
	}

	DBG("flash thread listen success!");
	while(IsThreadRunning)
	{
		// Create a SOCKET for accepting incoming requests.
        int AcceptSocket = 0;

		// Accept the connection.
		AcceptSocket = accept( ListenSocket, NULL, NULL );	// block here
		if (AcceptSocket < 0)
		{
			DBG("flash thread accept failed");
			close(ListenSocket);
			SDTHREAD_RETURN(0);
		}

		const int buflen = 512;
		char recvbuf[buflen];
		int recvbuflen = buflen;
		const char *sendbuf = "<cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";

		int iResult = recv(AcceptSocket, recvbuf, recvbuflen, 0);
		if ( iResult >= 0 )
		{
			iResult = send( AcceptSocket, sendbuf, (int)strlen(sendbuf), 0 );
		}
#ifdef WIN
		closesocket(AcceptSocket);
#else
		close(AcceptSocket);
#endif
	}
	DBG("flash thread listen end!");
	SDTHREAD_RETURN(0);
}

void InitFlashCheckThread()
{
#ifdef WIN
	// 创建一个用来验证flash安全策略的线程,仅windows(uinx下用python专用进程)
	Crown::SDTHREADID thrdId;
	Crown::SDCreateThread(NULL, AuthenticateFlashThread, NULL, &thrdId);
#endif
}


