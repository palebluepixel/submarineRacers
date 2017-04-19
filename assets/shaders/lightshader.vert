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
varying float depth;


void main (void)
{
    // Transform vertex from model space to camera space
    vec4 positionModel = model * vec4(position,1.0);
    vec4 positionModelView = projection * modelView * positionModel;
    fragmentNormal = normal;
    fragmentTexCoord = texCoord;

    distToCam = -positionModelView.z;
    depth = positionModel.y;
    gl_Position = positionModelView;
}
