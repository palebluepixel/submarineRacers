
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
    GLuint vertex_buffer;
    GLint mvp_location, vpos_location, vcol_location;

    // Initialize GLFW
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        Error::error("glfwInit failed",1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
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
    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glerr = glewInit();
    glGetError();    
    if (glerr != GLEW_OK){
        Error::error("GLEW init failed"+glerr, 1);
    }

    glfwSwapInterval(1);

    // OpenGL array stuff (TODO: error checks)

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set up shaders.

    Shader shader;
    shader.addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/simple.vert"));
    shader.addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/simple.frag"));
    shader.build();

    mvp_location = glGetUniformLocation(!shader, "MVP");
    vpos_location = glGetAttribLocation(!shader, "vPos");
    vcol_location = glGetAttribLocation(!shader, "vCol");
    
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));

    //initalize camera
    Camera *camera = new Camera();
    camera->init(vec3(0,0,2),vec3(0,0,0),vec3(0,1,0));
    camera->setFOV(90.0);
    camera->setNearFar(0.1, 100.0);

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

        shader.use();
        //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, value_ptr(mvp));
        setUniform(mvp_location, mvp);
        //shader.setUniformByName("MVP", mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}