#include <stdio.h>		/* fprintf, stderr */
#include <stdlib.h>		/* exit 		   */

#ifndef _UTIL_H
#define _UTIL_H

/* ANSI color escape codes */
#define RED 	"\033[31;1m"
#define GREEN	"\033[32;1m"
#define YELLOW 	"\033[33;1m"
#define BLUE	"\033[34;1m"
#define CLR		"\033[0m"

/* conditional messages */
#define DIE(assertion, message...)              \
	do {								        \
		if (assertion) {					    \
			fprintf(stderr, message);   		\
			exit(-1);					        \
		}							            \
	} while (0)

#endif