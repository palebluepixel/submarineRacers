#include "Racer.hxx"
#include <ent/Submarine.hxx>
#include <ai/AI.hxx>
#include <util/log.hxx>

void Racer::initRacer()
{
    static int IDNUM = -1;
    this->id = ++IDNUM;
    this->sub = NULL;
}

Racer::Racer(ServerNetworkManager *client)
{
    this->initRacer();
    this->type = HUMAN_CONTROLLED;
    this->client = client;
    this->ai = NULL;

    /* For clients, the server creates the racer and 
    automatically binds the ServerNetworkManager to us*/
}

Racer::Racer(SubmarineAI *ai)
{
    this->initRacer();
    this->type = AI_CONTROLLED;
    this->ai = ai;
    this->client = NULL;

    /* For AI, we have to tell the AI to bind to us. */
    ai->bindToRacer(this);
}

void Racer::bindToSub(Submarine *sub)
{
    if(!sub){
        logln(LOGERROR, "Error: null sub in bindToSub, racer %d", this->getID());
        return;
    }
    this->sub = sub;
    this->sub->bindToRacer(this);
}

void Racer::unbindFromSub()
{
    if(!this->sub)
        return;
    this->sub->unbindFromRacer();
    this->sub = NULL;
}

