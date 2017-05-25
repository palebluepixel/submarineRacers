#version 130

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 model;

uniform vec2 screensize;

uniform float time;
uniform vec4 campos;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

varying vec3 fragmentNormal; 
varying vec2 fragmentTexCoord;
varying float distToCam;
varying float depth;

varying vec4 pixpos;
varying vec4 campixpos;
varying mat4 modelViewB;


vec3 displace(vec3 p, vec3 dir, float phase,
  float A, float omega, float t){

  // todo: remove

  // A = 0.1;

  float k = length(dir);
  t+=phase;
  float ps = dot(dir,p)-omega*t;
  vec3 f = dir*(k/6.28) * A*sin(ps);
  float y = 1.0 * A*cos(ps);
  return vec3(f.x,y,f.z);
}
vec3 displace(vec3 p, float t, bool high_freq){
  // t=0;

  // limit is nyquist frequency.
  vec3 displ = vec3(0,0,0)
      +  displace(p,vec3(0.1,0.0,0.0),  0.3, 4.0, 1.0,t)
      +  displace(p,vec3(0.15,0.0,0.5),  0.3, 1.0, 2.0,t)
      +  displace(p,vec3(0.0,0.0,0.02),  0.0, 8.0, 0.05,t)
      // +  displace(p,vec3(0.0,0.0,1.0),  0.0, 1.0, 1.0,t)
      // +  displace(p,vec3(0.1,0.0,0.05), 0.0, 5.0, 0.2,t)
      ;
  if(high_freq){
    displ = displ
      // +  displace(p,vec3(3.0,0.0,0.1),  0.0, 0.5, 9.0,t)
      // +  displace(p,vec3(4.0,4.0,4.0),  0.0, 0.2, 8.0,t)
      // +  displace(p,vec3(1.0,5.0,5.0),  0.0, 0.1, 12.0,t)
      ;
  }
  //todo: remove
  // return vec4(0,0,0,0);
  // ^^^

  return vec3(-displ.x,displ.y,-displ.z);
}

void main (void)
{
    // Transform vertex from model space to camera space
    vec4 pModel = model * vec4(position,1.0);
    vec3 pModel3 = vec3(pModel.x/pModel.w,pModel.y/pModel.w,pModel.z/pModel.w);
    modelViewB = modelView;
    modelViewB[3].xyz=vec3(0,0,0);

    vec3 campos3 = vec3(campos.x/campos.w,campos.y/campos.w,campos.z/campos.w);
    vec3 p0  = pModel.xyz+vec3(campos3.x,0,campos3.z);

    vec3 dp0 = (displace(p0,time,true));
    vec3 dp1 = vec3(0.01,0,0)+(displace(p0+vec3(0.01,0,0),time,false));
    vec3 dp2 = vec3(0,0,0.01)+(displace(p0+vec3(0,0,0.01),time,false));

    vec3 cnormal = cross(dp1-dp0, dp2-dp0);
    fragmentNormal = normalize(-cnormal);
    // fragmentNormal = dp0.xyz;
    // if(fragmentNormal.y<0)fragmentNormal*=-vec3(.4,.4,.4);


    pixpos = vec4(dp0.x,dp0.y+campos3.y,dp0.z,1);
    // campixpos = vec4()
    pModel = pModel + vec4(dp0.xyz,0) +vec4(campos3.x,0,campos3.z,0);
    // if(pModel.x>0)pModel.x=0;

    // pModel = dp1;

    mat4 adj_modelView = modelView;
    vec4 userpos4 = modelView[3];
    vec3 userpos3 = vec3(userpos4.x/userpos4.w,userpos4.y/userpos4.w,userpos4.z/userpos4.w);

    float fragdist = length(pixpos.xyz-campos3);
    if(fragdist>700)pModel.y=mix(pModel.y,0,(fragdist-700)/100);


    pixpos = pModel;

    vec4 pModelView = projection * modelView * pModel;

    // fragmentNormal = (model * vec4(normal,0.0)).xyz;

    // todo: use provided texture coords.
    // fragmentTexCoord = texCoord;
    depth=1;
    float x = pModelView.x;
    float y = pModelView.y;
    float z = pModelView.z;




    // if (x<-100)depth=0;
    // if (y<-100)depth=0;
    // if(x>100)depth=0;
    // if(y>100)depth=0;
    // depth = x/100;
    // if(z<20)depth=0;
    // fragmentTexCoord = vec2(x,y);


    distToCam = -pModelView.z;
    // depth = 1;
    gl_Position = vec4(pModelView.x,pModelView.y,pModelView.z,pModelView.w);
}



// vec3 displace_normal(vec3 p, vec3 dir, float phase,
//   float A, float omega, float t){
//   vec3 h = A*normalize(dir);
//   float s = sin(dot(dir.xz,p.xz))-omega*t;
//   float c = cos(dot(dir.xz,p.xz))-omega*t;
//   vec3 k = dir;
//   float xx = -A*k.x*s+A*h.z*k.x*k.z*s*c-A*h.z*k.x*k.z*s*c;
//   float yy = 1-h.z*k.z*c - h.x*k.x*c + h.x*h.z*k.x*h.z*c*c - h.x*h.z*k.x*k.z*c*c;
//   float zz = -A*k.x*s+A*h.x*k.x*k.z*s*c - A*h.x*k.x*k.z*s*c;
//   return normalize(vec3(xx,yy,zz));
// }
