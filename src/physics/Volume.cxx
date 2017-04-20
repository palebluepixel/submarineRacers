#include "Volume.hxx"
#include <stdio.h>
#include <string.h>

using namespace Space;

const char *SphereVolume::type(){ return "sphere"; }
SphereVolume::SphereVolume(vec3 pp, double rad){
       pos.pos = pp;
       r = rad>0?rad:0;
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
const char* CylinderVolume::type(){ return "cylinder"; }

const char* HeightmapVolume::type(){ return "heightmap"; }
