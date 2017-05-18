#include "ClientNetworkManager.hxx"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern int keyboard[350];
extern int mouse[8];

ClientNetworkManager::ClientNetworkManager()
        : NetworkManager() {}

void ClientNetworkManager::sendControllerState() {
    ControllerState state;

    state.forwardKey = keyboard[GLFW_KEY_W];
    state.reverseKey = keyboard[GLFW_KEY_S];
    state.leftKey = keyboard[GLFW_KEY_A];
    state.rightKey = keyboard[GLFW_KEY_D];

    state.riseKey = keyboard[GLFW_KEY_SPACE];
    state.diveKey = keyboard[GLFW_KEY_LEFT_SHIFT];

    state.fireKey = keyboard[GLFW_KEY_Y];

    //TODO switchWeapon should probably be handled in central key manager, as we want most recent num key


    sendCommand(CODE_CONTROLLER, sizeof(ControllerState), (uint8_t *) &state);
}