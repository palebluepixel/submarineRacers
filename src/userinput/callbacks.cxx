#include "callbacks.hxx"

void KeyCallback (GLFWwindow *win, int key, int scancode, int action, int mods)
{
    
    if(action == GLFW_RELEASE)
        return; //there's only one part of an A press

    World *world = (World*)glfwGetWindowUserPointer(win);
    
    View *view = world->view;
    
    Camera *cam = view->activeCamera(); 

    switch(key) {
        case GLFW_KEY_ESCAPE:
            world->quit();
            break;
        case GLFW_KEY_W:
            cam->rotateCamUpDown(-ROTSPEED);
            break;
        case GLFW_KEY_S:
            cam->rotateCamUpDown(ROTSPEED);
            break;
        case GLFW_KEY_A:
            cam->rotateCamLeftRight(ROTSPEED);
            break;
        case GLFW_KEY_D:
            cam->rotateCamLeftRight(-ROTSPEED);
            break;
        case GLFW_KEY_Q:
            cam->rotateCamRoll(ROLLSPEED);
            break;
        case GLFW_KEY_E:
            cam->rotateCamRoll(-ROLLSPEED);
            break;
        case GLFW_KEY_UP:
            cam->translateCamViewAxis(TROTSPEED);
            break;
        case GLFW_KEY_DOWN:
            cam->translateCamViewAxis(-TROTSPEED);
            break;
        default:
            return;
    }
}