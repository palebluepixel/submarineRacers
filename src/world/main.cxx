
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
#include <graphics/cube.hxx>

/*static vec3 cubeVertices[24]= {
    vec3(-1.0f,  -1.0f,  1.0f), // 0 - 0
    vec3(-1.0f,  -1.0f,  1.0f),
    vec3(-1.0f,  -1.0f,  1.0f),

    vec3 (-1.0f,  1.0f,  1.0f), // 1 - 3
    vec3 (-1.0f,  1.0f,  1.0f),
    vec3 (-1.0f,  1.0f,  1.0f),

    vec3 ( 1.0f,  1.0f,  1.0f), // 2 - 6
    vec3 ( 1.0f,  1.0f,  1.0f),
    vec3 ( 1.0f,  1.0f,  1.0f),

    vec3( 1.0f,  -1.0f,  1.0f), // 3 - 9
    vec3( 1.0f,  -1.0f,  1.0f),
    vec3( 1.0f,  -1.0f,  1.0f),

    vec3 (-1.0f,  -1.0f, -1.0f), // 4 - 12 
    vec3 (-1.0f,  -1.0f, -1.0f),
    vec3 (-1.0f,  -1.0f, -1.0f),

    vec3 (-1.0f,  1.0f, -1.0f), // 5 - 15
    vec3 (-1.0f,  1.0f, -1.0f),
    vec3 (-1.0f,  1.0f, -1.0f),

    vec3 ( 1.0f,  1.0f, -1.0f), // 6 - 18
    vec3 ( 1.0f,  1.0f, -1.0f),
    vec3 ( 1.0f,  1.0f, -1.0f),

    vec3 ( 1.0f,  -1.0f, -1.0f), // 7 - 21
    vec3 ( 1.0f,  -1.0f, -1.0f),
    vec3 ( 1.0f,  -1.0f, -1.0f),
}; //7 

static vec3 cubeNormals[24] = {

}

static const uint32_t cubeIndices[36] = {
    0,2,1,  0,3,2,
    4,3,0,  4,7,3,
    4,1,5,  4,0,1,
    3,6,2,  3,7,6,
    1,6,5,  1,2,6,
    7,5,6,  7,4,5
  }; */

static void error_callback(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
    Error::error(std::string(description),0);
}

int keyboard[350];
int mouse[8];
double mousepos[2];

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

GLFWwindow* window;



int main(void){

    // Initialize GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        Error::error("glfwInit failed",1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window){
        Error::error("glfwCreateWindow failed",1);
        glfwTerminate();
        exit(EXIT_FAILURE);
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

    // set up shaders.
    Shader *shader = new Shader();
    shader->addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/lightshader.vert"));
    shader->addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/lightshader.frag"));
    shader->build();

    //initalize camera
    Camera *camera = new Camera();
    camera->init(vec3(6,9,4.20),vec3(0,0,0),vec3(0,1,0)); //location, looking-at, up
    camera->setFOV(90.0);
    camera->setNearFar(0.1, 100.0);

    //create view
    View *view = new View(window);
    view->addCamera(camera);
    view->setFOV(90);
    view->setNear(0.1);
    view->setFar(100.0);
    view->setSunlight(vec3(0, 1, 0), vec3(0.8, 0.8, 0.8), vec3(0.2, 0.2, 0.2));

    //create an example mesh
    //Mesh *test = new Mesh(GL_TRIANGLES);
    //test->LoadVertices(8, cubeVertices);
    //test->LoadIndices(36, cubeIndices);
    //test->color = vec4(0.1f, 0.9f, 0.5f, 1.0f);

    Cube *testcube = new Cube();

    //Renderer *r = new FlatShadingRenderer(shader);
    Renderer *r = new SunlightShadingRenderer(shader);


    while (!glfwWindowShouldClose(window)){
        //window setup
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // calculate matricies to transform us from object space to view space
        /*mat4 p = camera->projTransform((float) width / (float) height);
        mat4 v = camera->viewTransform();
        std::cout << "correct view matrix: " << glm::to_string(v) << "\n";
        std::cout << "correct projection matrix: " << glm::to_string(p) << "\n";
        std::cout << "\n\n real aspect: " << (float) width << " " << (float) height << " "  << (float) width / (float) height;
        std::cout << "\n fake aspect: " << view->getWid() << " " << view->getHt() << " " << (float)view->getAspect() << "\n";
        */
        //setup
        //shader->use();
        r->Enable ();

        //render an object
        //r->Render(view,test);
        int i;
        for(i=0; i<6; i++)
            r->Render(view, testcube->meshes[i]);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}







/*
projection: mat4x4((1.810660, 0.000000, 0.000000, 0.000000), (0.000000, 2.414213, 0.000000, 0.000000), (0.000000, 0.000000, -1.002002, -1.000000), (0.000000, 0.000000, -0.200200, 0.000000))
model view: mat4x4((1.810660, 0.000000, 0.000000, 0.000000), (0.000000, 2.414213, 0.000000, 0.000000), (0.000000, 0.000000, -1.002002, -1.000000), (0.000000, 0.000000, 9.819819, 10.000000))
color: vec4(0.100000, 0.900000, 0.500000, 1.000000)
*/


/* 
projection: mat4x4((1.810660, 0.000000, 0.000000, 0.000000), (0.000000, 2.414213, 0.000000, 0.000000), (0.000000, 0.000000, -1.002002, -1.000000), (0.000000, 0.000000, -0.200200, 0.000000))
model view: mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, -10.000000, 1.000000))

*/
/*
projection matrix: mat4x4((2.414213, 0.000000, 0.000000, 0.000000), (0.000000, 2.414213, 0.000000, 0.000000), (0.000000, 0.000000, -1.002002, -1.000000), (0.000000, 0.000000, -0.200200, 0.000000))
view matrix: mat4x4((0.573462, -0.635422, 0.517088, 0.000000), (0.000000, 0.631186, 0.775632, 0.000000), (-0.819232, -0.444795, 0.361961, 0.000000), (0.000000, -0.000000, -11.603448, 1.000000))


correct view matrix: mat4x4((0.573462, -0.635422, 0.517088, 0.000000), (0.000000, 0.631186, 0.775632, 0.000000), (-0.819232, -0.444795, 0.361961, 0.000000), (-0.000000, -0.000000, -11.603448, 1.000000))
correct projection matrix: mat4x4((1.810660, 0.000000, 0.000000, 0.000000), (0.000000, 2.414213, 0.000000, 0.000000), (0.000000, 0.000000, -1.002002, -1.000000), (0.000000, 0.000000, -0.200200, 0.000000))
view matrix: mat4x4((0.573462, -0.635422, 0.517088, 0.000000), (0.000000, 0.631186, 0.775632, 0.000000), (-0.819232, -0.444795, 0.361961, 0.000000), (0.000000, -0.000000, -11.603448, 1.000000))
projection matrix: mat4x4((2.414213, 0.000000, 0.000000, 0.000000), (0.000000, 2.414213, 0.000000, 0.000000), (0.000000, 0.000000, -1.002002, -1.000000), (0.000000, 0.000000, -0.200200, 0.000000))
*/