
#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

enum TimeVariables
{
	TIME_SECOND = 1,
	TIME_MINUTE = TIME_SECOND * 60,
	TIME_HOUR   = TIME_MINUTE * 60,
	TIME_DAY	= TIME_HOUR * 24,
	TIME_MONTH	= TIME_DAY * 30,
	TIME_YEAR	= TIME_MONTH * 12,
};

enum MsTimeVariables
{
	MSTIME_SECOND = 1000,
	MSTIME_MINUTE = MSTIME_SECOND * 60,
	MSTIME_HOUR   = MSTIME_MINUTE * 60,
	MSTIME_DAY	= MSTIME_HOUR * 24,
};

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include "Singleton.h"
#include "buildconfig.h"

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
 #include <winsock2.h>
 #include <ws2tcpip.h>
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>


#ifdef __linux__
 #define LINUX
#endif


#ifdef LINUX
  #define CONFIG_USE_EPOLL
#else
  #define CONFIG_USE_KQUEUE
#endif

//// current platform and compiler
//#define PLATFORM_WIN32 0
//#define PLATFORM_UNIX  1
//#define PLATFORM_APPLE 2

//#define UNIX_FLAVOUR_LINUX 1
//#define UNIX_FLAVOUR_BSD 2
//#define UNIX_FLAVOUR_OTHER 3
//#define UNIX_FLAVOUR_OSX 4

//#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
//#  define PLATFORM PLATFORM_WIN32
//#elif defined( __APPLE_CC__ )
//#  define PLATFORM PLATFORM_APPLE
//#else
//#  define PLATFORM PLATFORM_UNIX
//#endif


//#if PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
//#ifdef HAVE_DARWIN
//#define PLATFORM_TEXT "MacOSX"
//#define UNIX_FLAVOUR UNIX_FLAVOUR_OSX
//#else
//#ifdef USE_KQUEUE
//#define PLATFORM_TEXT "FreeBSD"
//#define UNIX_FLAVOUR UNIX_FLAVOUR_BSD
//#else
//#define PLATFORM_TEXT "Linux"
//#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX
//#endif
//#endif
//#endif

//#if PLATFORM == PLATFORM_WIN32
//#define PLATFORM_TEXT "Win32"
//#endif


//#if PLATFORM == PLATFORM_WIN32
//	#define ASYNC_NET
//#endif

//#ifdef USE_EPOLL
//	#define CONFIG_USE_EPOLL
//#endif

//#ifdef USE_KQUEUE
//	#define CONFIG_USE_KQUEUE
//#endif

//#ifdef USE_SELECT
//	#define CONFIG_USE_SELECT
//#endif

//#ifdef USE_POLL
//	#define CONFIG_USE_POLL
//#endif

#ifndef WIN32
#include <sys/timeb.h>
#endif

//uint32 now()
//{
//#ifdef WIN32
//	return GetTickCount();
//#else
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
//#endif
//}



#ifndef WIN32
#define Sleep(ms) usleep(1000*ms)
#endif

inline static unsigned int MakeIP(const char * str)
{
	unsigned int bytes[4];
	unsigned int res;
	if( sscanf(str, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]) != 4 )
		return 0;

	res = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
	return res;
}


#endif
