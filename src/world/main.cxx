
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

//defined in util/log.hxx
int loglevel_GLOBAL;

int keyboard[350];
int mouse[8];
double mousepos[2];

// there is only one world per instance of our program.
World* world;

static void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
    Error::error(std::string(description),0);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    /* On-press or on-release actiosn should go here */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) 
            world->getView()->nextCamera();

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


void update(double elapsed){
    using namespace std;
    // std::cout << elapsed << std::endl;

    View *view = world->getView();    
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
void init(){
    world = new World();
    world->window = initializeGLFW();

    //associate the window with our world struct so window callbacks
    //can easily find the world state using glfwGetWindowUserPointer(window)
    glfwSetWindowUserPointer(world->window, world);

    for(int i=0;i<350;++i)keyboard[i]=0;
}



int main(int argc, char*argv[]){

    // Set logging level
    loglevel_GLOBAL = LOGLOW;

    world = new World();

    init();

    //set up server or client: eventually we will make command line flags more advanced
    //First arg is "s" for server or "c" for client
    //Second arg is server hostname is we are a client
    world->setIsServer(argv[1][0] == 's');
    if(world->isServer()){
        world->setServer(new Server((short)PORT, NULL));
        world->getServer()->initListeningSocket();
    } else {
        world->setClient(new Client(PORT, argv[2]));
        log(LOGMEDIUM, "Hostname: %s, Port: %d\n", world->getClient()->getHost(), world->getClient()->getPort());
        world->getClient()->connectServer();
    }

    // set up shaders.
    Shader *shader = new Shader();
    shader->addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/lightshader.vert"));
    shader->addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/lightshader.frag"));
    shader->build();

    Shader *skyboxShader = new Shader();
    skyboxShader->addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/skyboxshader.vert"));
    skyboxShader->addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/skyboxshader.frag"));
    skyboxShader->build();

    //create renderer for the given shader
    Renderer *r = new UnderwaterRenderer(shader);  
    Renderer *rsky = new SkyboxRenderer(skyboxShader);

    //initalize camera
    Camera *camera = new Camera();
    
    //position, yaw-roll, up-vector
    camera->init(vec3(-2,0,-2),vec3(0.5,0,0),vec3(0,1,0)); //location, looking-at, up
    camera->setFOV(90.0);
    camera->setNearFar(0.1, 1000.0);

    vec3 oceanColor = vec3(0,70,95) / 256.0;
    vec3 oceanBrightColor = vec3(70,241,245) / 256.0;

    //create view
    View *view = new View(world->window);
    view->setSunlight(vec3(-0.3, -1.0, 0), vec3(0.9, 0.9, 0.9), vec3(0.1, 0.1, 0.1));
    view->setFog(0, oceanColor, 0.05f, 5.0);
    view->setColoring(1, vec3(1,1,1), vec3(0.2,0.2,0.2), oceanBrightColor, oceanColor,
        0.03f, -5.0f, -30.0f);

    Level *level = new Level();
    level->buildLevelFromFile();

    TetheredCamera * camTeth = new TetheredCamera(FIRSTPERSON, level->getEntityByID(0), vec3(4,7,0));

    view->addCamera(camera);
    view->addCamera(camTeth);

    //create skybox
    Gadget *skybox = new Gadget(vec3(0,0,0), quaternion(), strdup("sky"), TYPE1, SPAWNED, 0.1f, vec3(1,1,1), "../assets/models/sphere.obj");
    level->setSkybox(skybox);

    world->setLevel(level);
    world->setView(view);
    world->setEntityRenderer(r);
    world->setSkyboxRenderer(rsky);

    int width, height;

    // for timing
    using namespace std::chrono;
    high_resolution_clock::time_point time_prev = high_resolution_clock::now();
    high_resolution_clock::time_point time_curr;
    duration<double, std::milli> time_span;
    double elapsed;


    while (!glfwWindowShouldClose(world->window)){

        // timing across update operations.
        time_curr = high_resolution_clock::now();
        time_span = time_curr - time_prev;
        elapsed   = time_span.count() / 1000.0;
        update(elapsed);

        time_prev = time_curr;

        if(world->isServer()){
            //quick hack-in of a cube movement animation
            Entity *c = world->getLevel()->getEntityByID(0);
            vec3 pos = c->getPosition() - vec3(0,0.02,0);
            c->setPosition(pos);
        }

        //window setup
        glfwGetFramebufferSize(world->window, &width, &height);
        glViewport(0, 0, width, height); //allows us to adjust window size
        //glClearColor(oceanColor[0], oceanColor[1], oceanColor[2], 1.0);
        glClearColor(1.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        printf("1\n");
        world->getLevel()->updateLevel(elapsed);
        printf("2\n");
        world->handleNetworksTick(0,0,20);
        printf("3\n");
        world->handleGraphicsTick(0,0);
        printf("4\n");

        glfwSwapBuffers(world->window);
        glfwPollEvents();
    }



    glfwDestroyWindow(world->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}