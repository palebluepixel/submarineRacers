#include "Submarine.hxx"
#include "Actuator.hxx"
#include <util/log.hxx>
#include <ai/AI.hxx>
#include <world/world.hxx>
#include <network/ServerNetworkManager.hxx>

extern World *world;


Submarine::Submarine(int ID, vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein) :
	Agent(ID, initial_position, initial_orientation, name, type, status, tick_interval) {
	
    this->color = color;
    this->modelfile = modelfilein;
    this->initalizeVisualData();

    this->actuator = new SubmarineActuator(this);
    this->angularDragCoef = 20.f;

    this->maxAccel = 3;
    this->maxTurn = 10;
    this->maxRise = 3;
    this->maxDive = 3;

    this->ptSeek = new ProgressTracker();
    this->ptCheck = new ProgressTracker();

    this->isai = 0;
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


void Submarine::bindToAI(SubmarineAI * ai)
{
    this->isai = 1;
    this->ai = ai;
    ai->bindToSubAct((SubmarineActuator*)this->getActuator());
}

void Submarine::unbindFromAI()
{
    this->isai = 0;
    this->ai->unbindFromSubAct();
    this->ai = NULL;
}


/* Handles collision with track entities*/
void Submarine::hitSeekPoint(int id)
{
    ProgressTracker *pt = this->getPTSeek();
    pt->clearPoint(id);
    if(pt->isLapComplete()){
        pt->completeLap();
    }
}

void Submarine::hitCheckPoint(int id, int isFinish)
{
    Track *track = world->getLevel()->getTrack();
    ProgressTracker *pt = this->getPTCheck();

    /* We will send a message to this client, indicating that we have either:
        1) won the game
        2) cleared the finish line (not for a win), or
        3) cleared a non-finishline checkpoint 
    in that order of precedence. We send only one message. If the client does 
    not exist, we send no message. */
    SubmarineActuator *act = (SubmarineActuator*)this->getActuator();
    ServerNetworkManager *client = act->getControllingClient();

    /* Set progress tracker for this submarine. */
    pt->clearPoint(id);


    /* Check if this is a complete lap. Reset the progress tracker, 
    check if we won the race, and inform the client. */
    if(isFinish && pt->isLapComplete()){
        int laps = pt->completeLap();
        

        /* Check if we won!*/
        if(laps >= track->getLapsToWin()){
            //win handles the message sending that the client won
            world->win(this);
        } else if(!this->isai && client){ //if we are AI or client is null, no message
            message *msg = createLapClearMsg();
            world->getServer()->messageClient(client, msg);
            delete(msg);
        }

    } else if(!this->isai && client){ //if we are AI or client is null, no message
        message *msg = createCheckClearMsg(id);
        world->getServer()->messageClient(client, msg);
        delete(msg);
    }

}



/* ==================================================
   ===============    ACTUATOR     ==================
   ================================================== */


void SubmarineSteeringState::reset() {
    rotationChange = 0.0f; // + is left
    depthChange = 0.0f;
    acceleration = 0.0f;
    fireWeapon = false;
}


SubmarineActuator::SubmarineActuator(Submarine *agent) {
    this->controllingClient = NULL;
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
