#version 130

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 model;

in vec3 position;
uniform vec3 camPos;

varying vec4 spaceCoords;


void main (void)
{
    vec3 stretchedPosition = position*100 + camPos;

    vec4 pos = projection * modelView * model * vec4(stretchedPosition,1.0);

    spaceCoords = vec4(position,1.0);
    gl_Position = pos;
}
