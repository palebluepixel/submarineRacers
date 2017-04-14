#include "ServerNetworkManager.hxx"


ServerNetworkManager::ServerNetworkManager(Submarine *player/*other args*/)
        : NetworkManager(/*other args*/) {
    sub = player;
    actuator = player->getActuator();
}

void sendWorldDeltas(/*TODO: args?*/) {
    //TODO
}

void ServerNetworkManager::controllerStateCommand() {
    //TODO
}