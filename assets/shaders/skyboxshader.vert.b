#version 130

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 model;

uniform float yDim;
uniform float xDim;
uniform float zDim;

uniform vec3 camPos;

in vec3 position;

varying vec3 fragmentTexCoord;


void main (void)
{
    vec3 stretchedPosition = vec3(position.x*xDim+camPos.x, position.y*yDim+camPos.y, position.z*zDim+camPos.z);

    vec4 pos = projection * modelView * model * vec4(stretchedPosition,1.0);
    fragmentTexCoord = (position + vec3(1.0,1.0,1.0))/2.0;

    gl_Position = pos;
}
