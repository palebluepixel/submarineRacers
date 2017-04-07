#include <GL/glew.h>
#include <iostream>
#include "shader.hxx"

Shader::Shader(){
	built=0;
    prog = glCreateProgram();
    
}

int Shader::addShader(GLenum type, const GLchar *code){
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
	glCompileShader(shader);
	shaders.push_back(shader);
	return shader;
}
void Shader::build(){
	for (GLuint s : shaders){
		glAttachShader(prog,s);
	}
    glLinkProgram(prog);
    built = 1;
}
bool Shader::isBuilt(){
	return built!=0;
}
GLuint Shader::ID(){
	return prog;
}
void Shader::use(){
	if(built)glUseProgram(prog);
}
GLuint Shader::operator!(){
	return prog;
}