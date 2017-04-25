#ifndef _LEVEL_HXX_
#define _LEVEL_HXX_

#include <stdlib>
#include <network/MessageProtolcols.h>
#include <ent/Entity.hxx>
#include <vector>
#include <unordered_map>

using namespace std;

class Level {

public:
	Level(); 
	~Level();

	/* Populate all fields of the class by loading them from a file. */
	void buildLevelFromFile();

	/* Update the data for an entity based on a CODE_OBJECT_CHANGE message */
	void upEntData(posUpBuf *info);

	/* Spawn or despawn an object. Returns the old EntityStatus */
	EntityStatus changeObjectSpawn(int id, EntityStatus new);

	/* Starting positions for each submarine. In the future, this might be
	a map of <Submarine, vec3> */
	vector<vec3> submarineStarts;


	/* Get the entity with the given ID */
	Entity * getEntityByID(int id);

	/* Add this entity to the entity list. This function checks that an entity
	with the current ID does not already exist. Returns 1 if the entity was
	added, 0 if it couldn't due to the ID already being taken. */
	int addEntity(Entity *entity);


	/**** TODO ****/
	/* Color and camera information */
	/* heightmap */

private:
	/* A list of every entity that exists in the level. For ease of finding 
	specific entities, we store them as a hash table (unoredered map)*/
	unordered_map<int, Entity *> entities;


};


#endif