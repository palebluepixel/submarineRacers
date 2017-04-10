#version 130

uniform vec4 color;

uniform vec3 lightDir;
uniform vec3 lightInten;
uniform vec3 lightAmb;

varying vec3 fragmentNormal;

void main(){
    vec3 norm = normalize(fragmentNormal);
    norm = vec3(0.0, -1.0, 0.0);
    vec3 ndir = lightDir;
    ndir *= -1;
    float tnorm = max(0.0f, dot(ndir,norm));
    vec3 tIl = lightInten;
    tIl *= tnorm;
    vec3 tIa = lightAmb;
    tIa += tIl;

    gl_FragColor = vec4(tIa,1) * color;
}
