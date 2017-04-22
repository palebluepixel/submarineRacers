
#include <glm/ext.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>

#include <error/error.hxx>
#include <util/file.hxx>
#include <graphics/shader.hxx>
#include <graphics/camera.hxx>
#include <graphics/renderer.hxx>
#include <ent/Entity.hxx>
#include <ent/cube.hxx>
#include <ent/gadget.hxx>
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


// there is only one world per instance of our program.
World* world;

GLFWwindow *initializeGLFW(){
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        Error::error("glfwInit failed",1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Submarines!", NULL, NULL);
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
void init(){
    world = new World();
    world->window = initializeGLFW();

    //associate the window with our world struct so window callbacks
    //can easily find the world state using glfwGetWindowUserPointer(window)
    glfwSetWindowUserPointer(world->window, world);

    for(int i=0;i<350;++i)keyboard[i]=0;
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

    if(keyboard[GLFW_KEY_ESCAPE])       world->quit();

}

int main(void){

    world = new World();

    init();

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
    
    //position, look-at point, up-vector
    camera->init(vec3(-2,0,-2),vec3(3,0,3),vec3(0,1,0)); //location, looking-at, up
    camera->setFOV(90.0);
    camera->setNearFar(0.1, 1000.0);

    vec3 oceanColor = vec3(0,70,95) / 256.0;
    vec3 oceanBrightColor = vec3(141,241,245) / 256.0;

    //create view
    View *view = new View(world->window);
    world->view = view;
    view->addCamera(camera);
    view->setFOV(90);
    view->setNear(0.1);
    view->setFar(1000.0);
    view->setSunlight(vec3(-0.3, 1.0, 0), vec3(0.9, 0.9, 0.9), vec3(0.1, 0.1, 0.1));
    view->setFog(1, oceanColor, 0.05f, 5.0);
    view->setColoring(1, vec3(1,1,1), vec3(0.2,0.2,0.2), oceanBrightColor, oceanColor,
        0.03f, -5.0f, -30.0f);

    //create test objects
    vec3 cubePos[] = {vec3(1,5,10), vec3(5, 0, 5), vec3(5, -5, 5), vec3(5, -10, 5), vec3(5, -20, 5),
        vec3(5, -40, 5)}; 
    vec3 cubeColor[] = {vec3(1,1,1), vec3(1,1,1), vec3(1,1,0), vec3(1,0,1), vec3(0,1,1), vec3(0,0,1)};
    int ncubes = 6, i;
    Entity * cubes[ncubes];
    cubes[0] = new Gadget(cubePos[0], quaternion(), 0, strdup("kyubey"), TYPE1, SPAWNED, 0.1f, cubeColor[0], "../assets/models/bigmonkey.obj");
    cubes[0]->volume = new Space::SphereVolume(vec3(0,0,0),2.f);
    cubes[0]->meshes.push_back(cubes[0]->volume->collisionMesh());
    for(i=1; i<ncubes; i++){
        cubes[i] = new Gadget(cubePos[i], quaternion(), 0, strdup("kyubey"), TYPE1, SPAWNED, 0.1f, cubeColor[i], "../assets/models/cube.obj");
        cubes[i]->volume = new Space::SphereVolume(vec3(0,0,0),1.414);
        cubes[i]->meshes.push_back(cubes[i]->volume->collisionMesh());
    }

    //create skybox
    Gadget *skybox = new Gadget(vec3(0,0,0), quaternion(), 0, strdup("sky"), TYPE1, SPAWNED, 0.1f, vec3(1,1,1), "../assets/models/sphere.obj");

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

        //quick hack-in of a cube movement animation
        // vec3 pos = cubes[0]->getPosition();
        // cubes[0]->setPosition(pos - vec3(0,0.03,0));

        //window setup
        glfwGetFramebufferSize(world->window, &width, &height);
        glViewport(0, 0, width, height); //allows us to adjust window size
        //glClearColor(oceanColor[0], oceanColor[1], oceanColor[2], 1.0);
        glClearColor(1.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rsky->enable();
        rsky->render(view, skybox);

        r->enable ();

        for(i=0; i<ncubes; i++){
            r->render(view, cubes[i]);
            cubes[i]->orientation = glm::rotate(cubes[i]->orientation,3.14f/64.f,vec3(0,1.f,0));
        }

        glfwSwapBuffers(world->window);
        glfwPollEvents();
    }



    glfwDestroyWindow(world->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}