#include "Submarine.hxx"
#include "Actuator.hxx"
#include <util/log.hxx>


Submarine::Submarine(int ID, vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein) :
	Agent(ID, initial_position, initial_orientation, name, type, status, tick_interval) {
	
    this->color = color;
    this->modelfile = modelfilein;
    this->initalizeVisualData();

    this->actuator = new SubmarineActuator(this);
    this->angularDragCoef = 20.f;

    this->maxAccel = 3;
    this->maxTurn = 3;
    this->maxRise = 3;
    this->maxDive = 3;
}

void Submarine::switchWeapons(uint8_t weapon) {
    //TODO
}

void Submarine::initalizeTextures(const char* texfile)
{
    //load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Submarine::initalizeVisualData()
{
    this->initalizeTextures("../assets/textures/cubetex.png");
    this->initalizeMeshes();
}

void Submarine::initalizeMeshes()
{
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    mesh->loadOBJ(modelfile);
    mesh->data.color = vec4(this->color,0.5);
    mesh->data.tex = this->tex;
    meshes.push_back(TransformedMesh(TransformedMesh::MeshInfo(mesh, mat4())));
}

/* Return movement and handling parameters for this sub. These may eventually be
more complicated, for example they may take into account status conditions and 
powerups affecting this sub, and maximum acceleration may depend on depth. */
float Submarine::getMaxAccel() { return this->maxAccel; }
float Submarine::getMaxTurn() { return this->maxTurn; }
float Submarine::getMaxRise() { return this->maxRise; }
float Submarine::getMaxDive() { return this->maxDive; }





void SubmarineSteeringState::reset() {
    rotationChange = 0.0f; // + is left
    depthChange = 0.0f;
    acceleration = 0.0f;
    fireWeapon = false;
}


SubmarineActuator::SubmarineActuator(Submarine *agent) {
    this->agent = agent;
    state.reset();
}

void SubmarineActuator::doSteering(float dt) {
    agent->applyTorque(vec3(0.0f, state.rotationChange * (0.2f + 0.5f * dot(agent->getDirection(), agent->getVelocity())), 0.0f));
    agent->applyForce(state.acceleration * 20.f * agent->getDirection());
    agent->applyForce(vec3(0.0f, state.depthChange * 3, 0.0f));
    if(state.fireWeapon) {
        //spawn(agent.weapon.missileType);
    }
    state.reset();
}


void SubmarineActuator::rise(float amount) {
    state.depthChange += amount;
}
void SubmarineActuator::dive(float amount) {
    state.depthChange -= amount;
}
void SubmarineActuator::turnLeft(float amount) {
    state.rotationChange += amount;
}
void SubmarineActuator::turnRight(float amount) {
    state.rotationChange -= amount;
}
void SubmarineActuator::accelerate(float amount) {
    state.acceleration += amount;
}
void SubmarineActuator::fire() {
    state.fireWeapon = true;
}
void SubmarineActuator::switchWeapons(uint8_t weapon) {
    //agent->switchWeapons(weapon);
}