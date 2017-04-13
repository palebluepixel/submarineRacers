#version 130

uniform vec4 color;

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

uniform int fogOn;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogStart;

uniform int shouldTexture;
uniform sampler2D texSampler;

varying vec3 fragmentNormal;
varying vec2 fragmentTexCoord;
varying float distToCam;

void main(){
    vec3 norm = normalize(fragmentNormal);

    vec4 colorLight = vec4(0.0,0.0,0.0,0.0);

    if(shouldTexture!=0){
        colorLight = texture(texSampler, fragmentTexCoord);
    } else {
        colorLight = color;
    }

    colorLight = vec4((lightAmb + 0.5) + max(0, dot(-lightDir, norm)) * lightInten,1.0) * colorLight;

    if(fogOn != 0 && -distToCam > fogStart){
        float ffog = exp2(-1.442695 * fogDensity * fogDensity * distToCam * distToCam);
        vec3 fog = (1.0 - ffog) * fogColor;
        colorLight = ffog * colorLight + vec4(fog, 1.0);
    }

    gl_FragColor = colorLight;
}
