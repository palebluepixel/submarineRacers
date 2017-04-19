#include "ServerNetworkManager.hxx"


ServerNetworkManager::ServerNetworkManager(int id, int socketAddr)
        : NetworkManager(/*other args*/) {
    this->id = id;
    this->socketAddr = socketAddr;
}

void ServerNetworkManager::bindToSub(Submarine *sub)
{
    this->sub = sub;
    this->actuator = sub->Actuator();
}

void sendWorldDeltas(/*TODO: args?*/) {
    //TODO
}

void ServerNetworkManager::controllerStateCommand() {
    //TODO
}