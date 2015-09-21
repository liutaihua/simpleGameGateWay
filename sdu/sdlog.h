#pragma once

/*
 *	统一log接口
 */
#ifndef sdlog_h__
#define sdlog_h__

#ifdef WIN
void WinLogger_CRITICAL(const char* format, ...);
void WinLogger_WARN(const char* format, ...);
void WinLogger_INFO(const char* format, ...);
void WinLogger_DBG(const char* format, ...);
void WinLogger_PY(const char* pylog, bool isErr = false);

#define CRITICAL	WinLogger_CRITICAL
#define WARN		WinLogger_WARN
#define INFO		WinLogger_INFO
#define DBG		    WinLogger_DBG
#define PY          WinLogger_PY

#else

#include <stdarg.h>
#include "Logging.h"

// just tempporary for linux
void LinuxLogger_CRITICAL(const char* format, ...);
void LinuxLogger_WARN(const char* format, ...);
void LinuxLogger_INFO(const char* format, ...);
void LinuxLogger_DBG(const char* format, ...);
void LinuxLogger_CDBG(const char* format, ...);

// Comment out the macro which does not exist in windows.
#define DBG		    Logger::D
#define INFO		Logger::I
#define WARN		Logger::W
#define CRITICAL	Logger::E
#define PY			Logger::PY
#define CDBG        Logger::C

#endif


#endif // sdlog_h__
