#include "stdio.h"
#include <string>
#include <list>
#include <functional>

#ifndef ERROR_HXX
#define ERROR_HXX

#define KYEL  "\x1B[33m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"



#define ERR(...) { \
	fprintf(stderr,KRED); \
	fprintf(stderr,"At %s: %d\n",__FILE__,__LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr,KNRM"\n"); \
}

namespace Error{
	class err{
	public:
		err(std::string mdesc, int mfatality);
		std::string desc;
		int fatality;
	};

	extern int kill_fatality;
	extern std::function<int(void)> exit_func;
	extern std::list<err> error_log;

	extern void error(err e);
	extern void error(std::string mdesc, int mfatality);
	extern void print_errors();
	extern void setKillFunc(std::function<int(void)> func);
};


#endif