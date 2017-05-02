#include "ServerNetworkManager.hxx"

server_handler ServerNetworkManager::table[1] = {{ CODE_CONTROLLER, &ServerNetworkManager::controllerStateCommand }};

ServerNetworkManager::ServerNetworkManager(int id)
        : NetworkManager(/*other args*/) {
    this->id = id;
}

void ServerNetworkManager::bindToSub(Submarine *sub)
{
    this->sub = sub;
    this->actuator = new SubmarineActuator(sub);
}

void sendWorldDeltas(/*TODO: args?*/) {
    //TODO
}

void ServerNetworkManager::controllerStateCommand(COMMAND_PARAMS) {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    ControllerState state;
    memcpy(&state, message, len);
    //state->moude = ntoh(state->mouse)

    if(state.switchWeaponsKey != 0) {
        actuator->switchWeapons(state.switchWeaponsKey);
    }

    if(state.riseKey) {
        actuator->rise();
    }
    if(state.diveKey) {
        actuator->dive();
    }

    if(state.leftKey && !state.rightKey) {
        actuator->turnLeft();
    }
    if(state.rightKey && !state.leftKey) {
        actuator->turnRight();
    }

    if(state.forwardKey) {
        actuator->accelerate();
    }
    if(state.fireKey) {
        actuator->fire();
    }
}

// First checks dispatch table in superclass, then in this class.
bool ServerNetworkManager::processCommand(short code, short len, uint8_t *message) {
    if(NetworkManager::processCommand(code, len, message)) {
        return true;
    }
    return checkDispatch(code, len, message);
}