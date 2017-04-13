
#include <glm/ext.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <error/error.hxx>
#include <util/file.hxx>
#include <graphics/shader.hxx>
#include <graphics/camera.hxx>
#include <graphics/renderer.hxx>
#include <ent/Entity.hxx>
#include <ent/cube.hxx>
#include <world/world.hxx>
#include <userinput/callbacks.hxx>
#include <graphics/texture.hxx>

int keyboard[350];
int mouse[8];
double mousepos[2];

static void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
    Error::error(std::string(description),0);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    keyboard[key] = action;
}
static void mouse_callback(GLFWwindow* window, int button, int action, int mods){
    mouse[button] = action;
}
static void cursorpos_callback(GLFWwindow* window, double xpos, double ypos){
    // xpos and ypos are in pixels (not sure why they're doubles).
    mousepos[0] = xpos;
    mousepos[1] = ypos;
}


GLFWwindow *initalizeGLFW()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        Error::error("glfwInit failed",1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window){
        Error::error("glfwCreateWindow failed",1);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);
    glfwSetKeyCallback (window, KeyCallback);
    glfwMakeContextCurrent (window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glerr = glewInit();
    glGetError();    
    if (glerr != GLEW_OK){
        Error::error("GLEW init failed"+glerr, 1);
    }

    glfwSwapInterval(1);

    return window;
}


int main(void){

    World * world = new World();

    GLFWwindow *window = initalizeGLFW();
    world->window = window;

    //associate the window with our world struct so window callbacks
    //can easily find the world state using glfwGetWindowUserPointer(window)
    glfwSetWindowUserPointer(window, world);

    // set up shaders.
    Shader *shader = new Shader();
    shader->addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/lightshader.vert"));
    shader->addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/lightshader.frag"));
    shader->build();

    //create renderer for the given shader
    Renderer *r = new SunlightShadingRenderer(shader);  

    //initalize camera
    Camera *camera = new Camera();
    //position, look-at point, up-vector
    camera->init(vec3(6,9,4.20),vec3(-1,0,0),vec3(0,1,0)); //location, looking-at, up
    camera->setFOV(90.0);
    camera->setNearFar(0.1, 100.0);

    vec3 oceanColor = vec3(64,141,167) / 256.0;

    //create view
    View *view = new View(window);
    world->view = view;
    view->addCamera(camera);
    view->setFOV(90);
    view->setNear(0.1);
    view->setFar(100.0);
    view->setSunlight(vec3(0, 0.3, 0.9), vec3(0.9, 0.9, 0.9), vec3(0.1, 0.1, 0.1));
    view->setFog(1, oceanColor, 0.05f, 5.0);

    //create test object
    vec3 cubePos[] = {vec3(-1,2,0), vec3(-5, -15, -2)}; 
    int ncubes = 2, i;
    Cube * cubes[ncubes];
    for(i=0; i<ncubes; i++){
        cubes[i] = new Cube(cubePos[i], mat3(), 0, strdup("kyubey"), TYPE1, SPAWNED, 0.1f);
    }

    int width, height;

    while (!glfwWindowShouldClose(window)){
        //window setup
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height); //allows us to adjust window size
        glClearColor(oceanColor[0], oceanColor[1], oceanColor[2], 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        r->Enable ();

        for(i=0; i<ncubes; i++)
            r->Render(view, cubes[i]);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}