#ifndef _LEVEL_HXX_
#define _LEVEL_HXX_

#include <stdlib.h>
#include <network/MessageProtocols.hxx>
#include <ent/Entity.hxx>
#include <ai/AI.hxx>
#include <ent/gadget.hxx>
#include <vector>
#include <unordered_map>
//#include <util/log.hxx>
#include <network/Server.hxx>
#include <graphics/view.hxx>
#include <graphics/renderer.hxx>
#include <ent/Checkpoint.hxx>
#include <world/Track.hxx>

using namespace std;


struct AI_entry {
    AI* ai;
    float tickrate;
    float time_left;
};


typedef struct{
    UnderwaterRenderer *ent;
    Renderer *flat;
    Renderer *water;
}RendererList;

class Level {

public:
    Level(); 
    Level(const char *path);
    ~Level();

    /* Release all memory used by this level.*/
    void unload();
    /* Checks if the entity should be deleted on world unload. Some things, like
    submarines, stick around across levels. */
    int shouldDeleteOnUnload(Entity *entity);

    /* Populate all fields of the class by loading them from a file. */
    void buildLevelFromFile();
    void buildDemoLevel();

    /* Generate a sequence of n regular hexagons with radius r, centered at center[i] for every i in [0,n]. */
    void generateDummyPath(float r, vec3 *centers, int n, int& cur_id);

    /* Update the data for an entity based on a CODE_OBJECT_CHANGE message */
    void upEntData(posUpBuf *info);

    /* Spawn or despawn an object. Returns the old EntityStatus */
    EntityStatus changeObjectSpawn(int id, EntityStatus n);

    /* Starting positions for each submarine. In the future, this might be
    a map of <Submarine, vec3> */
    vector<vec3> submarineStarts;


    /* Get the entity with the given ID */
    Entity * getEntityByID(int id);

    /* Returns 1 if an entity with id exists in our entity list,
    0 otherwise */
    int entityExists(int id);

    /* Add this entity to the entity list. This function checks that an entity
    with the current ID does not already exist. Returns 1 if the entity was
    added, 0 if it couldn't due to the ID already being taken. */
    int addEntity(Entity *entity);
    int removeEntity(Entity * entity);

    /* Adds an AI to the list of AIs to update*/
    void addAI(AI *ai, float tickrate);
    int removeAI(AI *ai);

    /* Using the given server, broadcast position updates to all clients. */
    void sendPosUps(Server *server);

    /* Render all renderable entities, the skybox ... 
    Right now we pass in two renderers, one for entities, and one for the skybox. 
    We may eventually want to change this to be a map <char*,renderer*> and allow
    entities to define the "name" of the renderer that is supposed to render them.*/
    void renderAll(View *view, RendererList renderers, int passes=1);
    
    /* Skybox */
    void setSkybox(Gadget *skybox);

    /* Track */
    inline void setTrack(Track *track) { this->track = track; }
    inline Track * getTrack() { return this->track; }

    void updateLevel(float dt);
    void interpolateLevel(float dt);

    /**** TODO ****/
    /* Color and camera information */
    /* heightmap */

private:
    const char *path;
    /* A list of every entity that exists in the level. For ease of finding 
    specific entities, we store them as a hash table (unordered map)*/
    unordered_map<int, Entity *> entities;
    std::vector<AI_entry> ais;  //REMARK this might need to become a list later if we start doing a lot of deleteions

    Gadget *skybox;

    Track *track;

    void physicsTick(float dt);
    void handleCollisions(float dt);
    void updateAIs(float dt);

};


#endif
