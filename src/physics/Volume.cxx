#include <glm/gtc/matrix_transform.hpp>
#include "Volume.hxx"
#include <stdio.h>
#include <string.h>
#include <util/log.hxx>

using namespace Space;

Volume::Volume(Pos p) : pos(p){
}

Volume::Pos::Pos(Entity *e){
  set(e);
  transformInv_calc = false;
}
void Volume::Pos::set(Entity *e){
  if(!e){
    logln(LOGHIGH,"Error: Pos passed null argument.");
  }
  mat4 tnew = e->modelMatrix();

  // optimize this by not checking the bottom row
  //   (which is always 0 0 0 1)
  float delta = std::abs(tnew[0][0]-transform[0][0]) + 
                std::abs(tnew[0][1]-transform[0][1]) + 
                std::abs(tnew[0][2]-transform[0][2]) + 
                std::abs(tnew[0][3]-transform[0][3]) + 
                std::abs(tnew[1][0]-transform[1][0]) + 
                std::abs(tnew[1][1]-transform[1][1]) + 
                std::abs(tnew[1][2]-transform[1][2]) + 
                std::abs(tnew[1][3]-transform[1][3]) + 
                std::abs(tnew[2][0]-transform[2][0]) + 
                std::abs(tnew[2][1]-transform[2][1]) + 
                std::abs(tnew[2][2]-transform[2][2]) + 
                std::abs(tnew[2][3]-transform[2][3]) + 
                std::abs(tnew[3][0]-transform[3][0]) + 
                std::abs(tnew[3][1]-transform[3][1]) + 
                std::abs(tnew[3][2]-transform[3][2]) + 
                std::abs(tnew[3][3]-transform[3][3]);
  // update collision volume only if this entity has moved
  // or rotated more than 1mm.
  if(delta>0.001){
    pos = e->pos();
    ori = e->getOrientation();
    transform = tnew;
    transformInv_calc = false;
    updated=1;
  }
}

//////////////////////////////////////////*
//            Sphere Volume
/////////////////////////////////////////*/

const char *SphereVolume::type(){ return "sphere"; }
SphereVolume::SphereVolume(Pos posi, double rad) : Volume(posi){
  r = rad>0?rad:0;
  if(SphereVolume::mesh == 0){
    SphereVolume::mesh = new Mesh(GL_TRIANGLES);
    SphereVolume::mesh->loadOBJ("../assets/models/sphere.obj");
    // SphereVolume::mesh->data.polygon_mode = GL_LINE;
  }
}
double SphereVolume::distance(Volume *other){
    if(!strcmp(other->type(),"sphere")){
        SphereVolume* v = static_cast<SphereVolume*>(other);
  		double dx = pos.pos.x - v->pos.pos.x;
  		double dy = pos.pos.y - v->pos.pos.y;
  		double dz = pos.pos.z - v->pos.pos.z;
  		double dist = sqrt(dx*dx + dy*dy + dz*dz);

  		return dist - r - v->r;

    }else
    if(!strcmp(other->type(),"cylinder")){
           // CylinderVolume* v = static_cast<CylinderVolume*>(other);
    	return 0;
    }
}
Model SphereVolume::collisionMesh(){
  Model tm(
    Model::FancyMesh(
      mesh,
      glm::scale(glm::mat4(1),vec3(r,r,r)),
      Model::RenderState(true, vec4(1,1,1,1), GL_LINE, GL_BACK, 1)));

  return tm;
}

vec3 SphereVolume::push(Volume *other){
  if(other->type() == "sphere"){
    SphereVolume *sph = static_cast<SphereVolume*>(other);
    float dist = distance(sph);
    if(dist>0)return vec3();

    return (pos.pos - other->pos.pos)*dist;
  }else if(!strcmp(other->type(),"cylinder")){
    return -other->push(this);
  }else if(!strcmp(other->type(),"heightmap")){
    return -other->push(this);
  }else if(!strcmp(other->type(),"flat")){
    return -other->push(this);
  }
  else{
    return vec3(0,0,0);
  }
}

vec3 SphereVolume::pushAlong(Volume *other, vec3 direction){
  return vec3();
}
bool SphereVolume::collision(Volume *other){
  return false;
}
bool SphereVolume::containsPoint(vec3 pt){
  return false;
}
float SphereVolume::R(){
  return r;
}

Mesh* SphereVolume::mesh = 0;


//////////////////////////////////////////*
//            Cylinder Volume
/////////////////////////////////////////*/

const char *CylinderVolume::type(){ return "cylinder"; }
CylinderVolume::CylinderVolume(Pos posi, double rad, double h, glm::mat4 rotation) : Volume(posi){
  r = rad>0?rad:0;
  h = h>0?h:0;
  this->h=h;
  this->rotation=rotation;
  if(!CylinderVolume::meshcyl){
    CylinderVolume::meshcyl = new Mesh(GL_TRIANGLES);
    CylinderVolume::meshcyl->loadOBJ("../assets/models/cylinder.obj");
    // CylinderVolume::meshcyl->data.polygon_mode = GL_LINE;
  }
  if(!CylinderVolume::meshcap){
    CylinderVolume::meshcap = new Mesh(GL_TRIANGLES);
    CylinderVolume::meshcap->loadOBJ("../assets/models/hemisphere.obj");
    // CylinderVolume::meshcap->data.polygon_mode = GL_LINE;
  }
}
double CylinderVolume::distance(Volume *other){

    if(!strcmp(other->type(),"sphere")){
        SphereVolume* v = static_cast<SphereVolume*>(other);

        vec4 bottom = pos.transform * rotation * vec4(0,0.5*h,0,1);
        vec4 top    = pos.transform * rotation * vec4(0,-0.5*h,0,1);
        DistanceResult dr = shortestDistance(v->pos.pos, Segment{vec3(bottom.x,bottom.y,bottom.z),vec3(top.x,top.y,top.z)});

        // fprintf(stderr,"(%.3f,%.3f,%.3f) - (%.3f,%.3f,%.3f)\n",bottom.x,bottom.y,bottom.z,top.x,top.y,top.z);
        
        return dr.distance;

    }else if(!strcmp(other->type(),"cylinder")){
           // CylinderVolume* v = static_cast<CylinderVolume*>(other);
      return 0;
    }
}
Model CylinderVolume::collisionMesh(){
  Model::RenderState rs(true, vec4(1,1,1,1), GL_LINE, GL_BACK, 1);
  Model tm(Model::FancyMesh(meshcyl,rotation*glm::scale(glm::mat4(1),vec3(r,h/2.f,r)),rs));
  glm::mat4 topcap = glm::translate(mat4(1),glm::vec3(0,h/2,0)) * glm::scale(mat4(1),glm::vec3(r,r,r));
  glm::mat4 botcap = glm::translate(mat4(1),glm::vec3(0,-h/2,0)) * glm::scale(mat4(1),glm::vec3(r,-r,r));
  tm.meshes.push_back(Model::FancyMesh(meshcap,rotation*topcap,rs));
  tm.meshes.push_back(Model::FancyMesh(meshcap,rotation*botcap,rs));
  for(int i=0;i<4;i++){
    // printf("%f,%f,%f,%f\n",tm.transform[i][0],tm.transform[i][1],tm.transform[i][2],tm.transform[i][3]);
  }
  return tm;
}

vec3 CylinderVolume::push(Volume *other){
  if(other->type() == "sphere"){
    SphereVolume *v = static_cast<SphereVolume*>(other);

    vec4 bottom = pos.transform * rotation * vec4(0,0.5*h,0,1);
    vec4 top    = pos.transform * rotation * vec4(0,-0.5*h,0,1);
    DistanceResult dr = shortestDistance(v->pos.pos, Segment{vec3(bottom.x,bottom.y,bottom.z),vec3(top.x,top.y,top.z)});

    // fprintf(stderr,"DR: %.4f\n",dr.distance);

    // fprintf(stderr,"(%.3f,%.3f,%.3f) - (%.3f,%.3f,%.3f) : (%.3f,%.3f,%.3f)\n",
    //   bottom.x,bottom.y,bottom.z,top.x,top.y,top.z,other->pos.pos.x,other->pos.pos.y,other->pos.pos.z);
    // fprintf(stderr," -- (%.3f,%.3f,%.3f) (%.3f,%.3f%.3f) : %.3f\n",dr.a.x,dr.a.y,dr.a.z,dr.b.x,dr.b.y,dr.b.z,dr.distance);

    dr.distance -= (v->R() + r);

    if(dr.distance < 0){
      return dr.distance*normalize(dr.b - dr.a);
    }
    else return vec3();
  }else if(other->type() == "cylinder"){
    CylinderVolume *v = static_cast<CylinderVolume*>(other);

    // me
    vec4 bottom1 = pos.transform * rotation * vec4(0,0.5*h,0,1);
    vec4 top1    = pos.transform * rotation * vec4(0,-0.5*h,0,1);

    // them
    vec4 bottom2 = v->pos.transform * v->rotation * vec4(0,0.5*v->h,0,1);
    vec4 top2    = v->pos.transform * v->rotation * vec4(0,-0.5*v->h,0,1);

    DistanceResult dr = shortestDistance(Segment{vec3(bottom1.x,bottom1.y,bottom1.z),vec3(top1.x,top1.y,top1.z)},
                                         Segment{vec3(bottom2.x,bottom2.y,bottom2.z),vec3(top2.x,top2.y,top2.z)});
    dr.distance -= (v->R() + r);

    if(dr.distance < 0){
      return -dr.distance*normalize(dr.b-dr.a);
    }
    else return vec3();
  }else if(!strcmp(other->type(),"heightmap")){
    return -other->push(this);
  }else if(!strcmp(other->type(),"flat")){
    return -other->push(this);
  }
  else{
    return vec3(0,0,0);
  }
}

vec3 CylinderVolume::pushAlong(Volume *other, vec3 direction){
  return vec3();
}
bool CylinderVolume::collision(Volume *other){
  return false;
}
bool CylinderVolume::containsPoint(vec3 pt){
  return false;
}
float CylinderVolume::R(){
  return r;
}

mat4 CylinderVolume::Rotation(){
  return rotation;
}
float CylinderVolume::H(){
  return h;
}

Mesh* CylinderVolume::meshcyl = 0;
Mesh* CylinderVolume::meshcap = 0;

//////////////////////////////////////////*
//           Heightmap Volume
/////////////////////////////////////////*/

HeightmapVolume::HeightmapVolume(Pos posi, mat4 scale, int width, int height,
  HeightmapData data) : Volume(posi){
  // nothing yet.
  this->width = width;
  this->height=height;
  this->scale = scale;
  this->scaleInv = inverse(scale);
  this->data = data;
}
const char *HeightmapVolume::type(){return "heightmap";}

double HeightmapVolume::distance(Volume *other){
  return 1.f;
}

vec3 HeightmapVolume::push(Volume *other){
  if(other->type() == "sphere" || other->type() == "cylinder"){
    float rad=0;
    vec4 p1,p2;
    if(other->type() == "sphere"){
      SphereVolume *v = static_cast<SphereVolume*>(other);
      p1=vec4(v->pos.pos,1.f);
      p2=p1;
      rad = v->R();
    }else{
      CylinderVolume *v = static_cast<CylinderVolume*>(other);
      p1 = v->pos.transform * v->Rotation() * vec4(0,0.5*v->H(),0,1);
      p2 = v->pos.transform * v->Rotation() * vec4(0,-0.5*v->H(),0,1);
      rad = v->R();
    }
    if(!pos.transformInv_calc){
      pos.transformInv_calc=true;
      pos.transformInv = inverse(pos.transform);
    }
    mat4 WtoI = this->scaleInv * pos.transformInv;
    mat4 ItoW = pos.transformInv * this->scale;
    vec4 localPos1 = WtoI * p1;
    vec4 localPos2 = WtoI * p2;
    // do this faster by just looking at the matrix entries of scaleInv * pos.transformInv.
    vec4 preimX   = WtoI * vec4(1,0,0,0);
    vec4 preimY   = WtoI * vec4(0,1,0,0);
    vec4 preimZ   = WtoI * vec4(0,0,1,0);

    float m_preimX = length(preimX);
    float m_preimY = length(preimY);
    float m_preimZ = length(preimZ);

    float mag_preimY = length(preimY);
    vec2 lcldelta(rad*m_preimX,rad*m_preimZ); // size of radius in local coordinates
    vec2 idxdelta((lcldelta.x)*this->width,(lcldelta.y)*this->height);  // size of radius in index coordinates
    vec2 idxCenter1((localPos1.x)*this->width,(localPos1.z)*this->height); // center of object in index coords
    vec2 idxCenter2((localPos2.x)*this->width,(localPos2.z)*this->height); // center of object in index coords

    // range of points to check, in index coordinates

    ivec2 idxmin1(int(idxCenter1.x-idxdelta.x),   int(idxCenter1.y-idxdelta.y));
    ivec2 idxmax1(int(idxCenter1.x+idxdelta.x+1), int(idxCenter1.y+idxdelta.y+1));

    ivec2 idxmin2(int(idxCenter2.x-idxdelta.x),   int(idxCenter2.y-idxdelta.y));
    ivec2 idxmax2(int(idxCenter2.x+idxdelta.x+1), int(idxCenter2.y+idxdelta.y+1));

    ivec2 idxmin(idxmin1.x<idxmin2.x?idxmin1.x:idxmin2.x,idxmin1.y<idxmin2.y?idxmin1.y:idxmin2.y);
    ivec2 idxmax(idxmax1.x>idxmax2.x?idxmax1.x:idxmax2.x,idxmax1.y>idxmax2.y?idxmax1.y:idxmax2.y);

    if(idxmin.x<0)idxmin.x=0;
    if(idxmin.y<0)idxmin.y=0;
    if(idxmin.x>=width)idxmin.x=width-1;
    if(idxmin.y>=height)idxmin.y=height-1;
    
    if(idxmax.x<0)idxmax.x=0;
    if(idxmax.y<0)idxmax.y=0;
    if(idxmax.x>=width)idxmax.x=width-1;
    if(idxmax.y>=height)idxmax.y=height-1;


    // fprintf(stderr,"checking (%d,%d) to (%d,%d)\n",idxmin.x,idxmin.y,idxmax.x,idxmax.y);
    
    float lclHeight = fminf(localPos1.y,localPos2.y) - rad*mag_preimY;

    for(int x=idxmin.x; x<idxmax.x-1; ++x){
      for(int y=idxmin.y; y<idxmax.y-1; ++y){
        int idx = x + y*width;
        float d =data.values[idx];
        float dist =  d - lclHeight;
        if(dist>0){
          vec3 p1=ItoW * vec4(data.verts[idx],1);
          vec3 p2=ItoW * vec4(data.verts[idx+1],1);
          vec3 p3=ItoW * vec4(data.verts[idx+width],1);
          vec3 normal=-normalize(cross((p2-p1),(p3-p1)));
          // float d = dot(normal,data.verts[])
          fprintf(stderr,"normal %f (%f,%f,%f)\n",dist, normal.x,normal.y,normal.z);
          vec4 pushed = ItoW * vec4(normal.x,normal.y,normal.z,0)*dist;
          return vec3(pushed.x,pushed.y,pushed.z);
        }
      }
    }

    return vec3();

  }
  else{
    return vec3(0,0,0);
  }
}
vec3 HeightmapVolume::pushAlong(Volume *other, vec3 direction){
  return vec3();
}
bool HeightmapVolume::collision(Volume *other){
  return false;
}
bool HeightmapVolume::containsPoint(vec3 pt){
  return false;
}
Model HeightmapVolume::collisionMesh(){
  return Model(Model::FancyMesh(0,glm::mat4(),Model::RenderState()));
}


//////////////////////////////////////////*
//           Flat Volume
/////////////////////////////////////////*/

FlatVolume::FlatVolume(Pos posi, Polygon polygon) : Volume(posi), polygon(polygon){

}
const char *FlatVolume::type(){return "flat";}

double FlatVolume::distance(Volume *other){
  return 1.f;
}

vec3 FlatVolume::push(Volume *other){
  if(pos.updated){
    // polygon.recalculate();
    polygon.transform(pos.transform);
    pos.updated=0;
  }
  if(other->type() == "sphere"){
    SphereVolume *v = static_cast<SphereVolume*>(other);
    DistanceResult dr = polygon.distance(v->pos.pos);
    float dist = fabsf(dr.distance) - v->R();
    // fprintf(stderr,"dist: %.3f | (%.3f,%.3f,%.3f)\n",dist,dr.b.x,dr.b.y,dr.b.z);
    if(dist>0)return vec3();
    return normalize(dr.b-dr.a)*dist;
  }
  else if(other->type() == "cylinder"){
    CylinderVolume *v = static_cast<CylinderVolume*>(other);
    vec4 p1 = v->pos.transform * v->Rotation() * vec4(0,0.5*v->H(),0,1);
    vec4 p2 = v->pos.transform * v->Rotation() * vec4(0,-0.5*v->H(),0,1);
    DistanceResult dr = polygon.distance(Segment{vec3(p1.x,p1.y,p1.z),vec3(p2.x,p2.y,p2.z)});
    // fprintf(stderr,"(%.3f,%.3f,%.3f) - (%.3f,%.3f,%.3f) : (%.3f,%.3f,%.3f)\n",
      // p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,other->pos.pos.x,other->pos.pos.y,other->pos.pos.z);
    // fprintf(stderr," >      >   d: %.3f\n",dr.distance);
    float dist = dr.distance - v->R();
    if(dist>0)return vec3();
    return normalize(dr.b-dr.a)*dist;

  }
  else{
    return vec3(0,0,0);
  }
}
vec3 FlatVolume::pushAlong(Volume *other, vec3 direction){
  return vec3();
}
bool FlatVolume::collision(Volume *other){
  return false;
}
bool FlatVolume::containsPoint(vec3 pt){
  return false;
}
Model FlatVolume::collisionMesh(){
  return Model(Model::FancyMesh(0,glm::mat4(),Model::RenderState()));
}