#version 130

uniform vec4 color;

uniform vec3 oceanTopBrightness;
uniform vec3 oceanBottomBrightness;
uniform vec3 oceanBottomColor;
//uniform float surfaceDepth; 
//uniform float floorDepth;  

varying vec4 spaceCoords;

void main(){
    vec3 finalColor = oceanBottomColor; 
    float darkCoord = -0.5; //screen coord below which everything should be dark
    float lightCoord = 0.5;

    // Linearly interpolate between top and bottom values
    if(spaceCoords.y > lightCoord) {
        finalColor = oceanTopBrightness * finalColor;
    } else if(spaceCoords.y < darkCoord) {
        finalColor = oceanBottomBrightness * finalColor;
    } else {
        vec3 ratio = (oceanTopBrightness - oceanBottomBrightness) / (lightCoord - darkCoord);
        ratio = oceanBottomBrightness + (spaceCoords.y - darkCoord) * ratio;
        finalColor = ratio * finalColor;
    }

    vec4 fc = vec4(finalColor, 1.0);

    gl_FragColor = fc + color - color;
}
