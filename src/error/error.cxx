#include "error.hxx"


// Error::hi = 1;

Error::err::err(std::string mdesc, int mfatality){
	desc=mdesc;
	fatality=mfatality;
}
void Error::error(err e){
	error_log.push_back(e);
	if(exit_func && e.fatality >= kill_fatality){
		exit(exit_func());
	}
}
void Error::error(std::string mdesc, int mfatality){
	Error::error(Error::err(mdesc,mfatality));
}
void Error::print_errors(){
	fprintf(stderr,"Error Log:\n");
	for (err e : error_log){
		fprintf(stderr,"(%d) %s\n",e.fatality, e.desc.c_str());
	}
}
void Error::setKillFunc(std::function<int(void)> func){
	exit_func = func;
}


std::function<int(void)> Error::exit_func = []()
	{
		print_errors();
		return 0;
	};

int Error::kill_fatality = 1;
std::list<Error::err> Error::error_log;