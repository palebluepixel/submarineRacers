#include "InputManager.hxx"

/*
int InputManager::getKeyState(int key) {
    return keyboard[key];
}

int InputManager::getMouseState(int button) {
    return mouse[button];
}

int InputManager::getGameKeyState(GameKey key) {
    DeviceKey devKey = this->config[key];
    if(devKey.device == MOUSE) {
        return getMouseState(devKey.key);
    } else {
        return getKeyState(devKey.key);
    }
}

DeviceKey InputManager::getKeyConfig(GameKey key) {
    return this->config[key];
}

void InputManager::setKeyConfig(GameKey gameKey, int keydbaordKey) {
    setKeyConfig(gameKey, keydbaordKey, InputDevice::KEYBOARD);
}

void InputManager::setKeyConfig(GameKey gameKey, int physicalKey, InputDevice device) {
    DeviceKey dk;
    dk.device = device;
    dk.key = physicalKey;
    this->config[gameKey] = dk;
}*/

//int loadConfigFromFile(char *path);

InputManager::InputManager(/*args*/) {
    //TODO
}
/*
void InputManager::onKeyPress();
void InputManager::onKeyRelease();
void InputManager::onMousePress();
void InputManager::onMouseRelease();*/