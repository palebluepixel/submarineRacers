#include "Entity.hxx"
#include <cmath>
#include <util/log.hxx>

using namespace glm;

Entity::Entity(vec3 initial_position, tquat<float> initial_orientation, char*name, 
    EntityType type, EntityStatus status, float tick_interval) : meshes()
{
    this->initial_position = initial_position;
    this->initial_orientation = initial_orientation;
    this->id = this->assignNewID();
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
    this->mass = 1;
    this->dragCoef = 0.1;

    this->drawable = 1;

    /* This is intialized to 1 so we can be positive the server and client
    agree about the starting position of every object. */
    this->shouldSendUpdate = 1;
}

Entity::~Entity()
{
    free(this->name);
}


vec3 Entity::setPosition(vec3 pos)
{
    this->shouldSendUpdate = 1;
    vec3 old = this->position;
    this->position = pos;
    return old;
}

vec3 Entity::getPosition(){
    return this->position;
}

vec3 Entity::setVelocity(vec3 vel) {
    this->shouldSendUpdate = 1;
    vec3 old = this->velocity;
    this->velocity = vel;
    return old;
}

tquat<float> Entity::setOrientation(tquat<float> ori)
{
    this->shouldSendUpdate = 1;
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

int Entity::getID()
{
    return this->id;
}

quaternion Entity::getOrientation()
{
    return this->orientation;
}    

vec3 Entity::getVelocity()
{
    return this->velocity;
}

char* Entity::setName(char* name)
{
    char*old = this->name;
    this->name = strdup(name);
    return old;
}

//overwrite client data with server
int Entity::overwrite(vec3 pos, tquat<float> ori, vec3 vel)
{
    this->setPosition(pos);
    this->setOrientation(ori);
    this->setVelocity(vel);

    return 0;
}

int Entity::overwrite(posUpBuf *msg)
{
    this->overwrite(msg->pos, msg->ori, msg->vel);

    return 0;
}

//creates server message describing current pos and ori
message * Entity::prepareMessageSegment(){
    this->shouldSendUpdate = 0;
    return createPosUpMsg(this);
}

//physics tick behavior
int Entity::onTick(float dt){
    //Collision checks already done

    // Calculate physics forces
    applyForce(getDrag());
    //Do we want some sort of bouyancy to restrict objects to underwater?
    
    // Apply Forces
    vec3 acceleration = forces / mass;

    this->setPosition(position + (velocity + acceleration * (dt/2)) * dt);
    this->setVelocity(velocity + acceleration * dt);

    // Apply rotations
    vec3 angular_accel = torques / mass;

    vec3 rot_vec = (this->angular_velocity + angular_accel * (dt/2)) * dt;

    float rot_vec_len = length(rot_vec);
    if(rot_vec_len != 0){
        angular_velocity += angular_accel * dt;
        quaternion rotation = angleAxis(rot_vec_len, rot_vec / rot_vec_len);
        this->setOrientation(rotation * this->orientation); //Order matters!
    }

    // Reset
    forces = glm::vec3(0, 0, 0);
    torques = glm::vec3(0, 0, 0);
    
    return 0;
}

// Some entites can have more complex drag functions
vec3 Entity::getDrag() {
    return velocity * length(velocity) * (-dragCoef);
}

//change object's status to spawned and place it in its intial position
EntityStatus Entity::spawn(){
    this->overwrite(this->initial_position, this->initial_orientation, this->initial_velocity);
    EntityStatus old = this->status;
    this->status = SPAWNED;
    return old;
}

mat4 Entity::modelMatrix(){
    mat4 model = mat4_cast(orientation);
    model[3][0] += this->position[0];
    model[3][1] += this->position[1];
    model[3][2] += this->position[2];
    model[3][3] = 1.0;

    return model;
}


void Entity::drawEntity(){
    //int i;
    for(TransformedMesh tmesh : meshes)
        tmesh.mesh->draw();
}

void Entity::applyForce(vec3 force) {
    if(isnan(force[0]) || isnan(force[1]) || isnan(force[2])){
        return;
    }
    forces += force;
}

int Entity::assignNewID()
{
    static int ID = -1;
    return ++ID;
}

void Entity::applyTorque(vec3 torque) {
    torques += torque;
}

vec3 Entity::getDirection() {
    return orientation * glm::vec3(0, 0, 1);
}