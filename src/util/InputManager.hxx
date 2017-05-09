#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <unordered_map>
#include <world/world.hxx>

#define KEYBOARDSIZE 350
#define MOUSESIZE 8

class World;


enum class GameKey {Rise, Fall, Left, Right, Forward, Reverse, Fire, Secondary, 
        Sonar, Weapon1, Weapon2, Weapon3, Pause, MenuUp, MenuDown, 
        MenuLeft, MenuRight, MenuCancel, MenuConfirm};

struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

enum class InputDevice : std::int8_t {MOUSE, KEYBOARD};

struct DeviceKey {
    InputDevice device;
    int key;
};


class InputManager {
public:
    int getKeyState(int key);
    int getMouseState(int button);
    int getGameKeyState(GameKey key);

    DeviceKey getKeyConfig(GameKey key);
    void setKeyConfig(GameKey gameKey, int keydbaordKey);
    void setKeyConfig(GameKey gameKey, int physicalKey, InputDevice device);
    int loadConfigFromFile(char *path);    //returns 0 on success, -1 on error.

    InputManager(GLFWwindow *window, World *world);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods, World *world);
    void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    void cursorpos_callback(GLFWwindow* window, double xpos, double ypos);
protected:
    int keyboard[KEYBOARDSIZE];
    int mouse[MOUSESIZE];
    double mousepos[2];

    std::unordered_map<GameKey, DeviceKey, EnumClassHash> config;

    void onKeyPress();
    void onKeyRelease();
    void onMousePress();
    void onMouseRelease();
};

#endif


///DO EVENT MANAGER REGISTRATION!!!!!!!!!!!!!!!!!!!!!