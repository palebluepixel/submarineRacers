#include "NetworkManager.hxx"
#include <network/Server.hxx>

#include <world/world.hxx>
class World;
extern World* world; //global 


handler NetworkManager::table[7] = {{ CODE_PING,          &NetworkManager::pingCommand }, 
                                    { CODE_PONG,          &NetworkManager::pongCommand }, 
                                    { CODE_INIT,          &NetworkManager::initCommand },
                                    { CODE_OBJECT_CHANGE, &NetworkManager::objectChangeCommand },
                                    { CODE_LEVEL_SELECT,  &NetworkManager::levelSelectCommand },
                                    { CODE_LOAD_LEVEL,    &NetworkManager::levelLoadCommand },
                                    { CODE_LEVEL_LOADED,  &NetworkManager::levelLoadedCommand }};

NetworkManager::NetworkManager() 
{}


void NetworkManager::sendMessage(uint8_t* mes, int len)
{
    struct sockaddr_in targetAddr = this->getTargetAddr();
    int sock = this->getTargetSocket();

    logln(LOGMEDIUM, "\nSending message: (length: %d)\nTo: %s at port on socket %d\n", len,
        inet_ntoa(targetAddr.sin_addr), sock);
    logPrintBuf(LOGMEDIUM, mes, len);
    int bytesSent = sendto(sock, mes, len, 0, (struct sockaddr*)&targetAddr, sizeof(struct sockaddr_in));
    logln(LOGMEDIUM, "Sent %d bytes\n\n", bytesSent);
}


void NetworkManager::recieveMessage(uint8_t* mes, int len) {
    logln(LOGMEDIUM, "\nreceived message: ");
    logPrintBuf(LOGMEDIUM, mes, len);
    logln(LOGMEDIUM, "\n");
    
    uint8_t *current = mes;

    while(current - mes < len) {
        struct CommandHeader *command = (struct CommandHeader *)(current);

        if(current + sizeof(CommandHeader) + ntohs(command->len) - mes >= len) {
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
bool NetworkManager::processCommand(short code, short len, uint8_t *mes) {
    logln(LOGLOW, "command: %d %d\n", code, len);

    return checkDispatch(code, len, mes);
}

// Actually checks the dispatch table for a code. Returns true if found.
bool NetworkManager::checkDispatch(short code, short len, uint8_t *mes) {
    int dispatch_size = sizeof(table) / sizeof(table[0]);

    for(int i=0; i<dispatch_size; i++) {
        if(code == table[i].code) {
            table[i].func(*this, len, mes);
            return true;
        }
    }

    return false;
}

void NetworkManager::sendCommand(short code, short len, uint8_t *payload) {
    int total_len = sizeof(code) + sizeof(len) + len;
    uint8_t mes[total_len];
    short *m_short = (short*)mes;
    m_short[0] = htons(code);
    m_short[1] = htons(len);
    memcpy(mes + 4, payload, len);

    sendMessage(mes, total_len);
}

void NetworkManager::pingCommand(COMMAND_PARAMS) {
    logln(LOGLOW, "got ping\n");
    this->sendCommand(CODE_PONG, 0, nullptr);
}

void NetworkManager::pongCommand(COMMAND_PARAMS) {
    logln(LOGLOW, "got pong\n");
    //this->sendCommand(CODE_PING, 0, nullptr);
}

/* If we are a server, broadcast the selected level to all clients and begin
loading the level ourselves. If we are a client, ignore the message. */
void NetworkManager::levelSelectCommand(COMMAND_PARAMS)
{
    if(world->isServer()){
        int level = bufToInt(mes);
        world->getServer()->sendLoadLevel(level);
        world->handleEvent(LOADLEVEL, level);
    }
}

/* If we are a client, load the level and tell the server that we did so. If
we are a server, ignore the message. */
void NetworkManager::levelLoadCommand(COMMAND_PARAMS)
{
    if(world->isClient()){
        int level = bufToInt(mes);
        world->handleEvent(LOADLEVEL, level);
        message *msg = createLevelLoadedMsg(level);
        world->getClient()->messageServer(msg);
    }
}

/* If we are a server, mark that the sending client has loaded the level in
this ServerNetworkManager. Then check if all clients have loaded the level, 
and start the race if they have. If we are a client, ignore this message. */ 
void NetworkManager::levelLoadedCommand(COMMAND_PARAMS)
{
    if(world->isServer()) {
        int level = bufToInt(mes);
        ServerNetworkManager *this2 = (ServerNetworkManager*)this;
        this2->setLoadedLevel(1);
        this2->setWhichLevel(level);

        if(world->getServer()->clientsLoaded(level)){
            world->handleEvent(LEVELLOADEDBYALL, level);
            //tell clients the race is starting
        }
    }
}

/* This doesn't require any actual processing because just getting
the message from a new client means we will add them to our 
connection list. */
void NetworkManager::initCommand(COMMAND_PARAMS) 
{}


void NetworkManager::objectChangeCommand(COMMAND_PARAMS) 
{
    logln(LOGLOW,"rec obj change");
    posUpBuf *msg = getPosUpBuf(mes);
    world->setEntData(msg);
    free(msg);

}
