#include <glm/gtc/matrix_transform.hpp>
#include "Volume.hxx"
#include <stdio.h>
#include <string.h>

using namespace Space;

Volume::Volume(Pos p) : pos(p){ }

//////////////////////////////////////////*
//            Sphere Volume
/////////////////////////////////////////*/

const char *SphereVolume::type(){ return "sphere"; }
SphereVolume::SphereVolume(vec3 pp, double rad) : Volume(Pos({pp,quaternion(),0.f,1.f})){
  r = rad>0?rad:0;
  if(SphereVolume::mesh == 0){
    SphereVolume::mesh = new Mesh(GL_TRIANGLES);
    SphereVolume::mesh->loadOBJ("../assets/models/sphere.obj");
    SphereVolume::mesh->data.polygon_mode = GL_LINE;
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
TransformedMesh SphereVolume::collisionMesh(){
  TransformedMesh tm(mesh);
  tm.transform = glm::scale(glm::mat4(1),vec3(r,r,r));
  for(int i=0;i<4;i++){
    // printf("%f,%f,%f,%f\n",tm.transform[i][0],tm.transform[i][1],tm.transform[i][2],tm.transform[i][3]);
  }
  return tm;
}

vec3 SphereVolume::push(Volume *other){
  if(other->type() == "sphere"){
    SphereVolume *sph = static_cast<SphereVolume*>(other);
    float dist = distance(sph);
    if(dist>0)return vec3();

    return (pos.pos - other->pos.pos)*dist;
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
CylinderVolume::CylinderVolume(vec3 pp, double rad, double h) : Volume(Pos({pp,quaternion(),0.f,1.f})){
  r = rad>0?rad:0;
  h = h>0?h:0;
  if(CylinderVolume::mesh == 0){
    CylinderVolume::mesh = new Mesh(GL_TRIANGLES);
    CylinderVolume::mesh->loadOBJ("../assets/models/sphere.obj");
    CylinderVolume::mesh->data.polygon_mode = GL_LINE;
  }
}
double CylinderVolume::distance(Volume *other){
    if(!strcmp(other->type(),"sphere")){
        SphereVolume* v = static_cast<SphereVolume*>(other);
        double dx = pos.pos.x - v->pos.pos.x;
        double dy = pos.pos.y - v->pos.pos.y;
        double dz = pos.pos.z - v->pos.pos.z;
        double dist = sqrt(dx*dx + dy*dy + dz*dz);

        return dist - r - v->R();

    }else
    if(!strcmp(other->type(),"cylinder")){
           // CylinderVolume* v = static_cast<CylinderVolume*>(other);
      return 0;
    }
}
TransformedMesh CylinderVolume::collisionMesh(){
  TransformedMesh tm(mesh);
  tm.transform = glm::scale(glm::mat4(1),vec3(r,r,r));
  for(int i=0;i<4;i++){
    // printf("%f,%f,%f,%f\n",tm.transform[i][0],tm.transform[i][1],tm.transform[i][2],tm.transform[i][3]);
  }
  return tm;
}

vec3 CylinderVolume::push(Volume *other){
  if(other->type() == "sphere"){
    SphereVolume *sph = static_cast<SphereVolume*>(other);
    float dist = distance(sph);
    if(dist>0)return vec3();

    return (pos.pos - other->pos.pos)*dist;
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

Mesh* CylinderVolume::mesh = 0;

//////////////////////////////////////////*
//           Heightmap Volume
/////////////////////////////////////////*/

HeightmapVolume::HeightmapVolume(vec3 pp, vec3 scale, int width, int height,
  float *data) : Volume(Pos({pp,quaternion(),0.f,1.f})){
  // nothing yet.
  this->width = width;
  this->height=height;
  this->scale = scale;
}
const char *HeightmapVolume::type(){return "heightmap";}

double HeightmapVolume::distance(Volume *other){
  return 1.f;
}

vec3 HeightmapVolume::push(Volume *other){
  return vec3();
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
TransformedMesh HeightmapVolume::collisionMesh(){
  return TransformedMesh(0);
}