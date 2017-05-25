#version 130

uniform mat4 modelView; 
uniform mat4 projection;

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
uniform sampler2D mapReflDepth;

uniform float time;
uniform vec2 screensize;
uniform vec4 campos;

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
// depthSample from depthTexture.r, for instance
float linearDepth(float depthSample){
    // todo: make this a uniform which is pulled from code.
    float zNear = 0.1;
    float zFar = 1000.0;
    // ^^^^
    depthSample = 2.0 * depthSample - 1.0;
    float zLinear = 2.0 * zNear * zFar / (zFar + zNear - depthSample * (zFar - zNear));
    return zLinear;
}

void main(){
    // Make sure its normalized
    vec3 norm = normalize(fragmentNormal);
    vec2 relscrpos = vec2(gl_FragCoord.x/screensize.x, gl_FragCoord.y/screensize.y);


    // colorLight will hold the color of this fragment as we apply various
    // visual effects
    vec4 colorLight = vec4(0.0,0.0,0.0,0.0);

    // Texture
    float fragd = gl_FragCoord.z / gl_FragCoord.w;
    float zdist = linearDepth(texture(mapReflDepth, relscrpos.xy).x) - fragd; // distance from pixel to image point.

    vec4 colorTex = texture(texSampler, relscrpos.xy/* +vec2(gl_FragCoord.z,gl_FragCoord.z) */) ;
    

    vec3 lightDirm=vec3(0.707,-0.707,0);
    float light = pow(abs(dot(-lightDirm, norm)),5);

    // todo: remove
    // norm = vec3(0,1,0);

    vec3 pixpos3 = vec3(pixpos.x/pixpos.w,pixpos.y/pixpos.w,pixpos.z/pixpos.w);
    vec3 campos3 = vec3(campos.x/campos.w,campos.y/campos.w,campos.z/campos.w);

    vec3 camToPt = normalize(pixpos3-campos3);
    vec3 ptToImg = (camToPt.xyz - 2 * norm * dot(camToPt.xyz,norm));
    vec3 imgPos  = pixpos3+(4)*ptToImg;  // position of image.
    vec3 imgPosR =  vec3(imgPos.x,-imgPos.y,imgPos.z);  // reflect across water.

    // todo: remove;
    // imgPosR = pixpos3+camToPt;

    mat4 mp = projection * modelView;
    vec4 outpos =  mp * vec4(imgPosR,1);
    vec2 texpos =  (1 + outpos.xy / outpos.w)/2;
    if(texpos.x<0)texpos.x=0;
    if(texpos.y<0)texpos.y=0;
    if(texpos.x>1)texpos.x=1;
    if(texpos.y>1)texpos.y=1;

    colorTex = texture(texSampler,texpos);


    // note: vec4(0.2,0.8,0.5,1.0) is a good ocean-water color.

    colorLight=colorTex * vec4(0.2,0.8,0.5,1.0)/* *(0.5+((pixpos.y+40)*0.02))*0.4 */ + 0.2*max(0.0, light);

    // if(light<0)colorLight.xyz*=(1,0.5,0.5);

    // Diffuse and Directed Sunlight Lighting
    // (note that we aply this light at the very end of the shader program)

    // colorLight = colorLight * sunlightMod; // apply sun and diffuse

    // gl_FragColor = vec4(0.5+0.25*fragmentNormal.x,0.5+0.25*fragmentNormal.y,0.5+0.25*fragmentNormal.z,1.0);
    // gl_FragColor = vec4(fragmentNormal.x,fragmentNormal.y,fragmentNormal.z,1.0);
    // Out

    // float mult = (-campos.z)+-1;


    // gl_FragColor=vec4(campos.x-15.3,campos.y+9.8,campos.z+2.1,1);
    
    // gl_FragColor=vec4(campos.x-15.3,campos.y+9.8,campos.z-1.6,1);
    
    // if(gl_FragCoord.y <0)mult=0;
    gl_FragColor = colorTex* vec4(colorLight.xyz,1.0);
    // gl_FragColor = vec4(abs(camToPt.x),abs(camToPt.y),abs(camToPt.z),1);
    // gl_FragColor=vec4(abs(norm.x),abs(norm.y),abs(norm.z),1);
    // gl_FragColor = colorTex * depth * mult;
    // gl_FragColor = vec4(depth);

}