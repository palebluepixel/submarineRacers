#include "Entity.hxx"

using namespace glm;

Entity::Entity(vec3 initial_position, tquat<float> initial_orientation, int id, char*name, 
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

    this->velocity = glm::vec3(0, 0, 0);
    //this->angular_velocity
    this->forces = glm::vec3(0, 0, 0);
    //this.>torques

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

vec3 Entity::getPosition(){
    return this->position;
}

tquat<float> Entity::setOrientation(tquat<float> ori)
{
    tquat<float> old = this->orientation;
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
int Entity::overwrite(vec3 pos, tquat<float> ori)
{
    this->setPosition(pos);
    this->setOrientation(ori);

    return 0;
}

//creates server message describing current pos and ori
int Entity::prepare_message_segment(){
    return 0;
}

//physics tick behavior
int Entity::onTick(float dt){
    //TODO fix all pseudocode
    //Collision checks already done

    // Calculate physics forces
    vec3 drag = velocity * length(velocity) * (-dragCoef);
    applyForce(drag);
    //Do we want some sort of bouyancy to restrict objects to underwater?

    // Apply Forces
    vec3 acceleration = forces / mass;

    position += (velocity + acceleration * (dt/2)) * dt;
    velocity += acceleration * dt;

    // Apply Rotations
    // somethin like this angular_velocity += torques
    //TODO see if this works? Almost definitely doesn't :(
    //orientation = angular_velocity * dt * orientation;

    // Reset
    forces = glm::vec3(0, 0, 0);
    //torques.zero();
    return 0;
}

//change object's status to spawned and place it in its intial position
EntityStatus Entity::spawn(){
    this->overwrite(this->initial_position, this->initial_orientation);
    EntityStatus old = this->status;
    this->status = SPAWNED;
    return old;
}

mat4 Entity::modelMatrix(){
    // orientation = glm::rotate(orientation,3.14f/16.f,vec3(1.f,1.f,1.f));
    // printf("quat: %f,%f,%f,%f\n",orientation[0],orientation[1],orientation[2],orientation[3]);
    mat4 model = mat4_cast(orientation);
    model[3][0] += this->position[0];
    model[3][1] += this->position[1];
    model[3][2] += this->position[2];
    model[3][3] = 1.0;

    // for(int i=0;i<4;i++){
    //     printf("%f,%f,%f,%f\n",model[0][0],model[0][1],model[0][2],model[0][3]);
    // }
    // printf("\n");
    //TODO: rotation

    // model[0][3] = 0;
    // model[1][3] = 0;
    // model[2][3] = 0;
    return model;
}


void Entity::drawEntity(){
    int i;
    for(i=0; i<this->nMeshes; i++)
        meshes[i]->draw();
}

void Entity::applyForce(vec3 force) {
    forces += force;
}