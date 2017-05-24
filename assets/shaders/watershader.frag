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
uniform vec3 oceanTopColor;
uniform vec3 oceanBottomColor;
uniform float oceanDensity;
uniform float surfaceDepth;
uniform float floorDepth;

uniform int shouldTexture;
uniform sampler2D texSampler;

uniform float time;

varying vec3 fragmentNormal;
varying vec2 fragmentTexCoord;
varying float distToCam;
varying float depth;

varying vec4 pixpos;

vec3 displace(vec3 p, vec3 dir, float phase,
  float A, float omega, float t){
  // float k = length(dir);
  // t+=phase;
  float ps = dot(dir,p)-omega*t;
  // vec3 f = vec3(1,1,1)*A*sin(ps);
  // float y = 1.0 * A*cos(ps);
  return A*vec3(sin(p.x*dir.x+omega*t),0,sin(p.z*dir.z+omega*t));
}
vec4 displace(vec3 p, float t){
  return vec4(0,0,0,0)
      // +  vec4(displace(p,vec3(3.0,0.0,0.0),  0.0, 0.1, 4.0,t),0)
      // *  vec4(displace(p,vec3(0.0,0.0,9.0),  0.0, 0.9, 3.0,t),0)
      // +  vec4(displace(p,vec3(4.0,4.0,0.0),  0.0, 0.1, 8.0,t),0)
      // +  vec4(displace(p,vec3(1.0,5.0,5.0),  0.0, 0.3, 15.0,t),0)
      ;
}

void main(){
    // Make sure its normalized
    vec3 norm = normalize(fragmentNormal+displace(pixpos.xyz,time).xyz);


    // colorLight will hold the color of this fragment as we apply various
    // visual effects
    vec4 colorLight = vec4(0.0,0.0,0.0,0.0);

    // Texture
    vec4 colorTex = texture(texSampler, fragmentTexCoord);
    vec3 lightDirm=vec3(0.707,-0.707,0);

    float light = pow(abs(dot(-lightDirm, norm)),5);

    colorLight=colorTex * vec4(0.2,0.8,0.5,1.0)*(0.5+((pixpos.y+40)*0.02))*0.4 + 0.2*max(0.0, light);

    // if(light<0)colorLight.xyz*=(1,0.5,0.5);

    // Diffuse and Directed Sunlight Lighting
    // (note that we aply this light at the very end of the shader program)

    // colorLight = colorLight * sunlightMod; // apply sun and diffuse

    // gl_FragColor = vec4(0.5+0.25*fragmentNormal.x,0.5+0.25*fragmentNormal.y,0.5+0.25*fragmentNormal.z,1.0);
    // gl_FragColor = vec4(fragmentNormal.x,fragmentNormal.y,fragmentNormal.z,1.0);
    // Out
    gl_FragColor = vec4(colorLight.xyz,0.8);
    // gl_FragColor = vec4(1,0,0,1);

}