#include "Level.hxx"

Level::Level() { }

Level::~Level() { }

/* Populate all fields of the class by loading them from a file. */
void Level::buildLevelFromFile() 
{ 
	//create test objects
    vec3 cubePos[] = {vec3(-5,5,10), vec3(5, 0, 5), vec3(5, -5, 5), vec3(5, -10, 5), vec3(5, -20, 5),
        vec3(5, -40, 5)}; 
    vec3 cubeColor[] = {vec3(1,1,1), vec3(1,1,1), vec3(1,1,0), vec3(1,0,1), vec3(0,1,1), vec3(0,0,1)};
    int ncubes = 6, i;
    Entity * cubes[ncubes];
    cubes[0] = new Gadget(cubePos[0], quaternion(), strdup("kyubey"), TYPE1, SPAWNED, 0.1f, cubeColor[0], "../assets/models/sub_3.obj");
    cubes[0]->volume = new Space::SphereVolume(vec3(0,0,0),2.f);
    //cubes[0]->meshes.push_back(cubes[0]->volume->collisionMesh());
    for(i=1; i<ncubes; i++){
        cubes[i] = new Gadget(cubePos[i], quaternion(), strdup("kyubey"), TYPE1, SPAWNED, 0.1f, cubeColor[i], "../assets/models/cube.obj");
        cubes[i]->volume = new Space::SphereVolume(vec3(0,0,0),1.414);
        //cubes[i]->meshes.push_back(cubes[i]->volume->collisionMesh());
    }

    for(i=0; i<ncubes; i++)
    	this->addEntity(cubes[i]);
}

/* Update the data for an entity based on a CODE_OBJECT_CHANGE message */
void Level::upEntData(posUpBuf *info)
{
	// Get the entity for this ID
	Entity *ent = this->getEntityByID(info->id);
	if(ent == NULL){
		log(LOGERROR, "%s %d\n", "Null entity in upEntData", info->id);
		return;
	}

	ent->overwrite(info);
}

/* Spawn or despawn an object. Returns the old EntityStatus */
EntityStatus Level::changeObjectSpawn(int id, EntityStatus n) { return SPAWNED; }


/* Get the entity with the given ID */
Entity * Level::getEntityByID(int id)
{
	if(!this->entityExists(id))
		return NULL;
	return this->entities[id];
}

/* Returns 1 if an entity with id exists in our entity list,
0 otherwise */
int Level::entityExists(int id)
{
	return this->entities.find(id) != entities.end();
}

/* Add this entity to the entity list. This function checks that an entity
with the current ID does not already exist. Returns 1 if the entity was
added, 0 if it couldn't due to the ID already being taken. Since we
identify entities by ID, we can't let one entity "overwrite" another. */
int Level::addEntity(Entity *entity)
{	
	if(this->entityExists(entity->getID()))
		return 0;
	this->entities[entity->getID()] = entity;
	return 1;
}