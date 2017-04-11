#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <world/world.hxx>

#define ROTSPEED 0.1f
#define TROTSPEED 0.1f
#define ROLLSPEED 0.1f

void KeyCallback (GLFWwindow *win, int key, int scancode, int action, int mods);

#endif //_CALLBACKS_H_