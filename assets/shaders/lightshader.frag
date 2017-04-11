#version 130

uniform vec4 color;

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

varying vec3 fragmentNormal;

void main(){
    vec3 norm = normalize(fragmentNormal);

    vec4 colorLight = vec4((lightAmb + 0.5) + max(0, dot(-lightDir, norm)) * lightInten,1.0) * color;

    gl_FragColor = colorLight;
}
