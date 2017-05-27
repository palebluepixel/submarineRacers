#include "NetworkManager.hxx"
#include <network/Server.hxx>

#include <world/world.hxx>
class World;
extern World* world; //global 

handler NetworkManager::table[12] = {{ CODE_PING,         &NetworkManager::pingCommand }, 
                                    { CODE_PONG,          &NetworkManager::pongCommand }, 
                                    { CODE_INIT,          &NetworkManager::initCommand },
                                    { CODE_OBJECT_CHANGE, &NetworkManager::objectChangeCommand },
                                    { CODE_CONTROLLER,    &NetworkManager::controllerStateCommand },
                                    { CODE_LEVEL_SELECT,  &NetworkManager::levelSelectCommand },
                                    { CODE_LOAD_LEVEL,    &NetworkManager::levelLoadCommand },
                                    { CODE_LEVEL_LOADED,  &NetworkManager::levelLoadedCommand },
                                    { CODE_EXIT_LEVEL,    &NetworkManager::exitLevelCommand },
                                    { CODE_LEVEL_START,   &NetworkManager::startLevelCommand },
                                    { CODE_CHECK_CLEAR,   &NetworkManager::checkClearCommand },
                                    { CODE_LAP_CLEAR,     &NetworkManager::lapClearCommand }};

NetworkManager::NetworkManager() 
{}


void NetworkManager::sendMessage(uint8_t* mes, int len)
{
    struct sockaddr_in targetAddr = this->getTargetAddr();
    int sock = this->getTargetSocket();

    /*log(LOGMEDIUM, "\nSending message: (length: %d)\nTo: %s at port on socket %d\n", len,
        inet_ntoa(targetAddr.sin_addr), sock);
    logPrintBuf(LOGMEDIUM, message, len);*/
    int bytesSent = sendto(sock, mes, len, 0, (struct sockaddr*)&targetAddr, sizeof(struct sockaddr_in));
    //log(LOGMEDIUM, "Sent %d bytes\n\n", bytesSent);
}


void NetworkManager::recieveMessage(uint8_t* mes, int len) {
    /*logln(LOGMEDIUM, "\nreceived message: ");
    logPrintBuf(LOGMEDIUM, mes, len);
    logln(LOGMEDIUM, "\n");*/
    
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
    //logln(LOGLOW, "command: %d %d\n", code, len);

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
            message *msg = createStartLevelMsg();
            world->getServer()->broadcast(msg);
        }
    }
}

/* RACERS START UR ENGYNES */
void NetworkManager::startLevelCommand(COMMAND_PARAMS)
{
    world->handleEvent(LEVELLOADEDBYALL, 0);
}

/* If we are a server, exit the current level and tell all clients
to do the same. If we are a client, exit the current level. */
void NetworkManager::exitLevelCommand(COMMAND_PARAMS)
{
    world->handleEvent(EXIT, 0);
    if(world->isServer())
        world->getServer()->exitLevel();
}

/* This doesn't require any actual processing because just getting
the message from a new client means we will add them to our 
connection list. */
void NetworkManager::initCommand(COMMAND_PARAMS) 
{}


void NetworkManager::objectChangeCommand(COMMAND_PARAMS) 
{
    posUpBuf *msg = getPosUpBuf(mes);
    world->setEntData(msg);
    free(msg);

}

void NetworkManager::controllerStateCommand(COMMAND_PARAMS) {
    if(world->isServer()){
        SubmarineActuator *actuator = ((ServerNetworkManager*)this)->actuator;

        ControllerState state;
        memcpy(&state, mes, len);
        //state->moude = ntoh(state->mouse)

        if(state.switchWeaponsKey != 0) {
            actuator->switchWeapons(state.switchWeaponsKey);
        }

        if(state.riseKey) {
            actuator->rise(1);
        }
        if(state.diveKey) {
            actuator->dive(1);
        }

        if(state.leftKey && !state.rightKey) {
            actuator->turnLeft(1);
        }
        if(state.rightKey && !state.leftKey) {
            actuator->turnRight(1);
        }

        if(state.forwardKey) {
            actuator->accelerate(3);
        }
        if(state.fireKey) {
            actuator->fire();
        }
    }
}


/* Track progress visualization */

/* TODO: a good abstraction to have here would be to give the world (or level 
or track) a clearCheck and clearLevel function that would be called by these
handlers, so that checkpoint and finishline clearing behavior wouldn't assume
a client-server connection. */

/* If we are a client, update the visual information for the given checkpoint */
void NetworkManager::checkClearCommand(COMMAND_PARAMS)
{
    if(world->isClient()){

        Track * track = world->getLevel()->getTrack();
        int id = bufToInt(mes); //get int from message
        track->clearCheckVis(id);
    }
}

/* If we are a client,update the visual information for all checkpoints */
void NetworkManager::lapClearCommand(COMMAND_PARAMS)
{
    if(world->isClient()){

        Track * track = world->getLevel()->getTrack();
        track->resetAllChecksVis();
    }

}