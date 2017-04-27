#include "MessageProtocols.hxx"

posUpMsg *getPosUpMsg(uint8_t*msg)
{
	posUpMsg *p = (posUpMsg*)malloc(sizeof(posUpMsg));
	if(p==NULL)
	{
		//log(LOGERROR, "%s", "malloc failed in getPosUpMsg");
		exit(-1);
	}

	memcpy(p, msg, sizeof(posUpMsg)); //this is hacky
}