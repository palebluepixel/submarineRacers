#include "NetworkManager.hxx"

#include <world/world.hxx>
class World;
extern World* world; //global 


handler NetworkManager::table[4] = {{ CODE_PING,          &NetworkManager::pingCommand }, 
                                    { CODE_PONG,          &NetworkManager::pongCommand }, 
                                    { CODE_INIT,          &NetworkManager::initCommand },
                                    { CODE_OBJECT_CHANGE, &NetworkManager::objectChangeCommand }};

NetworkManager::NetworkManager() 
{}


void NetworkManager::sendMessage(uint8_t* message, int len)
{
    struct sockaddr_in targetAddr = this->getTargetAddr();
    int sock = this->getTargetSocket();

    logln(LOGMEDIUM, "\nSending message: (length: %d)\nTo: %s at port on socket %d\n", len,
        inet_ntoa(targetAddr.sin_addr), sock);
    logPrintBuf(LOGMEDIUM, message, len);
    int bytesSent = sendto(sock, message, len, 0, (struct sockaddr*)&targetAddr, sizeof(struct sockaddr_in));
    logln(LOGMEDIUM, "Sent %d bytes\n\n", bytesSent);
}


void NetworkManager::recieveMessage(uint8_t* message, int len) {
    logln(LOGMEDIUM, "\nreceived message: ");
    logPrintBuf(LOGMEDIUM, message, len);
    logln(LOGMEDIUM, "\n");
    
    uint8_t *current = message;

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
bool NetworkManager::processCommand(short code, short len, uint8_t *message) {
    logln(LOGLOW, "command: %d %d\n", code, len);

    return checkDispatch(code, len, message);
}

// Actually checks the dispatch table for a code. Returns true if found.
bool NetworkManager::checkDispatch(short code, short len, uint8_t *message) {
    int dispatch_size = sizeof(table) / sizeof(table[0]);

    for(int i=0; i<dispatch_size; i++) {
        if(code == table[i].code) {
            table[i].func(*this, len, message);
            return true;
        }
    }

    return false;
}

void NetworkManager::sendCommand(short code, short len, uint8_t *payload) {
    int total_len = sizeof(code) + sizeof(len) + len;
    uint8_t message[total_len];
    short *m_short = (short*)message;
    m_short[0] = htons(code);
    m_short[1] = htons(len);
    memcpy(message + 4, payload, len);

    sendMessage(message, total_len);
}

void NetworkManager::pingCommand(COMMAND_PARAMS) {
    logln(LOGLOW, "got ping\n");
    this->sendCommand(CODE_PONG, 0, nullptr);
}

void NetworkManager::pongCommand(COMMAND_PARAMS) {
    logln(LOGLOW, "got pong\n");
    //this->sendCommand(CODE_PING, 0, nullptr);
}
void NetworkManager::initCommand(COMMAND_PARAMS) {}


void NetworkManager::objectChangeCommand(COMMAND_PARAMS) 
{
    logln(LOGLOW,"rec obj change");
    posUpBuf *msg = getPosUpBuf(message);
    world->setEntData(msg);
    free(msg);

}
