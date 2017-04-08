
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

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static vec3 cubeVertices[8]= {vec3(-1.0f,  -1.0f,  1.0f), //0
                   vec3 (-1.0f,  1.0f,  1.0f), //1
                   vec3 ( 1.0f,  1.0f,  1.0f), //2
                   vec3( 1.0f,  -1.0f,  1.0f), //3
                   vec3 (-1.0f,  -1.0f, -1.0f), //4
                   vec3 (-1.0f,  1.0f, -1.0f), //5
                   vec3 ( 1.0f,  1.0f, -1.0f), //6
                   vec3 ( 1.0f,  -1.0f, -1.0f)}; //7 
/* the indices that allow us to create the cube. */ 
static const uint32_t cubeIndices[36] = {
    0,2,1,  0,3,2,
    4,3,0,  4,7,3,
    4,1,5,  4,0,1,
    3,6,2,  3,7,6,
    1,6,5,  1,2,6,
    7,5,6,  7,4,5
  }; 

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

    // Remove these lines...
    GLuint vertex_buffer, index_buffer, vao;
    GLint proj_location, mod_location, position_location, color_location;

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
    shader->addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/shader.vert"));
    shader->addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/shader.frag"));
    shader->build();
    //printf("%d\n", shader->ID());

    proj_location = glGetUniformLocation(shader->ID(), "projection");
    mod_location =  glGetUniformLocation(shader->ID(), "modelView");
    color_location = glGetUniformLocation(shader->ID(), "color");
    position_location = glGetAttribLocation(shader->ID(), "position");
    printf("%d\n", position_location);

    // OpenGL array stuff (TODO: error checks)

    /*glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(cubeVertices[0]), (void*) 0);

    glGenBuffers (1, &index_buffer);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(uint32_t), cubeIndices, GL_STATIC_DRAW); //might be STATIC_DRAW
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(position_location);*/


    //initalize camera
    Camera *camera = new Camera();
    camera->init(vec3(6,9,10),vec3(0,0,0),vec3(0,1,0));
    camera->setFOV(90.0);
    camera->setNearFar(0.1, 100.0);








    Mesh *test = new Mesh(GL_TRIANGLES);
    test->LoadVertices(8, cubeVertices);
    test->LoadIndices(36, cubeIndices);
    test->color = vec4(0.1f, 0.9f, 0.5f, 1.0f);

    Renderer *r = new FlatShadingRenderer(shader);
    



    while (!glfwWindowShouldClose(window)){
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        camera->setViewport(width, height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4 Projection = camera->projTransform();
        mat4 View = camera->viewTransform();
          
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4();

        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

        shader->use();
        //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, value_ptr(mvp));
        //setUniform(proj_location, Projection);
        //setUniform(mod_location, View*Model);
        //setUniform(color_location, vec4(1.0f, 0.5f, 0.3f, 1.0f));
        //std::cout << "model view: " << glm::to_string(View*Model) << "\n"
        //    << "projection: " << glm::to_string(Projection) << "\n";
        //shader.setUniformByName("MVP", mvp);
        //glDrawArrays(GL_LINES, 0, 36);
        //printf("%d\n", vao);
        //glBindVertexArray(vao);
        //glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        //glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);

        /* This will go in view->render */
        /* clear the screen */
        //glClearColor (0.0f, 0.0f, 0.0f, 1.0f);    // clear the surface
        //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Enable the renderer */ 
        r->Enable (Projection);
        /* HINT: Draw the objects in the scene */
        r->Render(View,test);

        test->Draw();

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