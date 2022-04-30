#ifndef DEFINES_H
#define DEFINES_H

	#include "cmake_config.h" 

	// shared libraries
	#ifdef __linux__
		#define EXPORT
	#elif _WIN32
		#define EXPORT   __declspec(dllexport)
	#endif

	// logging subsystem
	#ifdef mv_LOGGING_on
		#include "logging.h"
	#else
		#define _TR_
	#endif

	// last cache generating timestamp
	#ifdef mv_LAST_COMPILE
		#include "last_compile.h"
	#else
		const char* _LC_ = "_LC_ : no information";
	#endif

#endif