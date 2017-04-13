#version 130

uniform vec4 color;

uniform int fogOn;
uniform vec3 fogColor;

uniform int shouldTexture;
uniform sampler2D texSampler;

varying vec3 fragmentTexCoord;
varying vec4 spaceCoords;

void main(){
    vec2 texSide = fragmentTexCoord.xy;

    vec4 colorLight = vec4(0.0,0.0,0.0,0.0);

    if(shouldTexture!=0){
        colorLight = texture(texSampler, texSide);
    } else {
        colorLight = color;
    }


    if(fogOn != 0){
        colorLight = vec4(fogColor, 1.0);
    }

    colorLight = colorLight - colorLight + spaceCoords; 

    gl_FragColor = colorLight;
}
