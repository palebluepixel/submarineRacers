#version 130

uniform vec4 color;

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

uniform int fogOn;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogStart;

varying vec3 fragmentNormal;
varying float distToCam;

void main(){
    vec3 norm = normalize(fragmentNormal);

    vec4 colorLight = vec4((lightAmb + 0.5) + max(0, dot(-lightDir, norm)) * lightInten,1.0) * color;

    if(fogOn != 0 && -distToCam > fogStart){
        float ffog = exp2(-1.442695 * fogDensity * fogDensity * distToCam * distToCam);
        vec3 fog = (1.0 - ffog) * fogColor;
        colorLight = ffog * colorLight + vec4(fog, 1.0);
    }

    //colorLight = 0 * colorLight;
    //colorLight = vec4((lightAmb + 0.5) + max(0, dot(-lightDir, norm)) * lightInten,1.0) * color;

    gl_FragColor = colorLight; //+ vec4(-distToCam > fogStart, 0, 0, 1.0);
}
