#include "Entity.hxx"
#include <cmath>
#include <util/log.hxx>

using namespace glm;

Entity::Entity(int ID, vec3 initial_position, tquat<float> initial_orientation, std::string name, 
    EntityType type, EntityStatus status, float tick_interval) : meshes(), velocity(vec3())
{
    this->id = (ID);
    this->name = (name);
    
    this->type = type;
    this->status = status;
    this->tick_interval = tick_interval;

    setPosition(initial_position);
    setOrientation(initial_orientation);

    this->velocity = glm::vec3(0, 0, 0);
    //this->angular_velocity
    this->forces = glm::vec3(0, 0, 0);
    //this.>torques

    this->drawable = 1;

    /* This is intialized to 1 so we can be positive the server and client
    agree about the starting position of every object. */
    this->shouldSendUpdate = 1;
    this->volume = 0;
    _mass = 1.f;
    _dragCoef=0.f;
    this->angularDragCoef = 5.f;
}

Entity::~Entity()
{
    
}


void Entity::setPhysicsParams(PhysicsParams in){
    dragCoef(in.dragCoef);
}
vec3 Entity::setPosition(vec3 pos){
    // if(pos != this->position)
        //this->shouldSendUpdate = 1;

    vec3 old = this->position;
    this->position = pos;
    return old;
}
vec3 Entity::getPosition(){
    return this->position;
}
vec3 Entity::pos(){return getPosition();}
vec3 Entity::pos(vec3 in){return setPosition(in);};

vec3 Entity::vel(){return getVelocity();}
vec3 Entity::vel(vec3 in){return setVelocity(in);};

float Entity::mass(){return getMass();}
float Entity::mass(float in){return setMass(in);};

float Entity::dragCoef(){return _dragCoef;}
void Entity::dragCoef(float in){_dragCoef = in;}

vec3 Entity::setVelocity(vec3 vel) {
    if(vel != this->velocity)
        this->shouldSendUpdate = 1;

    vec3 old = this->velocity;
    this->velocity = vel;
    return old;
}


float Entity::setMass(float newmass){
    this->_mass = newmass;
}
float Entity::getMass(){
    return _mass;
}

void Entity::setVolume(Volume *vol){
    this->volume = vol;
    updatePhysicsVolume();
}
Volume* Entity::getVolume(){
    return volume;
}

void Entity::updatePhysicsVolume(){
    if(volume){
        volume->pos.pos = position;
        volume->pos.orient = orientation;
    }
}

tquat<float> Entity::setOrientation(tquat<float> ori)
{
    this->shouldSendUpdate = 1;
    tquat<float> old = this->orientation;
    this->orientation = ori;
    return old;
}

EntityType Entity::setEntityType(EntityType type){
    EntityType old = this->type;
    this->type = type;
    return old;
}

EntityType Entity::getEntityType()
{
    return this->type;
}

int Entity::setID(int id){
    int old = this->id;
    this->id = id;
    return old;
}

int Entity::getID(){
    return this->id;
}

quaternion Entity::getOrientation(){
    return this->orientation;
}    

vec3 Entity::getVelocity()
{
    return this->velocity;
}

std::string Entity::setName(std::string name){
    std::string old = this->name;
    return old;
}
std::string Entity::getName(){
    return name;
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
    // log(LOGLOW,"preparing message statement %s",name.c_str());
    this->shouldSendUpdate = 0;
    return createPosUpMsg(this);
}

//physics tick behavior
int Entity::onTick(float dt){

    // update server position every second.
    static float serverupdatetick=0;
    serverupdatetick+=dt;
    if(serverupdatetick>1.f){
        serverupdatetick=0.f;
        shouldSendUpdate=1;
    }
    //TODO fix all pseudocode
    //Collision checks already done

    // Calculate physics forces
    applyForce(getDrag());
    applyTorque(getDragTorque());
    //Do we want some sort of bouyancy to restrict objects to underwater?
    
    // Apply Forces
    vec3 acceleration = forces / this->_mass;
    if(acceleration != vec3()){
        shouldSendUpdate = 1;
    }

    this->setPosition(position + (velocity + acceleration * (dt/2)) * dt);
    this->setVelocity(velocity + acceleration * dt);

    // Apply rotations
    vec3 angular_accel = torques / this->_mass;

    vec3 rot_vec = (this->angular_velocity + angular_accel * (dt/2)) * dt;

    float rot_vec_len = length(rot_vec);
    if(rot_vec_len != 0){
        angular_velocity += angular_accel * dt;
        quaternion rotation = angleAxis(rot_vec_len, rot_vec / rot_vec_len);
        this->setOrientation(rotation * this->orientation); //Order matters!
    }

    // logln(LOGLOW,"%s: %f",getName().c_str(),_dragCoef);

    // Reset
    forces = glm::vec3(0, 0, 0);
    torques = glm::vec3(0, 0, 0);
    
    return 0;
}

// Some entites can have more complex drag functions
vec3 Entity::getDrag() {
    return velocity * (length(velocity) * (-_dragCoef) * 0.5f);
}

// Some entites can have more complex drag functions
vec3 Entity::getDragTorque() {
    float l = length(angular_velocity);
    // if(l < 0.01)return vec3();
    return -angularDragCoef * l * angular_velocity;
}

//change object's status to spawned and place it in its intial position
EntityStatus Entity::spawn(){
    // this code should be handled by the Level and by
    // the physics engine. It should not be here.
    // this->overwrite(this->initial_position, this->initial_orientation, this->initial_velocity);
    // EntityStatus old = this->status;
    // this->status = SPAWNED;
    // return old;
}

mat4 Entity::modelMatrix(){
    mat4 model = mat4_cast(orientation);
    model[3][0] += this->position[0];
    model[3][1] += this->position[1];
    model[3][2] += this->position[2];
    model[3][3] = 1.0;

    return model;
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