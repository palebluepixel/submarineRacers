#include <glm/gtc/matrix_transform.hpp>
#include "Volume.hxx"
#include <stdio.h>
#include <string.h>

using namespace Space;

const char *SphereVolume::type(){ return "sphere"; }
SphereVolume::SphereVolume(vec3 pp, double rad){
  pos.pos = pp;
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
    printf("%f,%f,%f,%f\n",tm.transform[i][0],tm.transform[i][1],tm.transform[i][2],tm.transform[i][3]);
  }
  return tm;
}

        vec3 SphereVolume::push(Volume *other){}
        vec3 SphereVolume::pushAlong(Volume *other, vec3 direction){}
        bool SphereVolume::collision(Volume *other){}
        bool SphereVolume::containsPoint(vec3 pt){}


const char* CylinderVolume::type(){ return "cylinder"; }

const char* HeightmapVolume::type(){ return "heightmap"; }


Mesh* SphereVolume::mesh = 0;