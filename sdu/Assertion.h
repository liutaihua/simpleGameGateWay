#pragma once

#include "Logging.h"

//#if defined(_DEBUG)


#define CnAssert(condition) \
do \
{ \
	if (!(condition)) \
	{ \
		LOG_E << "Assertion failed: " << #condition << std::endl \
			<< __FILE__ << ":" << __LINE__ << std::endl; \
	} \
} while (0)
#define CnVerify(f) CnAssert(f)


 //__CnAssert(false); \
//#else	// defined(_DEBUG)
//#define CnAssert(f) ((void)0)
//#define CnVerify(f) ((void)(f))
//#endif	// defined(_DEBUG)
