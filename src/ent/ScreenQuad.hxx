#ifndef _SCREENQUAD_HXX_
#define _SCREENQUAD_HXX_

/* TODO: I didn't have time to make this an actual screen quad. It's just a cube. */

#include <ent/Entity.hxx>

using namespace glm;

class ScreenQuad : public Entity {

public:

    ScreenQuad(int ID, vec3 initial_position, quaternion initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval, vec3 color, char* modelfile, char* texfile);
    ~ScreenQuad();

    void initalizeVisualData();

protected:

    vec3 color;
    char *modelfile;
    char *texfile;

    virtual void initalizeTextures(const char* texfile);
    virtual void initalizeMeshes();
};



#endif //_CUBE_HXX_