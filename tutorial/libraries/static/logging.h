#ifndef LOGGING_H
#define LOGGING_H

#include <stdarg.h>		// variable args
#include <stdio.h>
#include <string.h>

#ifdef __linux__
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#elif _WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#define _TR_	printf(" ::  %s : %s : %d \n", __FILENAME__, __func__,  __LINE__)

void print_log(int _auto_num_, ...);


#endif // LOGGING_H