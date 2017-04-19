#include "ServerNetworkManager.hxx"


ServerNetworkManager::ServerNetworkManager(int id, int socketAddr)
        : NetworkManager(/*other args*/) {
    this->id = id;
    this->socketAddr = socketAddr;
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