/****************************
服务器监听对象类。
处理网络的异步tcp监听。
可以使用多实例模式。
委托给管理器处理网络的连接消息。
开发：chenshaopo 2015.1
***************************/
#ifndef _LISTENSOCKET_H
#define _LISTENSOCKET_H
#include "Common.h"
#include "Network.h"
#include "SocketOps.h"
#include "SocketDefines.h"

template<class T>
class ListenSocket : public BaseSocket
{
    int new_fd;
    sockaddr_in new_peer;
    sockaddr_in address;
public:

    ListenSocket()
    {
        m_fd = socket(AF_INET, SOCK_STREAM, 0);
        u_long arg = 1;
        setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(u_long));
        if(m_fd < 0)
        {
           DBG("WARNING: ListenSocket constructor: could not create socket() %u (%s)\n", errno, strerror(errno));
        }
        m_connected = false;
        m_deleted = false;
        m_WillDisconnet=false;
    }

    void OnRead(size_t len)
    {
        if(!m_connected)
            return;

#ifdef WIN32
        int len2 = sizeof(sockaddr_in);
#else
        socklen_t len2 = sizeof(sockaddr_in);
#endif
       // usleep(20);
        new_fd = accept(m_fd, (sockaddr*)&new_peer, &len2);
        if(new_fd > 0)
        {
            T * s = new T(new_fd, &new_peer);
            s->Finalize();
            std::cout << "LISTEN SOCKET ONREAD CALL tcpsocket s->Finalize " <<new_fd<< "\n";
        }
    }

    void OnWrite(size_t len) {}
    void OnError(int err) {}
    void OnAccept(void * pointer) {}

    bool Open(const char * hostname, u_short port)
    {
        if(m_fd < 0)
        {
           DBG("No fd in listensocket\n");
            return false;
        }

        if(!strcmp(hostname, "0.0.0.0"))
            address.sin_addr.s_addr = htonl(INADDR_ANY);
        else
        {
            hostent * h = gethostbyname(hostname);
            if(!h)
            {
               DBG("Could not resolve listen address\n");
                return false;
            }

            memcpy(&address.sin_addr, h->h_addr_list[0], sizeof(in_addr));
        }

        address.sin_family = AF_INET;
        address.sin_port = ntohs(port);

        if(::bind(m_fd, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
        {
           DBG("Could not bind\n");
            return false;
        }

        if(listen(m_fd, 5) < 0)
        {
           DBG("Could not bind\n");
            return false;
        }

        // add to mgr
        m_connected = true;
        sSocketEngine.AddSocket(this);
        DBG("listen socket open done sSocketEngine.AddSocket this");
        return true;
    }

    bool Writable() { return false; }

    void Delete()
    {
        if(m_deleted) return;
        m_deleted = true;

        /* queue deletion */
        sSocketDeleter.Add(this);

        if(m_connected) Disconnect();
    }

    void Disconnect()
    {
        if(!m_connected) return;
        m_connected = false;

        DBG("ListenSocket disconnect");
        sSocketEngine.RemoveSocket(this);
        SocketOps::CloseSocket(m_fd);
        if(!m_deleted) Delete();
    }
};


/* Common Functions */
template<class T>
bool CreateListenSocket(const char * hostname, u_short port)
{
    ListenSocket<T> * s = new ListenSocket<T>();
    //std::cout<<"CreateListenSocket"<<"\n";
    if(!s->Open(hostname, port))
    {
        s->Delete();
        return false;
    }
    else
        return true;
}

#endif

