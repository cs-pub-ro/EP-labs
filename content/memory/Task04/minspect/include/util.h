#include <cstdio>   /* fprintf, srderr */
#include <cstdlib>  /* exit            */

#ifndef _UTIL_H
#define _UTIL_H

/* ANSI color codes */
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"
#define CLEAR   "\033[0m"

#define TOOL_TAG "[minspect]"

/* useful macros */
#define DIE(assertion, message)                                         \
	do {								                                \
		if (assertion) {					                            \
			fprintf(stderr, RED "[ERR]" GREEN " %s:%d " CLEAR "| %s\n", \
                __FILE__, __LINE__, message);                           \
            exit(-1);					                                \
		}							                                    \
	} while (0)

#endif