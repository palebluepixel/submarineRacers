#version 130

uniform vec4 color;

uniform int fogOn;
uniform vec3 fogColor;
uniform float fogDensity;

uniform int shouldTexture;
uniform sampler2D texSampler;

varying vec3 fragmentTexCoord;

void main(){
    vec2 texSide = fragmentTexCoord.xy;

    vec4 colorLight = vec4(0.0,0.0,0.0,0.0);

    if(shouldTexture!=0){
        colorLight = texture(texSampler, texSide);
    } else {
        colorLight = color;
    }


    if(fogOn != 0){
        float ffog = exp2(-1.442695 * fogDensity * fogDensity * (1.0 - gl_FragCoord.y) * (1.0 - gl_FragCoord.y));
        vec3 fog = (1.0 - ffog) * fogColor;
        colorLight = ffog * colorLight + vec4(fog, 1.0);
    }

    gl_FragColor = vec4(fogColor, 1.0);
}
