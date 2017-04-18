#version 130

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform vec3 camPos;

varying vec3 fragmentNormal; 
varying vec2 fragmentTexCoord;
varying float distToCam;
varying float depth;


void main (void)
{
    vec3 stretchedPosition = position - camPos;
    
    vec4 position2 = projection * modelView * model * vec4(position,1.0);
    fragmentNormal = normal;
    fragmentTexCoord = texCoord;

    distToCam = -position2.z;
    depth = stretchedPosition.y;
    gl_Position = position2;
}
