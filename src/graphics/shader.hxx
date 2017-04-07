#include <GL/gl.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.inl"

	using namespace glm;

class Shader{
private:
	std::vector<GLuint> shaders;
	GLuint prog;
	int built;
public:
	Shader();
	int addShader(GLenum type, const GLchar *code);
	void build();
	bool isBuilt();
	GLuint ID();
	GLuint operator!();
	void use();

	//return the GLint index of a uniform variable in this shader
	GLint getUniformLocation(const char* name);
	//... attribute variable...
	GLint getAttributeLocation(const char* name);


	//set variable by name
	void setUniformByName (const char *name, int const &v); 
	void setUniformByName (const char *name, int const &v1, int const &v2);
	void setUniformByName (const char* name, int const &v1, int const &v2, int const &v3);
	void setUniformByName (const char* name, int const &v1, int const &v2, int const &v3, int const &v4);
	void setUniformByName (const char* name, unsigned int const &v);
	void setUniformByName (const char* name, unsigned int const &v1, unsigned int const &v2);
	void setUniformByName (const char* name, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3);
	void setUniformByName (const char* name, unsigned int const &v1, unsigned int const &v2, unsigned int const &v3, unsigned int const &v4);
	void setUniformByName (const char* name, float const &v);
	void setUniformByName (const char* name, vec2 const &v);
	void setUniformByName (const char* name, vec3 const &v);
	void setUniformByName (const char* name, vec4 const &v);
	void setUniformByName (const char* name, mat2 const &v);
	void setUniformByName (const char* name, mat3 const &v);
	void setUniformByName (const char* name, mat4 const &v);

	void setAttributeByName (const char* name, int const &v);
	void setAttributeByName (const char* name, unsigned int const &v);
	void setAttributeByName (const char* name, float const &v);
	void setAttributeByName (const char* name, vec2 const &v);
	void setAttributeByName (const char* name, vec3 const &v);
	void setAttributeByName (const char* name, vec4 const &v);




};