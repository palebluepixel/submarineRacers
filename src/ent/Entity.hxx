#ifndef _ENTITY_HXX_
#define _ENTITY_HXX_


#include <glm/glm.hpp>
#include <physics/physics.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphics/mesh.hxx>

using namespace glm;
using namespace Space;


//not sure if we want this here
enum EntityType {
    TYPE1=0,
    TYPE2,
    TYPE3
    //torpedo,
    //mine,
    //boost,
    //etc
};

//can be used to "turn an object off" or delay placing it in the world
//until some trigger. collision behavior can be based off this state
enum EntityStatus {
    UNSPAWNED=0,
    SPAWNED,
    DESTROYED
};


class Entity {

public:

    Entity(vec2 initial_position, mat3 initial_orientation, int id, char*name, 
        EntityType type, EntityStatus status, float tick_interval);
    ~Entity();

    vec2 setPosition(vec2 pos);
    mat3 setOrientation(mat3 ori);
    EntityType setEntityType(EntityType type);
    int setID(int id);
    char* setName(char* name);

    //overwrite client data with server
    virtual int overwrite(vec2 pos, mat3 ori);
    //creates server message describing current pos and ori
    virtual int prepare_message_segment();

    //physics tick behavior
    virtual int onTick();

    EntityStatus status;
    //change object's status to spawned and place it in its intial position
    virtual EntityStatus spawn();


    //Render all meshes to screen
    void drawEntity();

    inline int getNMeshes() { return this->nMeshes; }
    Mesh ** meshes;

protected:

    vec2 position;
    vec2 initial_position;

    mat3 orientation;    
    mat3 initial_orientation;

    int id;
    char* name;
    EntityType type;

    //Bounding volume, not sure how we are representing it
    Volume *volume;

    float tick_interval;

    bool collidable;
    bool movable;
    bool drawable;

    //allow objects to be composed of multiple meshes
    int nMeshes;
};

//TODO remove this
#include "Agent.hxx"

#endif //!_ENTITY_HXX_