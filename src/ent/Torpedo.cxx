#include "Torpedo.hxx"
#include <util/log.hxx>


Torpedo::Torpedo(int ID, vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein) :
	Agent(ID, initial_position, initial_orientation, name, type, status, tick_interval) {
	
    this->color = color;
    this->modelfile = modelfilein;
    this->initalizeVisualData();

    this->actuator = new TorpedoActuator(this);
    this->angularDragCoef = 0.f;

    this->maxAccel = 3;
    this->maxTurn = 10;
    this->maxRise = 3;
    this->maxDive = 3;
}

void Torpedo::initalizeTextures(const char* texfile)
{
    //load texture
    image2d * image = new image2d(texfile, true);
    texture2d * cubetex = new texture2d(GL_TEXTURE_2D, image);
    this->tex = cubetex;
    this->img = image;
}

void Torpedo::initalizeVisualData()
{
    this->initalizeTextures("../assets/textures/cubetex.png");
    this->initalizeMeshes();
}

void Torpedo::initalizeMeshes()
{
    Mesh *mesh = new Mesh(GL_TRIANGLES);
    mesh->loadOBJ(modelfile);
    mesh->data.tex = this->tex;
    meshes.push_back(Model(Model::FancyMesh(mesh, mat4(),Model::RenderState(vec4(color,1.f)))));
}

/* Return movement and handling parameters for this sub. These may eventually be
more complicated, for example they may take into account status conditions and 
powerups affecting this sub, and maximum acceleration may depend on depth. */
float Torpedo::getMaxAccel() { return this->maxAccel; }
float Torpedo::getMaxTurn() { return this->maxTurn; }
float Torpedo::getMaxRise() { return this->maxRise; }
float Torpedo::getMaxDive() { return this->maxDive; }





void TorpedoSteeringState::reset() {
    rotationChange = 0.0f; // + is left
    vertAngleChange = 0.0f;
    acceleration = 0.0f;
}


TorpedoActuator::TorpedoActuator(Torpedo *agent) {
    this->agent = agent;
    state.reset();
}

void TorpedoActuator::doSteering(float dt) {
    /*agent->applyTorque(vec3(0.0f, state.rotationChange * (0.2f + 0.5f * dot(agent->getDirection(), agent->getVelocity())), 0.0f));
    agent->applyForce(state.acceleration * 20.f * agent->getDirection());
    agent->applyForce(vec3(0.0f, state.depthChange * 3, 0.0f));*/
    agent->applyForce(20.f * agent->getDirection());        // for demo

    state.reset();
}


void TorpedoActuator::turnUp(float amount) {
    state.vertAngleChange += amount;
}
void TorpedoActuator::turnDown(float amount) {
    state.vertAngleChange -= amount;
}
void TorpedoActuator::turnLeft(float amount) {
    state.rotationChange += amount;
}
void TorpedoActuator::turnRight(float amount) {
    state.rotationChange -= amount;
}
void TorpedoActuator::accelerate(float amount) {
    state.acceleration += amount;
}