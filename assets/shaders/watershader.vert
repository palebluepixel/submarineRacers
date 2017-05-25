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
vec4 displace(vec3 p, float t, bool high_freq){
  t=0;

  // limit is nyquist frequency.
  vec4 displ = vec4(0,0,0,1)
      +  vec4(displace(p,vec3(0.2,0.0,0.0),  0.3, 10.0, 1.0,t),0)
      +  vec4(displace(p,vec3(0.3,0.0,1.6),  0.3, 1.0, 2.0,t),0)
      // +  vec4(displace(p,vec3(-5.0,0.0,0.1),  0.0, 0.5, 9.0,t),0)
      +  vec4(displace(p,vec3(0.0,0.0,1.0),  0.0, 1.0, 1.0,t),0)
      +  vec4(displace(p,vec3(0.1,0.0,0.05), 0.0, 5.0, 0.2,t),0)
      ;
  if(high_freq){
    displ = displ
      +  vec4(displace(p,vec3(3.0,0.0,0.1),  0.0, 0.5, 9.0,t),0)
      +  vec4(displace(p,vec3(4.0,4.0,4.0),  0.0, 0.2, 8.0,t),0)
      +  vec4(displace(p,vec3(1.0,5.0,5.0),  0.0, 0.1, 12.0,t),0)
      ;
  }

  //todo: remove
  // return vec4(0,0,0,0);
  // ^^^

  return displ;
}

void main (void)
{
    // Transform vertex from model space to camera space
    vec4 pModel = model * vec4(position,1.0);
    mat4 modelViewB = modelView;
    modelViewB[3].xyz=vec3(0,0,0);

    vec4 dp0 = vec4(displace((pModel+campos).xyz,time,true));
    vec4 dp1 = vec4(displace((pModel+campos).xyz+vec3(0.01,0,0),time,false));
    vec4 dp2 = vec4(displace((pModel+campos).xyz+vec3(0,0,0.01),time,false));

    vec3 cnormal = cross((dp1-dp0).xyz, (dp2-dp0).xyz);
    fragmentNormal = -normalize(cnormal);
    // if(fragmentNormal.y<0)fragmentNormal*=-vec3(.4,.4,.4);


    pModel = pModel + dp0 -vec4(0,campos.y,0,0);
    // if(pModel.x>0)pModel.x=0;

    // pModel = dp1;
    pixpos = pModel;

    mat4 adj_modelView = modelView;
    vec4 userpos4 = modelView[3];
    vec3 userpos3 = vec3(userpos4.x/userpos4.w,userpos4.y/userpos4.w,userpos4.z/userpos4.w);

    vec4 pModelView = projection * modelViewB * pModel;

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
