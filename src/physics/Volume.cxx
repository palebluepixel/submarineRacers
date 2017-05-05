#include <glm/gtc/matrix_transform.hpp>
#include "Volume.hxx"
#include <stdio.h>
#include <string.h>
#include <util/log.hxx>

using namespace Space;

Volume::Volume(Pos p) : pos(p){ }

Volume::Pos::Pos(Entity *e){
  set(e);
}
void Volume::Pos::set(Entity *e){
  if(!e){
    logln(LOGHIGH,"Error: Pos passed null argument.");
  }
  transform = e->modelMatrix();
  pos = e->pos();
  ori = e->getOrientation();
  // logln(LOGLOW,"making volume");
  // for(int i=0;i<4;i++){
  //   fprintf(stderr,"%.3f %.3f %.3f %.3f\n",transform[0][i],transform[1][i],transform[2][i],transform[3][i]);
  // }
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
  TransformedMesh tm(TransformedMesh::MeshInfo(mesh,glm::scale(glm::mat4(1),vec3(r,r,r))));
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
  }else 
  if(!strcmp(other->type(),"cylinder")){
    CylinderVolume* v = static_cast<CylinderVolume*>(other);
    return -v->push(this);
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
    CylinderVolume::meshcyl->data.polygon_mode = GL_LINE;
  }
  if(!CylinderVolume::meshcap){
    CylinderVolume::meshcap = new Mesh(GL_TRIANGLES);
    CylinderVolume::meshcap->loadOBJ("../assets/models/hemisphere.obj");
    CylinderVolume::meshcap->data.polygon_mode = GL_LINE;
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

    }else
    if(!strcmp(other->type(),"cylinder")){
           // CylinderVolume* v = static_cast<CylinderVolume*>(other);
      return 0;
    }
}
TransformedMesh CylinderVolume::collisionMesh(){
  TransformedMesh tm(TransformedMesh::MeshInfo(meshcyl,rotation*glm::scale(glm::mat4(1),vec3(r,h/2.f,r))));
  glm::mat4 topcap = glm::translate(mat4(1),glm::vec3(0,h/2,0)) * glm::scale(mat4(1),glm::vec3(r,r,r));
  glm::mat4 botcap = glm::translate(mat4(1),glm::vec3(0,-h/2,0)) * glm::scale(mat4(1),glm::vec3(r,-r,r));
  tm.meshes.push_back(TransformedMesh::MeshInfo(meshcap,rotation*topcap));
  tm.meshes.push_back(TransformedMesh::MeshInfo(meshcap,rotation*botcap));
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

    fprintf(stderr,"(%.3f,%.3f,%.3f) - (%.3f,%.3f,%.3f) : (%.3f,%.3f,%.3f)\n",
      bottom.x,bottom.y,bottom.z,top.x,top.y,top.z,other->pos.pos.x,other->pos.pos.y,other->pos.pos.z);
    fprintf(stderr," -- (%.3f,%.3f,%.3f) (%.3f,%.3f%.3f) : %.3f\n",dr.a.x,dr.a.y,dr.a.z,dr.b.x,dr.b.y,dr.b.z,dr.distance);

    dr.distance -= (v->R() + r);

    if(dr.distance < 0){
      return dr.distance*normalize(dr.b - dr.a);
    }
    else return vec3();
  }else
  if(other->type() == "cylinder"){
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
      return dr.distance*normalize(dr.a - dr.b);
    }
    else return vec3();
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

Mesh* CylinderVolume::meshcyl = 0;
Mesh* CylinderVolume::meshcap = 0;

//////////////////////////////////////////*
//           Heightmap Volume
/////////////////////////////////////////*/

HeightmapVolume::HeightmapVolume(Pos posi, vec3 scale, int width, int height,
  float *data) : Volume(posi){
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
  return TransformedMesh(TransformedMesh::MeshInfo(0,glm::mat4()));
}