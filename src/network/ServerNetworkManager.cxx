#include "ServerNetworkManager.hxx"
#include <util/log.hxx>

ServerNetworkManager::ServerNetworkManager(int id)
        : NetworkManager(/*other args*/) {
    this->id = id;
    this->loadedLevel=0;
    this->whichLevel=-1;
}

void ServerNetworkManager::bindToSub(Submarine *sub)
{
    this->sub = sub;
    this->actuator = (SubmarineActuator *)sub->getActuator();
    if(!actuator){
        logln(LOGERROR, "%s", "WARNING: sub actuator NULL in bindToSub!");
    }
    this->actuator->bindToClient(this);
}

void sendWorldDeltas(/*TODO: args?*/) {
    //TODO
}

