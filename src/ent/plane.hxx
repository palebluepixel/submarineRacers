#ifndef _PLANE_HXX_
#define _PLANE_HXX_

#include <ent/Entity.hxx>

using namespace glm;

/* Simple example mesh */

class Plane : public Entity {

public:

    Plane(int ID, vec3 initial_position, quaternion initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color, char* modelfile, char* texfile);
    ~Plane();
    virtual int onTick(float dt);

protected:
    vec3 color;
    char *modelfile;
};



#endif //_CUBE_HXX_