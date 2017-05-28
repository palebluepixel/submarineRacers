#include "MessageProtocols.hxx"
#include <ent/Entity.hxx>

message * createMessage(short code, short len, uint8_t *msg)
{
	message *m = (message*) malloc(sizeof(message));
	if(m==NULL)
	{
		exit(-1);
	}

	m->code = code;
	m->len = len;
	m->msg = msg;

	return m;
}

message *createMessageIntPayload(short code, int payload)
{
	int payloadcpy = payload;
	uint8_t *buf = (uint8_t *)malloc(sizeof(int));
	if(!buf) exit(-1);
	memcpy(buf, &payloadcpy, sizeof(int));

	return createMessage(code, sizeof(int), buf);
}

void deleteMessage(message *m)
{
	if(m->msg != NULL)
		free(m->msg);
	free(m);
}

/* Tell a client that a player p has finished the race and come in position n */
/* TODO: if I wanted to do this the long but more extensible way, I would make a
function that takes varargs and copies them all into a uint8_t buffer in order */
message *createPlayerFinishMessage(int p, int n)
{
	int pcopy = p;
	int ncopy = n;
	uint8_t *buf = (uint8_t*)malloc(sizeof(int)*2);
	if(!buf) exit(-1);
	memcpy(buf, &pcopy, sizeof(int));
	memcpy(buf+sizeof(int), &ncopy, sizeof(int));

	return createMessage(CODE_PLAYER_FINISH, sizeof(int)*2, buf);
}

posUpBuf *getPosUpBuf(uint8_t*msg)
{
	posUpBuf *p = (posUpBuf*)malloc(sizeof(posUpBuf));
	if(p==NULL)
	{
		//log(LOGERROR, "%s", "malloc failed in getPosUpMsg");
		exit(-1);
	}

	memcpy(p, msg, sizeof(posUpBuf)); //this is hacky

	return p;
}

/* Create a posUpMsg for the entity's current fields */
posUpBuf *createPosUpBuf(Entity *ent)
{
	posUpBuf *p = (posUpBuf*)malloc(sizeof(posUpBuf));
	if(p==NULL)
	{
		//log(LOGERROR, "%s", "malloc failed in getPosUpMsg");
		exit(-1);
	}

	p->id = ent->getID();
	p->pos = ent->getPosition();
	p->ori = ent->getOrientation();
	p->vel = ent->getVelocity();

	return p;
}

message *createPosUpMsg(Entity *ent)
{
	uint8_t * buf = (uint8_t *) createPosUpBuf(ent);
	return createMessage(CODE_OBJECT_CHANGE, sizeof(posUpBuf), buf);
}