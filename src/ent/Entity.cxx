#include "Entity.hxx"
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

    _mass = 1.f;
    dragCoef=0.f;
}

Entity::~Entity()
{
    
}


vec3 Entity::setPosition(vec3 pos){
    // if(pos != this->position)
        this->shouldSendUpdate = 1;

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
    volume->pos.pos = position;
    volume->pos.orient = orientation;
}

tquat<float> Entity::setOrientation(tquat<float> ori)
{
    //this->shouldSendUpdate = 1;
    tquat<float> old = this->orientation;
    this->orientation = ori;
    return old;
}

EntityType Entity::setEntityType(EntityType type){
    EntityType old = this->type;
    this->type = type;
    return old;
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
    vec3 drag = velocity * length(velocity) * (-dragCoef);
    applyForce(drag);
    //Do we want some sort of bouyancy to restrict objects to underwater?

    // Apply Forces
    vec3 acceleration = forces / _mass;
    if(acceleration != vec3()){
        shouldSendUpdate = 1;
    }
    // printf("(%.3f,%.3f,%.3f),(%.3f,%.3f,%.3f),(%.3f,%.3f,%.3f)/%.3f\n",velocity.x,velocity.y,velocity.z,drag.x,drag.y,drag.z,forces.x,forces.y,forces.z,_mass);
    position += (velocity + acceleration * (dt/2)) * dt;
    velocity += acceleration * dt;

    // Reset
    forces = glm::vec3(0, 0, 0);
    //torques.zero();
    return 0;
}

// create a new Entity and spawn it in the world.

EntityStatus Entity::spawn(){
    // this code should be handled by the Level and by
    // the physics engine. It should not be here.
    // this->overwrite(this->initial_position, this->initial_orientation, this->initial_velocity);
    // EntityStatus old = this->status;
    // this->status = SPAWNED;
    // return old;
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
    for(TransformedMesh tmesh : meshes)
        tmesh.mesh->draw();
}

void Entity::applyForce(vec3 force) {
    forces += force;
}

int Entity::assignNewID()
{
    static int ID = -1;
    return ++ID;
}