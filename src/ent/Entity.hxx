#ifndef _ENTITY_HXX_
#define _ENTITY_HXX_


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <physics/Volume.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphics/mesh.hxx>
#include <graphics/texture.hxx>
#include <vector>
#include <network/MessageProtocols.hxx>

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

typedef glm::tquat<float> quaternion;

class Entity {

public:

    Entity(vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval);
    ~Entity();

    EntityStatus status;

public:
    char*        setName(char* name);
    int          setID(int id);
    EntityType   setEntityType(EntityType type);

    vec3         setPosition(vec3 pos);
    quaternion   setOrientation(quaternion ori);
    vec3         setVelocity(vec3 vel);

    vec3         getPosition();
    quaternion   getOrientation();
    vec3         getVelocity();
    int          getID();

    inline bool  isCollidable()       { return this->collidable;       }
    inline bool  isDrawable()         { return this->drawable;         }
    inline bool  isMovable()          { return this->movable;          }
    inline bool  isShouldSendUpdate() { return this->shouldSendUpdate; }

    vec3 getDirection();    // Get forward direction vector from orientation

    /**     networking:     **/
    virtual int overwrite(vec3 pos, quaternion ori, vec3 vel);    //overwrite client data with server
    virtual int overwrite(posUpBuf *msg);
    virtual message* prepareMessageSegment();                  //creates server message describing current pos, ori, and velocity
    
    /**     physics:        **/
    virtual int onTick(float dt);
    virtual vec3 getDrag();
    void applyForce(vec3 force);
    void applyTorque(vec3 torque);

    /**     game state:     **/
    virtual EntityStatus spawn();     // set status to spawned, place in intial position

    /**     graphics:       **/
    mat4 modelMatrix();           // return transform matrix TO world space.
    void drawEntity();            // render meshes to screen

    inline std::vector<TransformedMesh> getMeshes() {return this->meshes;}
    virtual void initalizeVisualData() = 0; //load meshes and textures

public:
    Volume *volume;
    quaternion orientation;
    std::vector<TransformedMesh> meshes;
protected:
    vec3 position;
    vec3 initial_position;      // we should remove this field.

    vec3 velocity; // Used for interpolation
    vec3 initial_velocity;

    quaternion initial_orientation;

    vec3 angular_velocity;

    float mass;
    float dragCoef;

    int id;
    char* name;
    EntityType type;

    float tick_interval;

    bool collidable;
    bool movable;
    bool drawable;

    /* if the server needs to send an update to the client this tick. This should be
    set to TRUE whenever we change this entity's position, velocity, or orientation.
    It should be set to FALSE when we call prepareMessageSegment. */
    bool shouldSendUpdate;

    virtual void initalizeMeshes()=0;

    texture2d *tex;
    image2d *img;
    virtual void initalizeTextures(const char* texfile)=0;

    //per tick quantities
    vec3 forces;    // Sum of all forces on this object
    vec3 torques; // Sum of all torques on this object

    /* Generates a unique ID for this object. */
    int assignNewID();
};

//TODO remove this
#include "Agent.hxx"

#endif //!_ENTITY_HXX_