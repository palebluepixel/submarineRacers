
#include <glm/ext.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>

#include <util/log.hxx>
#include <error/error.hxx>
#include <util/file.hxx>
#include <graphics/shader.hxx>
#include <graphics/TetheredCamera.hxx>
#include <graphics/renderer.hxx>
#include <ent/Entity.hxx>
#include <ent/cube.hxx>
#include <ent/gadget.hxx>
#include <world/world.hxx>
#include <graphics/texture.hxx>
#include <network/Server.hxx>
#include <network/Client.hxx>
#include <network/MessageProtocols.hxx>
#include <cstring>

#define PORT 8008

/* Set this to -1 if you want to initate the game in the title-screen state 
and have to step throigh using client-server messages. Otherwise, for example
if you are testing something, set this to the desired test level. */
#define QUICKSTART 0

//defined in util/log.hxx
int loglevel_GLOBAL;


#define KEYBOARDSIZE 350
#define MOUSESIZE 8
int keyboard[KEYBOARDSIZE];
int mouse[MOUSESIZE];
double mousepos[2];

// there is only one world per instance of our program.
World* world;

static void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
    Error::error(std::string(description),0);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
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
static void mouse_callback(GLFWwindow* window, int button, int action, int mods){
    mouse[button] = action;
}
static void cursorpos_callback(GLFWwindow* window, double xpos, double ypos){
    // xpos and ypos are in pixels (not sure why they're doubles).
    mousepos[0] = xpos;
    mousepos[1] = ypos;
}



GLFWwindow *initializeGLFW(){
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        Error::error("glfwInit failed",1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 960, "Submarines!", NULL, NULL);
    if (!window){
        glfwTerminate();
        Error::error("glfwCreateWindow failed",1);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);
    // glfwSetKeyCallback (window, KeyCallback);
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
void initalizeWorld(int isServer, char *hostname){
    world = new World();
    world->window = initializeGLFW();

    world->setIsServer(isServer);
    if(world->isServer()){
        logln(LOGMEDIUM,"building server\n");
        world->setServer(new Server((short)PORT, NULL));
        world->handleEvent(STARTSERVER,0);
    } else {
        world->setClient(new Client(PORT, hostname));
        logln(LOGMEDIUM, "Hostname: %s, Port: %d\n", world->getClient()->getHost(), world->getClient()->getPort());
        world->handleEvent(STARTCLIENT,0);
    }

    world->worldInitalizeDefault(isServer);

    //associate the window with our world struct so window callbacks
    //can easily find the world state using glfwGetWindowUserPointer(window)
    glfwSetWindowUserPointer(world->window, world);

    //memzero the globally availible keyboard state
    for(int i=0;i<KEYBOARDSIZE;++i)keyboard[i]=0;
}


void update(double elapsed){
    using namespace std;
    // std::cout << elapsed << std::endl;

    View *view = world->view;    
    Camera *cam = view->activeCamera(); 

    double rSpeed = 2.0 * elapsed;
    double tSpeed = 10.0 * elapsed;

    // if(keyboard[GLFW_KEY_UP])          cam->rotateCamUpDown(-rSpeed);
    // if(keyboard[GLFW_KEY_DOWN])        cam->rotateCamUpDown(rSpeed);
    // if(keyboard[GLFW_KEY_LEFT])        cam->rotateCamLeftRight(rSpeed);
    // if(keyboard[GLFW_KEY_RIGHT])       cam->rotateCamLeftRight(-rSpeed);


    if(keyboard[GLFW_KEY_UP])          cam->addYPR(glm::vec3(0, rSpeed,0));
    if(keyboard[GLFW_KEY_DOWN])        cam->addYPR(glm::vec3(0, -rSpeed,0));
    if(keyboard[GLFW_KEY_LEFT])        cam->addYPR(glm::vec3(rSpeed, 0,0));
    if(keyboard[GLFW_KEY_RIGHT])       cam->addYPR(glm::vec3(-rSpeed, 0,0));

    // if(keyboard[GLFW_KEY_Q])          cam->rotateCamRoll(rSpeed);
    // if(keyboard[GLFW_KEY_E])          cam->rotateCamRoll(-rSpeed);
    
    // cout << keyboard[GLFW_KEY_UP] <<" , " << keyboard[GLFW_KEY_DOWN] << endl;

    if(keyboard[GLFW_KEY_W])           cam->translateCamViewAxis(tSpeed);
    if(keyboard[GLFW_KEY_S])           cam->translateCamViewAxis(-tSpeed);
    if(keyboard[GLFW_KEY_A])           cam->translateCamStrafeAxis(-tSpeed);
    if(keyboard[GLFW_KEY_D])           cam->translateCamStrafeAxis(tSpeed);
    if(keyboard[GLFW_KEY_R])           cam->translateCamUpAxis(tSpeed);
    if(keyboard[GLFW_KEY_F])           cam->translateCamUpAxis(-tSpeed);

    if(keyboard[GLFW_KEY_ESCAPE])      world->quit();
    if(keyboard[GLFW_KEY_Q])           world->quit();

}

int main(int argc, char*argv[]){

    // Set logging level
    loglevel_GLOBAL = LOGLOW;

    world = new World();
    bool isClient = (argc > 2) && argv[1][0]=='c';
    initalizeWorld(!isClient, isClient?argv[2]:0);

    /* Create levels */
    vector<const char*> levels;

    levels.push_back(0);
    levels.push_back((const char*)strdup("../assets/levels/level0.json"));
    levels.push_back((const char*)strdup("../assets/levels/level1.json"));

    world->addAllLevels(levels);

    int width, height;

    // for timing
    using namespace std::chrono;
    high_resolution_clock::time_point time_start = high_resolution_clock::now();
    high_resolution_clock::time_point time_prev = time_start;
    high_resolution_clock::time_point time_curr;
    duration<float, std::milli> time_span;
    duration<float, std::milli> time_total;
    float elapsed;

    /* USE THIS TO DISABLE MENUES FOR TESTING. SEE THE DEFINE ABOVE. */
    if(QUICKSTART>=0)
        world->quickSetup(QUICKSTART);

    while (!glfwWindowShouldClose(world->window)){

        // timing across update operations.
        time_curr  = high_resolution_clock::now();
        time_span  = time_curr - time_prev;
        time_total = time_curr - time_start;
        elapsed   = time_span.count() / 1000.0;

        time_prev = time_curr;

        update(elapsed);

        //window setup
        glfwGetFramebufferSize(world->window, &width, &height);
        glViewport(0, 0, width, height); //allows us to adjust window size
        //glClearColor(oceanColor[0], oceanColor[1], oceanColor[2], 1.0);
        glClearColor(1.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world->handleNetworksTick(time_total.count(),elapsed,20);
        world->handlePhysicsTick(time_total.count(), elapsed);
        world->handleGraphicsTick(time_total.count(),elapsed);
        

        glfwSwapBuffers(world->window);
        glfwPollEvents();
    }



    glfwDestroyWindow(world->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}