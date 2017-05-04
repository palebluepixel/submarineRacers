#ifndef _LOG_HXX_
#define _LOG_HXX_

#include <stdarg.h>
#include <stdio.h>
#include <iostream>

#define LOGDEFAULT 0
#define LOGERROR 4
#define LOGHIGH 3
#define LOGMEDIUM 2
#define LOGLOW 1

/* User must set this to the desired log level. Higher levels indicate
more printing. */
extern int loglevel_GLOBAL;

/*  priority: integer indicating at which log levels this should be printed.
Will print at any log level <= priority.
	format / args...: write as you would a printf(format, args...)
*/
#define log(priority, format, args...) \
			if(loglevel_GLOBAL <= priority) { \
				printf(format, ##args); \
			} while(0)

#define logPrintBuf(priority, buf, len) \
			if(loglevel_GLOBAL <= priority) { \
				int ___BUFPRINT___; \
				for(___BUFPRINT___=0; ___BUFPRINT___<len; ___BUFPRINT___++) \
					printf("%x ", buf[___BUFPRINT___]); \
				printf("\n"); \
			} while(0)

#define logvec3(priority, v, s) \
			if(loglevel_GLOBAL <= priority) { \
				printf("%s %f %f %f\n", s, v[0], v[1], v[2]);  \
			} while(0)


#endif //!_LOG_HXX_