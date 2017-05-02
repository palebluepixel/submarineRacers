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
    //this->actuator = sub->getActuator();
}

void sendWorldDeltas(/*TODO: args?*/) {
    //TODO
}

void ServerNetworkManager::controllerStateCommand(short len, char* message) {
    //TODO
}