#ifndef _TERRAIN_HXX_
#define _TERRAIN_HXX_

#include <ent/Entity.hxx>
#include <graphics/mesh.hxx>

using namespace glm;

/* Simple example mesh */

class Terrain : public Entity {

public:

    Terrain(int ID,vec3 initial_position, quaternion initial_orientation, int id, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color);
    ~Terrain();

    void initalizeVisualData();

protected:

    vec3 color;

    void initalizeTextures(const char* texfile);
    void initalizeMeshes();
};



#endif //_CUBE_HXX_