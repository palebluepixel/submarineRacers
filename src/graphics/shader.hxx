#include <GL/gl.h>
#include <vector>
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
};