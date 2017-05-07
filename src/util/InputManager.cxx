#include "InputManager.hxx"


int InputManager::getKeyState(int key) {
    return keyboard[key];
}

int InputManager::getMouseState(int button) {
    return mouse[button];
}

int InputManager::getGameKeyState(GameKey key) {
    DeviceKey devKey = this->config[key];
    if(devKey.device == InputDevice::MOUSE) {
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
}

//int loadConfigFromFile(char *path);


static void static_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    World *world = ((World *)(glfwGetWindowUserPointer(window)));
    world->in_man->key_callback(window, key, scancode, action, mods, world);
}

static void static_mouse_callback(GLFWwindow* window, int button, int action, int mods){
    World *world = ((World *)(glfwGetWindowUserPointer(window)));
    world->in_man->mouse_callback(window, button, action, mods);
}

static void static_cursorpos_callback(GLFWwindow* window, double xpos, double ypos){
    World *world = ((World *)(glfwGetWindowUserPointer(window)));
    world->in_man->cursorpos_callback(window, xpos, ypos);
}



InputManager::InputManager(GLFWwindow *window, World *world) {
    glfwSetWindowUserPointer(window, world);
    glfwSetKeyCallback(window, static_key_callback);
    glfwSetMouseButtonCallback(window, static_mouse_callback);
    glfwSetCursorPosCallback(window, static_cursorpos_callback);
}

/*
void InputManager::onKeyPress();
void InputManager::onKeyRelease();
void InputManager::onMousePress();
void InputManager::onMouseRelease();*/



void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods, World* world){
    /* On-press or on-release actions should go here */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) 
        world->getView()->nextCamera();

    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && world->isClient())
        world->getClient()->loadLevel(0);

    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && world->isClient())
        world->getClient()->loadLevel(1);

    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        if(world->isClient())
            world->getClient()->exitLevel(); 
        else
            world->getServer()->exitLevel();   
    }

    /* Continous actions should go here */
    keyboard[key] = action;
}
void InputManager::mouse_callback(GLFWwindow* window, int button, int action, int mods){
    mouse[button] = action;
}
void InputManager::cursorpos_callback(GLFWwindow* window, double xpos, double ypos){
    // xpos and ypos are in pixels (not sure why they're doubles).
    mousepos[0] = xpos;
    mousepos[1] = ypos;
}