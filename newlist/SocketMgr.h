
#ifndef SOCKETMGR_LINUX_H
#define SOCKETMGR_LINUX_H

#include "sdu.h"
#include "Common.h"
#include "SocketDefines.h"
#include "ThreadStarter.h"
#include "Singleton.h"


#define SOCKET_HOLDER_SIZE 30000    // You don't want this number to be too big, otherwise you're gonna be eating
                                    // memory. 65536 = 256KB, so thats no big issue for now, and I really can't
                                    // see anyone wanting to have more than 65536 concurrent connections.
#define THREAD_EVENT_SIZE 4096      // This is the number of socket events each thread can receieve at once.
                                    // This default value should be more than enough.

class TcpSocket;
class SocketWorkerThread;
class ListenSocketBase;


class SocketEngine : public Singleton<SocketEngine>
{
public:
    virtual ~SocketEngine() {}

    /** Adds a socket to the engine.
     */
    virtual void AddSocket(BaseSocket * s) = 0;

    /** Removes a socket from the engine. It should not receive any more events.
     */
    virtual void RemoveSocket(BaseSocket * s) = 0;

    /** This is called when a socket has data to write for the first time.
     */
    virtual void WantWrite(BaseSocket * s) = 0;

    /** Spawn however many worker threads this engine requires
     */
    virtual void SpawnThreads() = 0;

    /** Shutdown the socket engine, disconnect any associated sockets and
     * deletes itself and the socket deleter.
     */
    virtual void Shutdown() = 0;

    /** Called by SocketWorkerThread, this is the network loop.
     */
    virtual void RunLoop() = 0;
};

class SocketEngineThread : public ThreadBase
{
    SocketEngine * se;
public:
    SocketEngineThread(SocketEngine * s) : se(s) {}
    bool run()
    {
        se->RunLoop();
        return true;
    }
};

class SocketDeleter : public Singleton<SocketDeleter>
{
    typedef map<BaseSocket*, time_t> SocketDeleteMap;
    SocketDeleteMap _map;
    Mutex _lock;
public:
    /** Call this every loop of your program to delete old sockets
     */
    void Update()
    {
        time_t ct = time(NULL);
        _lock.Acquire();
        SocketDeleteMap::iterator it, it2;
//        DBG("deleter map size: %d", _map.size());
        for(it = _map.begin(); it != _map.end();)
        {
            it2 = it++;
            if(it2->second <= ct)
            {
                DBG("deleter delte %s", it2->first);
                delete it2->first;
                _map.erase(it2);
            }
        }

        _lock.Release();
    }

    /** Add a socket for deletion in time.
     */
    void Add(BaseSocket * s)
    {
        _lock.Acquire();
        _map[s] = time(NULL) + 15;
        _lock.Release();
    }

    /** Delete all sockets in this socket deleter, regardless of time
     */
    void Kill()
    {
        SocketDeleteMap::iterator itr = _map.begin();
        for(; itr != _map.end(); ++itr)
            delete itr->first;

        _map.clear();
    }
};





#ifdef CONFIG_USE_KQUEUE

/** This is the maximum number of connections you will be able to hold at one time.
 * adjust it accordingly.
 */
#define MAX_DESCRIPTORS 4096

class kqueueEngine : public SocketEngine
{
    /** Created kqueue file descriptor
     */
    int kq;

    /** Thread running or not?
     */
    bool m_running;

    /** Binding for fd -> pointer
     */
    BaseSocket * fds[MAX_DESCRIPTORS];

public:
    kqueueEngine();
    ~kqueueEngine();

    /** Adds a socket to the engine.
     */
    void AddSocket(BaseSocket * s);

    /** Removes a socket from the engine. It should not receive any more events.
     */
    void RemoveSocket(BaseSocket * s);

    /** This is called when a socket has data to write for the first time.
     */
    void WantWrite(BaseSocket * s);

    /** Spawn however many worker threads this engine requires
     */
    void SpawnThreads();

    /** Called by SocketWorkerThread, this is the network loop.
     */
    void RunLoop();

    /** Shutdown the socket engine, disconnect any associated sockets and
     * deletes itself and the socket deleter.
     */
    void Shutdown();
};

/** Returns the socket engine
 */
inline void CreateSocketEngine() { new kqueueEngine; }

#endif		// NETLIB_KQUEUE


#ifdef CONFIG_USE_EPOLL

/** This is the maximum number of connections you will be able to hold at one time.
 * adjust it accordingly.
 */
#define MAX_DESCRIPTORS 4096

class epollEngine : public SocketEngine
{
    /** Created epoll file descriptor
     */
    int epoll_fd;

    /** Thread running or not?
     */
    bool m_running;

    /** Binding for fd -> pointer
     */
    BaseSocket * fds[MAX_DESCRIPTORS];

public:
    epollEngine();
    ~epollEngine();

    /** Adds a socket to the engine.
     */
    void AddSocket(BaseSocket * s);

    /** Removes a socket from the engine. It should not receive any more events.
     */
    void RemoveSocket(BaseSocket * s);

    /** This is called when a socket has data to write for the first time.
     */
    void WantWrite(BaseSocket * s);

    /** Spawn however many worker threads this engine requires
     */
    void SpawnThreads();

    /** Called by SocketWorkerThread, this is the network loop.
     */
    void RunLoop();

    /** Shutdown the socket engine, disconnect any associated sockets and
     * deletes itself and the socket deleter.
     */
    void Shutdown();
};

/** Returns the socket engine
 */
inline void CreateSocketEngine() { new epollEngine; }

#endif

#define sSocketEngine SocketEngine::getSingleton()
#define sSocketDeleter SocketDeleter::getSingleton()

#endif
