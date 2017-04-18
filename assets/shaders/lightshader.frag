#version 130

uniform vec4 color;

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

uniform int fogOn;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogStart;

uniform int oceanColoringOn;
uniform vec3 oceanTopBrightness;
uniform vec3 oceanBottomBrightness;
//uniform vec3 oceanNearColor;
uniform vec3 oceanFarColor;
uniform float oceanDensity;
uniform float surfaceDepth; //everything above this is max bright
uniform float floorDepth; //everything below this is max dark
//uniform float nearDist; //everything beyond this is max near color
//uniform float farDist; //everything beyond this is max far color

uniform int shouldTexture;
uniform sampler2D texSampler;

varying vec3 fragmentNormal;
varying vec2 fragmentTexCoord;
varying float distToCam;
varying float depth;

void main(){
    vec3 norm = normalize(fragmentNormal);

    vec4 colorLight = vec4(0.0,0.0,0.0,0.0);

    if(shouldTexture!=0){
        colorLight = texture(texSampler, fragmentTexCoord);
    } else {
        colorLight = color;
    }

    vec4 sunlightMod = vec4((lightAmb + 0.5) + max(0, dot(-lightDir, norm)) * lightInten,1.0);

    if(fogOn != 0 && -distToCam > fogStart){
        float ffog = exp2(-1.442695 * fogDensity * fogDensity * distToCam * distToCam);
        vec3 fog = (1.0 - ffog) * fogColor;
        colorLight = ffog * colorLight + vec4(fog, 1.0);
    }

    if(oceanColoringOn!=0) {

        vec3 bright = vec3(1.0,1.0,1.0);

        if(depth > surfaceDepth) {
            bright = oceanTopBrightness * bright;
        } else if(depth < floorDepth) {
            bright = oceanBottomBrightness *  bright;
        } else {
            vec3 ratio = (oceanTopBrightness - oceanBottomBrightness) / (surfaceDepth - floorDepth);
            ratio = oceanBottomBrightness + (depth - floorDepth) * ratio;
            bright = ratio * bright;
        }

        float ffogOcean = exp2(-1.442695 * oceanDensity * oceanDensity * distToCam * distToCam);
        vec3 fogOcean = (1.0 - ffogOcean) * oceanFarColor;
        colorLight = ffogOcean * colorLight + vec4(fogOcean, 1.0);
        colorLight = colorLight * vec4(bright,1.0);
    }


    colorLight = colorLight * sunlightMod;
    gl_FragColor = colorLight;
}
