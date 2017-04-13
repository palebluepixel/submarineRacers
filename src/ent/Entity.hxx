#ifndef _ENTITY_HXX_
#define _ENTITY_HXX_


#include <glm/glm.hpp>
#include <physics/physics.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphics/mesh.hxx>
#include <graphics/texture.hxx>

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

    Entity(vec3 initial_position, mat3 initial_orientation, int id, char*name, 
        EntityType type, EntityStatus status, float tick_interval);
    ~Entity();

    vec3 setPosition(vec3 pos);
    mat3 setOrientation(mat3 ori);
    EntityType setEntityType(EntityType type);
    int setID(int id);
    char* setName(char* name);

    //overwrite client data with server
    virtual int overwrite(vec3 pos, mat3 ori);
    //creates server message describing current pos and ori
    virtual int prepare_message_segment();

    //physics tick behavior
    virtual int onTick();

    EntityStatus status;
    //change object's status to spawned and place it in its intial position
    virtual EntityStatus spawn();

    //return the model matrix to translate and rotate this object in world-space
    mat4 modelMatrix();


    //Render all meshes to screen
    void drawEntity();

    inline int getNMeshes() { return this->nMeshes; }
    inline Mesh ** getMeshes() {return this->nMeshes>0 ? this->meshes : NULL;}
    virtual void initalizeVisualData() = 0; //load meshes and textures

protected:

    vec3 position;
    vec3 initial_position;

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
    Mesh ** meshes;
    virtual void initalizeMeshes()=0;

    texture2d *tex;
    image2d *img;
    virtual void initalizeTextures(const char* texfile)=0;
};

//TODO remove this
#include "Agent.hxx"

#endif //!_ENTITY_HXX_