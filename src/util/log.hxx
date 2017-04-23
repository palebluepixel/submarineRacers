#include <stdarg.h>

#define LOGDEFAULT 0

/* User must set this to the desired log level. Higher levels indicate
more printing. */
extern int loglevel_GLOBAL;

/*  priority: integer indicating at which log levels this should be printed.
Will print at any log level >= priority.
	format / args...: write as you would a printf(format, args...)
*/
#define log(priority, format, args...) \
			if(loglevel_GLOBAL >= priority) { \
				printf(format, ##args); \
			} while(0)