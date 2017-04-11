#version 130

uniform mat4 modelView; 
uniform mat4 projection;

in vec3 position; 

void main (void)
{
    gl_Position =  projection * modelView * vec4(position,1.0);  
}
