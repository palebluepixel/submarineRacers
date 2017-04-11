#include "Entity.hxx"

using namespace glm;

Entity::Entity(vec3 initial_position, mat3 initial_orientation, int id, char*name, 
    EntityType type, EntityStatus status, float tick_interval)
{
    this->initial_position = initial_position;
    this->initial_orientation = initial_orientation;
    this->id = id;
    this->name = strdup(name); //not takin any fuckin chances
    this->type = type;
    this->status = status;
    this->tick_interval = tick_interval;

    this->position = this->initial_position;
    this->orientation = this->initial_orientation;

    this->nMeshes = 0;
    this->meshes = NULL;
}

Entity::~Entity()
{
    free(this->name);
}


vec3 Entity::Entity::setPosition(vec3 pos)
{
    vec3 old = this->position;
    this->position = pos;
    return old;
}


mat3 Entity::setOrientation(mat3 ori)
{
    mat3 old = this->orientation;
    this->orientation = ori;
    return old;
}

EntityType Entity::setEntityType(EntityType type)
{
    EntityType old = this->type;
    this->type = type;
    return old;
}

int Entity::setID(int id)
{
    int old = this->id;
    this->id = id;
    return old;
}

char* Entity::setName(char* name)
{
    char*old = this->name;
    this->name = strdup(name);
    return old;
}

//overwrite client data with server
int Entity::overwrite(vec3 pos, mat3 ori)
{
    this->setPosition(pos);
    this->setOrientation(ori);

    return 0;
}

//creates server message describing current pos and ori
int Entity::prepare_message_segment()
{
    return 0;
}

//physics tick behavior
int Entity::onTick()
{
    return 0;
}

//change object's status to spawned and place it in its intial position
EntityStatus Entity::spawn()
{
    this->overwrite(this->initial_position, this->initial_orientation);
    EntityStatus old = this->status;
    this->status = SPAWNED;
    return old;
}

mat4 Entity::modelMatrix()
{
    //TODO: rotation
    mat4 model = mat4();
    model[3][0] += this->position[0];
    model[3][1] += this->position[1];
    model[3][2] += this->position[2];
    return model;
}


void Entity::drawEntity()
{
    int i;
    for(i=0; i<this->nMeshes; i++)
        meshes[i]->draw();
}