#include "NetworkManager.hxx"

//handler NetworkManager::table = { CODE_PING, &NetworkManager::pingCommand };
handler NetworkManager::table[3] = {{ CODE_PING, &NetworkManager::pingCommand }, 
                                    { CODE_PING, &NetworkManager::pongCommand }, 
                                    { CODE_INIT, &NetworkManager::initCommand }};

NetworkManager::NetworkManager() 
{}


void NetworkManager::sendMessage(char* message, int len)
{
    struct sockaddr_in targetAddr = this->getTargetAddr();
    int sock = this->getTargetSocket();

    log(LOGMEDIUM, "Sending message: %s (length: %d)\nTo: %s at port on socket %d\n", message, len,
        inet_ntoa(targetAddr.sin_addr), sock);
    int bytesSent = sendto(sock, message, len, 0, (struct sockaddr*)&targetAddr, sizeof(struct sockaddr_in));
    log(LOGMEDIUM, "Sent %d bytes\n", bytesSent);
}


void NetworkManager::recieveMessage(char* message, int len) {
    log(LOGMEDIUM, "recieved message: %s\n", message);

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

    // message freeing is handled by the calling function
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

void NetworkManager::sendCommand(short code, short len, char *message)
{
    return;
}

void NetworkManager::pingCommand(COMMAND_PARAMS) {
    this->sendCommand(CODE_PONG, 0, nullptr);
}

void NetworkManager::pongCommand(COMMAND_PARAMS) {}
void NetworkManager::initCommand(COMMAND_PARAMS) {}
