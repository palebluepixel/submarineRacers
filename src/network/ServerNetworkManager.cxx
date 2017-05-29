#include "ServerNetworkManager.hxx"
#include <util/log.hxx>

ServerNetworkManager::ServerNetworkManager()
        : NetworkManager(/*other args*/) {
    this->loadedLevel=0;
    this->whichLevel=-1;

    /* Create associated racer for the human */
    this->racer = new Racer(this);
}

/*void ServerNetworkManager::bindToSub(Submarine *sub)
{
    this->sub = sub;
    this->actuator = (SubmarineActuator *)sub->getActuator();
    if(!actuator){
        logln(LOGERROR, "%s", "WARNING: sub actuator NULL in bindToSub!");
    }
    this->actuator->bindToClient(this);
}

void ServerNetworkManager::unbindFromSub()
{
    this->actuator->unbindFromClient();
    this->actuator = NULL;
    this->sub = NULL;
}*/

void ServerNetworkManager::bindToRacer(Racer *racer)
{
    this->racer = racer;
}

void ServerNetworkManager::unbindFromRacer()
{
    this->racer = NULL;
}

Submarine *ServerNetworkManager::getSubmarine()
{
    if(!this->racer) return NULL;
    return racer->getSub();
}

SubmarineActuator *ServerNetworkManager::getActuator()
{
    if(!this->racer || !this->racer->isBound()) return NULL;
    return (SubmarineActuator*)racer->getSub()->getActuator();
}


