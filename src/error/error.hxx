#include "stdio.h"

#ifndef ERROR_HXX
#define ERROR_HXX

#define KYEL  "\x1B[33m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

// associate verbosity with filename (but, because we don't have strcmp
// just yet, just look at the first letter of the filename).
//
// this is a bit of a hack; a better way of doing this would involve
// #defining a VERBOSE_PRECEDENCE flag in each of the associated files,
// and relying on that.

#define ERR(...) { \
	fprintf(stderr,KRED); \
	fprintf(stderr,"At %s: %d\n",__FILE__,__LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr,KNRM"\n"); \
}

#endif