#version 130

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

varying vec3 fragmentNormal; 
varying vec2 fragmentTexCoord;
varying float distToCam;


void main (void)
{
    vec4 position = projection * modelView * model * vec4(position,1.0);
    fragmentNormal = normal;
    fragmentTexCoord = texCoord;

    distToCam = -position.z;
    gl_Position = position;
}
