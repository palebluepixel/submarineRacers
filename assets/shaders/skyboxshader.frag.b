#version 130

uniform vec4 color;

uniform int fogOn;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogStart;

uniform int shouldTexture;
uniform sampler2D texSampler;

varying vec3 fragmentTexCoord;

void main(){
    vec2 texSide;
    switch(side)
    {
        case 0:
            texSide = vec2(1-fragmentTexCoord.z, fragmentTexCoord.y);
        case 1:
            texSide = fragmentTexCoord.zy;
            break;
        case 2:
        case 3:
            texSide = fragmentTexCoord.xz;
            break;
        case 4:
            texSide = fragmentTexCoord.xy;
            break;
        case 5:
            texSide = vec2(1-fragmentTexCoord.x, fragmentTexCoord.y);
    }

    vec4 colorLight = vec4(0.0,0.0,0.0,0.0);

    if(shouldTexture!=0){
        colorLight = texture(texSampler, fragmentTexCoord);
    } else {
        colorLight = color;
    }

    if(fogOn != 0){
        float ffog = exp2(-1.442695 * fogDensity * fogDensity * (1.0 - gl_FragCoord.y) * (1.0 - gl_FragCoord.y));
        vec3 fog = (1.0 - ffog) * fogColor;
        colorLight = ffog * colorLight + vec4(fog, 1.0);
    }

    gl_FragColor = colorLight;
}
