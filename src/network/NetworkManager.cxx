#include "NetworkManager.hxx"


NetworkManager::NetworkManager() {
    //Network setup things    
}

void NetworkManager::recieveMessage(char* message, int len) {
    char *current = message;

    while(current - message < len) {
        struct CommandHeader *command = (struct CommandHeader *)(current);

        if(current + sizeof(CommandHeader) + ntohs(command->len) - message >= len) {
            //TODO: error();
            return;
        }

        processCommand(ntohs(command->code), ntohs(command->len), current + sizeof(CommandHeader));

        current += sizeof(CommandHeader) + ntohs(command->len);
    }

    //TODO should this free message?
}

/* This is a wrapper for checkDispatch. Subclasses can replace this to
   checkDispatch with their own table. */
bool NetworkManager::processCommand(short code, short len, char *message) {
    return checkDispatch(code, len, message);
}

// Actually checks the dispatch table for a code. Returns true if found.
bool NetworkManager::checkDispatch(short code, short len, char *message) {
    int dispatch_size = sizeof(table) / sizeof(table[0]);
    for(int i=0; i<dispatch_size; i++) {
        if(code == table[i].code) {
            table[i].func(*this, len, message);
            return true;
        }
    }

    return false;
}

void NetworkManager::pingCommand(COMMAND_PARAMS) {
    sendCommand(CODE_PONG, 0, nullptr);
}

void NetworkManager::pongCommand(COMMAND_PARAMS) {}
void NetworkManager::initCommand(COMMAND_PARAMS) {}
