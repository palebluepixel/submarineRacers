#version 130

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 model;

in vec3 position;
//in vec3 normal;
//in vec2 texCoord;

uniform float yDim;
uniform float xDim;
uniform float zDim;

uniform vec3 camPos;

varying vec3 fragmentTexCoord;
varying vec4 spaceCoords;


void main (void)
{
    vec3 stretchedPosition = vec3(position.x*xDim+camPos.x, position.y*yDim+camPos.y, position.z*zDim+camPos.z);
    stretchedPosition = stretchedPosition - stretchedPosition;
    stretchedPosition = stretchedPosition + position*100 + camPos;

    vec4 pos = projection * modelView * model * vec4(stretchedPosition,1.0);
    fragmentTexCoord = (position + vec3(1.0,1.0,1.0))/2.0;

    spaceCoords = vec4(position,1.0);
    gl_Position = pos;
}
