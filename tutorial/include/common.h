#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include "cmake_config.h"

#ifdef __linux__
	#define EXPORT
#elif _WIN32
	#define EXPORT   __declspec(dllexport)
#endif


#endif // COMMON_H