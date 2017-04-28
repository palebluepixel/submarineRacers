#include "ClientNetworkManager.hxx"


handler ClientNetworkManager::table[0] = { /*{ CODE_CONTROLLER, &ServerNetworkManager::pingCommand }*/};

ClientNetworkManager::ClientNetworkManager(int id)
        : NetworkManager() {}

// First checks dispatch table in superclass, then in this class.
bool ClientNetworkManager::processCommand(short code, short len, uint8_t *message) {
    if(NetworkManager::processCommand(code, len, message)) {
        return true;
    }
    return checkDispatch(code, len, message);
}

void ClientNetworkManager::sendControllerState() {
    ControllerState state;
/*
    //preliminary
    //TODO make this configurable
    //TODO worry about sticky keys, maybe have all keypresses go through central location that handles pause, menu, etc as well.
    state.forwardKey = keyboard[GLFW_KEY_W];
    state.reverseKey = keyboard[GLFW_KEY_S];
    state.leftKey = keyboard[GLFW_KEY_A];
    state.rightKey = keyboard[GLFW_KEY_D];

    state.riseKey = keyboard[GLFW_KEY_SPACE];
    state.diveKey = keyboard[GLFW_KEY_SHIFT];

    //TODO switchWeapon should probably be handled in central key manager, as we want most recent num key
*/

    sendCommand(CODE_CONTROLLER, sizeof(ControllerState), (uint8_t *) &state);
}