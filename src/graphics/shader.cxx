#include <GL/glew.h>
#include <iostream>
#include "shader.hxx"

using namespace glm;

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

// Code below modified from John Reppy 
/*! \file cs237-shader.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * This file implements an API for loading and using shaders.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

//return the GLint index of a uniform variable in this shader. 
GLint Shader::getAttributeLocation (const char *name)
{
    GLint id = glGetAttribLocation (this->ID(), name);
    if (id < 0) {
    std::cerr << "Error: shader attribute \"" << name << "\" is invalid\n" << std::endl;
    exit (1);
    }
    return id;
}

//! \brief return the location of a uniform variable in this shader
//
GLint Shader::getUniformLocation (const char *name)
{
    GLint id = glGetUniformLocation (this->ID(), name);
    if (id < 0) {
    std::cerr << "Error: shader uniform \"" << name << "\" is invalid\n" << std::endl;
    exit (1);
    }
    return id;
}



/*================================ BY NAME ===============================*/

/*  Use these functions to set a uniform or attribute variable by name. 

    See setUniform and setAttrbiute from shader.inl if you want to use
    the GLint index location instead of the variable name. */

void Shader::setUniformByName (const char *name, int const &v) 
{ glUniform1i (this->getUniformLocation(name), v); }

void Shader::setUniformByName (const char *name, int const &v1, int const &v2)
{ glUniform2i (this->getUniformLocation(name), v1, v2); }

void Shader::setUniformByName (const char* name, int const &v1, int const &v2, int const &v3)
{ glUniform3i (this->getUniformLocation(name), v1, v2, v3); }

void Shader::setUniformByName (const char* name, int const &v1, int const &v2, int const &v3, int const &v4)
{ glUniform4i (this->getUniformLocation(name), v1, v2, v3, v4); }

void Shader::setUniformByName (const char* name, unsigned int const &v)
{ glUniform1ui (this->getUniformLocation(name), v); }

void Shader::setUniformByName (const char* name, unsigned int const &v1, unsigned int const &v2)
{ glUniform2ui (this->getUniformLocation(name), v1, v2); }

void Shader::setUniformByName (const char* name, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3)
{ glUniform3ui (this->getUniformLocation(name), v1, v2, v3); }

void Shader::setUniformByName (const char* name, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3, unsigned int const &v4)
{ glUniform4ui (this->getUniformLocation(name), v1, v2, v3, v4); }

void Shader::setUniformByName (const char* name, float const &v)
{ glUniform1f (this->getUniformLocation(name), v); }

void Shader::setUniformByName (const char* name, vec2 const &v)
{ glUniform2fv (this->getUniformLocation(name), 1, value_ptr(v)); }

void Shader::setUniformByName (const char* name, vec3 const &v)
{ glUniform3fv (this->getUniformLocation(name), 1, value_ptr(v)); }

void Shader::setUniformByName (const char* name, vec4 const &v)
{ glUniform4fv (this->getUniformLocation(name), 1, value_ptr(v)); }

void Shader::setUniformByName (const char* name, mat2 const &v)
{ glUniformMatrix2fv (this->getUniformLocation(name), 1, GL_FALSE, value_ptr(v)); }

void Shader::setUniformByName (const char* name, mat3 const &v)
{ glUniformMatrix3fv (this->getUniformLocation(name), 1, GL_FALSE, value_ptr(v)); }

void Shader::setUniformByName (const char* name, mat4 const &v)
{ glUniformMatrix4fv (this->getUniformLocation(name), 1, GL_FALSE, value_ptr(v)); }

/***** functions to Shader::set individual attribute variables *****/

void Shader::setAttributeByName (const char* name, int const &v)
{ glVertexAttribI1i (this->getUniformLocation(name), v); }

void Shader::setAttributeByName (const char* name, unsigned int const &v)
{ glVertexAttribI1ui (this->getUniformLocation(name), v); }

void Shader::setAttributeByName (const char* name, float const &v)
{ glVertexAttrib1f (this->getUniformLocation(name), v); }

void Shader::setAttributeByName (const char* name, vec2 const &v)
{ glVertexAttrib2fv (this->getUniformLocation(name), value_ptr(v)); }

void Shader::setAttributeByName (const char* name, vec3 const &v)
{ glVertexAttrib3fv (this->getUniformLocation(name), value_ptr(v)); }

void Shader::setAttributeByName (const char* name, vec4 const &v)
{ glVertexAttrib4fv (this->getUniformLocation(name), value_ptr(v)); }