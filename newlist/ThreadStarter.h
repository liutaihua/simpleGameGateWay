

#ifndef _THREADING_STARTER_H
#define _THREADING_STARTER_H

class  ThreadBase
{
public:
	ThreadBase() {}
	virtual ~ThreadBase() {}
    virtual bool run()=0;
	virtual void OnShutdown() {}
	pthread_t THREAD_HANDLE;
};

#endif

