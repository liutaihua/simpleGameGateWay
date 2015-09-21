#pragma once

/*
 *	跨平台类型定义
 */
#ifndef SDTYPE_H
#define SDTYPE_H

#include "buildconfig.h"

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN		// reduce the size of the Windows header files 
#endif

#ifdef WIN
	#include <Windows.h>
	#include <BaseTsd.h>

	#ifdef WIN
		#pragma warning(disable:4996)	// suppress VS 2005 deprecated function warnings
		#pragma warning(disable:4786)	// for string
		#pragma warning(disable:4800)	// 将值强制为布尔值“true”或“false”(性能警告)
	#endif

#else // Linux

#endif

COMPILE_ASSERT(sizeof(char) == 1);

// One byte unsigned type
typedef unsigned char		byte;
COMPILE_ASSERT(sizeof(byte) == 1);

typedef unsigned char		byte;
COMPILE_ASSERT(sizeof(byte) == 1);

typedef unsigned char		uint8;
COMPILE_ASSERT(sizeof(uint8) == 1);

typedef signed short			int16;
COMPILE_ASSERT(sizeof(int16) == 2);

typedef unsigned short		uint16;
COMPILE_ASSERT(sizeof(uint16) == 2);

typedef signed int			int32;
COMPILE_ASSERT(sizeof(int32) == 4);

typedef unsigned int			uint32;
COMPILE_ASSERT(sizeof(uint32) == 4);

typedef signed long long		int64;
COMPILE_ASSERT(sizeof(int64) == 8);

typedef unsigned long long	uint64;
COMPILE_ASSERT(sizeof(uint64) == 8);


// The clong, ulong are supposed to with the same size of arch.
#ifdef WIN
	#ifdef BITS64
		typedef signed long long	llong;
	#else
		typedef signed long			llong;
	#endif
#else // long is with the same size of arch in Linux and Mac OS X.
	typedef signed long			llong;
#endif

#ifdef BITS64
COMPILE_ASSERT(sizeof(llong) == 8);
#else
COMPILE_ASSERT(sizeof(llong) == 4);
#endif


#ifdef WIN
	#ifdef BITS64
		typedef unsigned long long	ulong;
	#else
		typedef unsigned long	ulong;
	#endif
#else
	typedef unsigned long		ulong;
#endif

#ifdef BITS64
COMPILE_ASSERT(sizeof(ulong) == 8);
#else
COMPILE_ASSERT(sizeof(ulong) == 4);
#endif


#ifndef nullptr
	#include <cstddef>
#endif

#if __GNUC__ > 3 && __GNUC_MINOR__ < 6
#define nullptr 0
#endif

#ifdef WIN64
	#define SDFMT_I64         "%I64d"
	#define SDFMT_U64         "%I64u"
	#define SDFMT_X64         "%I64x"
#else
	#define SDFMT_I64         "%lld"
	#define SDFMT_U64         "%llu"
	#define SDFMT_64X         "%llx"
#endif


#ifdef WIN64
	#define SDFMT_SSIZE          "%I64d"
	#define SDFMT_SIZE           "%I64u"
#else
	#define SDFMT_SSIZE          "%d"
	#define SDFMT_SIZE           "%u"
#endif // WIN64


#ifdef WIN
	#define SDAPI __stdcall
	typedef HANDLE SDHANDLE ;
	#define SDINVALID_HANDLE  nullptr
#else
	#define SDAPI
	#define SDINVALID_HANDLE  0
	typedef ulong SDHANDLE; // 句柄应该跟系统字长绑定。
#endif

#ifdef WIN
inline void SDCloseHandle(SDHANDLE handle)
{
    CloseHandle(handle);
}
#else
#define SDCloseHandle(handle)
#endif

#ifdef WIN
#define CNTHREAD_LOCAL __declspec(thread)
#else
#define CNTHREAD_LOCAL __thread
#endif

#define SD_OK     0
#define SD_ERR    0xFFFFFFFF

#define SDINFINITE  0xFFFFFFFF

#endif // SDTYPE_H
