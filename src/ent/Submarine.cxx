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
    //printf("AGENT IS %p %p\n", agent, this->agent);
    agent->applyTorque(vec3(0.0f, state.rotationChange * 2.f, 0.0f));
    agent->applyForce(state.acceleration * 10.f * agent->getDirection());
    agent->applyForce(vec3(0.0f, state.depthChange, 0.0f));
    if(state.fireWeapon) {
        //spawn(agent.weapon.missileType);
    }
    state.reset();
}


void SubmarineActuator::rise() {
    state.depthChange += 1;
}
void SubmarineActuator::dive() {
    state.depthChange -= 1;
}
void SubmarineActuator::turnLeft() {
    state.rotationChange += 1;
}
void SubmarineActuator::turnRight() {
    state.rotationChange -= 1;
}
void SubmarineActuator::accelerate() {
    state.acceleration += 1;
}
void SubmarineActuator::fire() {
    state.fireWeapon = true;
}
void SubmarineActuator::switchWeapons(uint8_t weapon) {
    //agent->switchWeapons(weapon);
}