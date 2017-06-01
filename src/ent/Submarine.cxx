#include "Submarine.hxx"
#include "Actuator.hxx"
#include <util/log.hxx>
#include <ai/AI.hxx>
#include <world/world.hxx>
#include <network/ServerNetworkManager.hxx>
#include "Torpedo.hxx"

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
    this->finishedRace = 0;

    //this->isai = 0;
    this->racer = NULL;
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
    mesh->data.tex = this->tex;
    meshes.push_back(Model(Model::FancyMesh(mesh, mat4(),Model::RenderState(vec4(color,1.f)))));
}

/* Return movement and handling parameters for this sub. These may eventually be
more complicated, for example they may take into account status conditions and 
powerups affecting this sub, and maximum acceleration may depend on depth. */
float Submarine::getMaxAccel() { return this->maxAccel; }
float Submarine::getMaxTurn() { return this->maxTurn; }
float Submarine::getMaxRise() { return this->maxRise; }
float Submarine::getMaxDive() { return this->maxDive; }


void Submarine::bindToRacer(Racer *racer)
{
    this->racer = racer;
    /*if(this->racer->getType() == AI_CONTROLLED){
        this->racer->getAI()->bindToSubAct((SubmarineActuator*)this->getActuator());
    } else if(this->racer->getType() == HUMAN_CONTROLLED){
        this->racer->getClient()->bindToSub(this);
    }*/
}

void Submarine::unbindFromRacer()
{
    /*if(this->racer->getType() == AI_CONTROLLED){
        this->racer->getAI()->unbindFromSubAct();
    } else if(this->racer->getType() == HUMAN_CONTROLLED){
        this->racer->getClient()->unbindFromSub();
    }*/
    this->racer = NULL;
}



/* Handles collision with track entities*/
void Submarine::hitSeekPoint(int id)
{
    ProgressTracker *pt = this->getPTSeek();

    /* If this isn't the next point we expect to hit, don't count it
    (avoids backtracking if we hit an old point)*/
    if(pt->getCurPoint() != id)
        return;

    pt->clearPoint(id);
    if(pt->isLapComplete()){
        pt->completeLap();
    }
}

void Submarine::hitCheckPoint(int id, int isFinish)
{
    /* If we already finished the race, don't track checkpoints any more */
    if(this->finishedRace == 1)
        return;

    /* If this isn't the next expected checkpoint, don't update */
    ProgressTracker *pt = this->getPTCheck();
    if(pt->getCurPoint() != id)
        return;

    Track *track = world->getLevel()->getTrack();
    Racer *racer = this->getRacer();

    /* If our racer is NULL, we haven't been bound yet, so we shouldn't do anything */
    if(!racer)
        return;

    int playerNo = racer->getID();

    /* Set progress tracker for this submarine. */
    //logln(LOGMEDIUM, "clearing point %d", id);
    pt->clearPoint(id);

    /* Flag for which message type to send:
        * 0 : send no message
        * 1 : send lap clear message
        * 2 : send check clear message
    */
    int flag = 0; 

    /* Check if this is a complete lap. Reset the progress tracker, 
    check if we won the race, and inform the client. */
    if(isFinish && pt->isLapComplete()){
        int laps = pt->completeLap();

        /* Check if we completed the race. If we did, check which position we came in. */
        if(laps >= track->getLapsToWin()){
            int position = this->finish();
            if(position == 1)
                world->win(this);
        } else { flag = 1; }//cleared finish line and valid lap}

    } else if(!isFinish) { flag = 2; } //cleared check, not finish line}

    /* If this submarine was controlled by a human, we will send a message to the client, 
    indicating that we have either:
        1) cleared the finish line (not for a win), or
        2) cleared a non-finishline checkpoint 
    in that order of precedence. We send only one message. If the client does 
    not exist, we send no message. */
    if(!racer->getType() == HUMAN_CONTROLLED)
        return;
    ServerNetworkManager *client = racer->getClient();
    if(!client)
        return;

    if(flag == 1){
        message *msg = createLapClearMsg();
        world->getServer()->messageClient(client, msg);
        delete(msg);
    } else if (flag == 2){
        message *msg = createCheckClearMsg(id);
        world->getServer()->messageClient(client, msg);
        delete(msg);
    }

}

/* Finish the race. Returns the position we finished in. */
int Submarine::finish()
{
    Racer *racer = this->getRacer();
    if(!racer)
        return -1;
    int playerNo = racer->getID();

    this->finishedRace = 1;
    int position = world->getLevel()->getTrack()->playerFinish(playerNo);
    message *msg = createPlayerFinishMessage(playerNo, position);
    world->getServer()->broadcast(msg);
    delete(msg); 
    return position;
}

/* Reset submarine information to prepare for the next race */
void Submarine::resetForRace()
{
    this->finishedRace = 0;
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
    //this->controllingClient = NULL;
    this->agent = agent;
    state.reset();
}

void SubmarineActuator::doSteering(float dt) {
    agent->applyTorque(vec3(0.0f, state.rotationChange * (0.2f + 0.5f * dot(agent->getDirection(), agent->getVelocity())), 0.0f));
    agent->applyForce(state.acceleration * 20.f * agent->getDirection());
    agent->applyForce(vec3(0.0f, state.depthChange * 3, 0.0f));
    if(state.fireWeapon) {
        spawnMissile();
    }
    state.reset();
}

void SubmarineActuator::spawnMissile() {
    spawnMissile(1, 5);
}

void SubmarineActuator::spawnMissile(int pos_flag, float vel_scalar) {
    Level *level = world->getLevel();
    vec3 pos = this->agent->getPosition() + this->agent->getDirection() * (5 * pos_flag);
    Torpedo *torp = new Torpedo(level->findFreeID(800), pos, this->agent->getOrientation(), "torp", TYPE1, SPAWNED, 1.f, vec3(1.f,0.8f,0.5f), "../assets/models/sub_3.obj");
    torp->setVelocity(this->agent->getDirection() * vel_scalar);
    level->addEntity(torp);
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
