#version 130

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform vec3 camPos;
varying vec2 fragmentTexCoord;

varying vec4 spaceCoords;


void main (void)
{
    vec3 stretchedPosition = position;

    mat4 centered = modelView;

    centered[3][0] = 0;
    centered[3][1] = 0;
    centered[3][2] = 0;

    vec4 pos = projection * centered * model * vec4(position,1.0);

    spaceCoords = vec4(position,1.0);
    fragmentTexCoord = texCoord;
    gl_Position = pos;
}
