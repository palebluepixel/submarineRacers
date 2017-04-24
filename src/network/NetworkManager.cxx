#include "NetworkManager.hxx"


NetworkManager::NetworkManager() {
    //Network setup things    
}

void NetworkManager::recieveMessage(char* message, int len) {
    /*struct CommandHeader *command = (struct CommandHeader *)(current);

    for(char *current = message; current - message < len; current += sizeof(CommandHeader) + ntohs(command->len)) {

        if(current + sizeof(CommandHeader) + ntohs(command->len) - message >= len) {
            //TODO: error();
            return;
        }

        processCommand(ntohs(command->code), ntohs(command->len), current + sizeof(CommandHeader));
    }

    free(message);*/

    log(LOGMEDIUM, "recieved message: %s\n", message);
}

void NetworkManager::processCommand(short code, short len, char* message) {
    /*//TODO switch to map
    switch(code) {
        case CODE_PING:
            pingCommand(len, message);
            break;
        case CODE_CONTROLLER:
            controllerCommand(len, message);
            break;
        default:
            break;
    }*/
}

void NetworkManager::pingCommand(short len, char* message) {
    //sendCommand(CODE_PONG, 0, NULL);
}

//TODO make NetworkManager, subclass this and client
