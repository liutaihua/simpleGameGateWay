

#include "Common.h"
#include "Network.h"
#include "ListenSocket.h"
#include "Singleton.h"

initialiseSingleton(SocketEngine);
initialiseSingleton(SocketDeleter);

#ifdef CONFIG_USE_KQUEUE


kqueueEngine::kqueueEngine()
{
    new SocketDeleter();

    kq = kqueue();
    assert(kq != -1);
    memset(this->fds, 0, sizeof(void*) * MAX_DESCRIPTORS);
    m_running = true;
}

kqueueEngine::~kqueueEngine()
{
    close(kq);
}

void kqueueEngine::AddSocket(BaseSocket * s)
{
    assert(fds[s->GetFd()] == 0);
    fds[s->GetFd()] = s;

    struct kevent ev;
    EV_SET(&ev, s->GetFd(), s->Writable() ? EVFILT_WRITE : EVFILT_READ, EV_ADD, 0, 0, NULL);
    DBG("AddSocket %d, %s", s->GetFd(), s);
    if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
       DBG("!! could not add kevent for fd %u\n", s->GetFd());
}

void kqueueEngine::RemoveSocket(BaseSocket * s)
{
    int a, b;
    assert(fds[s->GetFd()] == s);
    fds[s->GetFd()] = 0;

    struct kevent ke, k2;
    EV_SET(&ke, s->GetFd(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    EV_SET(&k2, s->GetFd(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    if((kevent(kq, &ke, 1, NULL, 0, NULL) < 0) && (kevent(kq, &k2, 1, NULL, 0, NULL) < 0))
       DBG("!! both kevent removals returned -1 for fd %u\n", s->GetFd());
}

void kqueueEngine::WantWrite(BaseSocket * s)
{
    struct kevent ev;
    EV_SET(&ev, s->GetFd(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
    if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
       DBG("!! could not modify kevent for fd %u\n", s->GetFd());
}

void kqueueEngine::RunLoop()
{
    const static int maxevents = MAX_DESCRIPTORS;
    timespec timeout;
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;
    struct kevent events[MAX_DESCRIPTORS];
    struct kevent ev;
    int nfds, i;
    BaseSocket * s;
    while(m_running)
    {
//        usleep(10);
//        DBG("loop....%d", sizeof(fds)/sizeof(*fds));
//        for(auto i : fds){
//            if (i)
//                DBG("loop in ... %s", i);
//        }

        nfds = kevent(kq, 0, 0, events, maxevents, &timeout);
        for(i = 0; i < nfds; ++i)
        {
            s = fds[events[i].ident];
            if(s == 0)
            {
               DBG("kqueue returned invalid fd %u\n", events[i].ident);
                continue;
            }

            if(events[i].flags & EV_EOF || events[i].flags & EV_ERROR)
            {
//                continue;
                DBG("enginesocket err event %d %d %d %d %s %s %s", events[i].flags, EV_EOF, EV_ERROR, events[i].ident, s, events[i].filter, events[i].data);
                s->OnError(events[i].fflags);
                continue;
            }

            if(events[i].filter == EVFILT_READ)
            {
                s->OnRead(0);
                if(s->Writable() && !s->m_writeLock)
                {
                    ++s->m_writeLock;
                    WantWrite(s);
                }
            }
            else if(events[i].filter == EVFILT_WRITE)
            {
                s->OnWrite(0);
                if(!s->Writable())
                {
                    --s->m_writeLock;
                    EV_SET(&ev, s->GetFd(), EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
                       DBG("!! could not modify kevent (to read) for fd %u\n", s->GetFd());
                }
                else
                {
                    EV_SET(&ev, s->GetFd(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
                    if(kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
                       DBG("!! could not modify kevent (to write) for fd %u\n", s->GetFd());
                }

            }
            else
            {
               DBG("Unknwon filter: %u Fflags: %u, fd: %u, flags: %u\n", events[i].filter, events[i].fflags, events[i].ident, events[i].flags);
            }
        }

//        for(i = 0; i < MAX_DESCRIPTORS; ++i)
//        {
//         s = fds[i];
//         if(s != 0 &&  s->m_WillDisconnet)
//         {
//           s->Disconnect();
//          // break;
//         }
//        }
    }
}

void kqueueEngine::Shutdown()
{
    m_running = false;
    for(int i = 0; i < MAX_DESCRIPTORS; ++i)
    {
        if(fds[i] != 0)
        {
            fds[i]->Delete();
        }
    }

    sSocketDeleter.Kill();
    delete SocketDeleter::getSingletonPtr();
    delete this;
}

void kqueueEngine::SpawnThreads()
{
    uint32 count = 1;
    for(uint32 i = 0; i < count; ++i)
        ThreadPool.ExecuteTask(new SocketEngineThread(this));
}

#endif

#ifdef CONFIG_USE_EPOLL

epollEngine::epollEngine()
{
    new SocketDeleter();
    epoll_fd = epoll_create(MAX_DESCRIPTORS);
    assert(epoll_fd != -1);
    memset(this->fds, 0, sizeof(void*) * MAX_DESCRIPTORS);
    m_running = true;
}

epollEngine::~epollEngine()
{
    close(epoll_fd);
}

void epollEngine::AddSocket(BaseSocket * s)
{
    assert(fds[s->GetFd()] == 0);
    fds[s->GetFd()] = s;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = s->GetFd();
    ev.events = (s->Writable()) ? EPOLLOUT : EPOLLIN;
   // ev.events |= EPOLLET;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, s->GetFd(), &ev);
}

void epollEngine::RemoveSocket(BaseSocket * s)
{
    assert(fds[s->GetFd()] == s);
    fds[s->GetFd()] = 0;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = s->GetFd();
    ev.events = (s->Writable()) ? EPOLLOUT : EPOLLIN;
    //ev.events |= EPOLLET;

    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, s->GetFd(), &ev);
}

void epollEngine::WantWrite(BaseSocket * s)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = s->GetFd();
    ev.events = EPOLLOUT;// | EPOLLET;

    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, s->GetFd(), &ev);
}

void epollEngine::RunLoop()
{
    const static int maxevents = MAX_DESCRIPTORS;
    struct epoll_event events[MAX_DESCRIPTORS];
    int nfds, i;
    BaseSocket * s;
    while(m_running)
    {
       // usleep(10);
        nfds = epoll_wait(epoll_fd, events, maxevents, 1000);
        for(i = 0; i < nfds; ++i)
        {
            s = fds[events[i].data.fd];
            if(s == 0)
            {
               DBG("epoll returned invalid fd %u\n", events[i].data.fd);
                continue;
            }

            if(events[i].events & EPOLLHUP || events[i].events & EPOLLERR)
            {
                s->OnError(errno);
            }
            else if(events[i].events & EPOLLIN)
            {
                s->OnRead(0);
                if(s->Writable() && !s->m_writeLock)
                {
                    ++s->m_writeLock;
                    WantWrite(s);
                }
            }
            else if(events[i].events & EPOLLOUT)
            {
                s->OnWrite(0);
                if(!s->Writable())
                {
                    /* change back to read state */
                    struct epoll_event ev;
                    memset(&ev, 0, sizeof(epoll_event));
                    ev.data.fd = s->GetFd();
                    ev.events = EPOLLIN ;//| EPOLLET;

                    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, s->GetFd(), &ev);
                    --s->m_writeLock;
                }
            }
        }

//        for(i = 0; i < MAX_DESCRIPTORS; ++i)
//        {
//         s = fds[i];
//         if(s != 0 &&  s->m_WillDisconnet)
//         {
//           s->Disconnect();
//           break;
//         }
//        }

    }
}

void epollEngine::Shutdown()
{
    m_running = false;
    for(int i = 0; i < MAX_DESCRIPTORS; ++i)
    {
        if(fds[i] != 0)
        {
            fds[i]->Delete();
        }
    }

    sSocketDeleter.Kill();
    delete SocketDeleter::getSingletonPtr();
    delete this;
}

void epollEngine::SpawnThreads()
{
    uint32 count = 1;
    for(uint32 i = 0; i < count; ++i)
        ThreadPool.ExecuteTask(new SocketEngineThread(this));
}



#endif

