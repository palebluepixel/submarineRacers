#include "ServerNetworkManager.hxx"

ServerNetworkManager::ServerNetworkManager(int id)
        : NetworkManager(/*other args*/) {
    this->id = id;
}

void ServerNetworkManager::bindToSub(Submarine *sub)
{
    this->sub = sub;
    this->actuator = (SubmarineActuator *)sub->getActuator();
    printf("TESTTSSTSTSTSTSTSTTSTST %p", this->actuator);
}

void sendWorldDeltas(/*TODO: args?*/) {
    //TODO
}

