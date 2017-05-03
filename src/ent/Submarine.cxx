#include "Submarine.hxx"
#include "Actuator.hxx"


Submarine::Submarine(vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein) :
	Agent(initial_position, initial_orientation, name, type, status, tick_interval) {
	
    this->color = color;
    this->modelfile = modelfilein;
    this->initalizeVisualData();

    this->actuator = new SubmarineActuator(this);
    printf("TEST111111111 %p", this);
    printf("TEST111111111 %p", this->actuator);
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
    meshes.push_back(TransformedMesh(mesh));
}


void SubmarineSteeringState::reset() {
    rotationChange = 0; // + is left
    depthChange = 0;
    acceleration = 0;
    fireWeapon = false;
}


SubmarineActuator::SubmarineActuator(Submarine *agent) {
    this->agent = agent;
    state.reset();
}

void SubmarineActuator::doSteering(float dt) {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    printf("AGENT IS %p %p\n", agent, this->agent);
    agent->applyTorque(glm::vec3(0, state.rotationChange, 0));
    printf("steer1\n");
    agent->applyForce(state.acceleration * agent->getDirection());
    printf("steer2\n");
    agent->applyForce(glm::vec3(0, state.depthChange, 0));
    printf("steer3\n");
    if(state.fireWeapon) {
        //spawn(agent.weapon.missileType);
    }
    printf("steer4\n");
    state.reset();
    printf("Steering done\n");
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
    printf("AGENT IS %p \n", this);
    //agent->switchWeapons(weapon);
}