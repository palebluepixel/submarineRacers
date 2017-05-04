#include "ServerNetworkManager.hxx"

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
    printf("TEST12222222222 %p", sub);
    printf("TEST12222222222 %p", this->actuator);
}

void sendWorldDeltas(/*TODO: args?*/) {
    //TODO
}

