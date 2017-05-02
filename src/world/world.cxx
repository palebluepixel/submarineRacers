#include "world.hxx"


World::World() 
{
    this->state=TITLE;
    this->setLevel(NULL);
}

World::~World(){};


void World::quit()
{
    glfwSetWindowShouldClose (this->window, true);
}

// Eventually this can perform a hard reset to a crash or title screen
void World::fatalError(){exit(-1);}

//========================== EVENT HANDLERS =========================//

int World::handleEvent(EventType t, HANDLER_PARAMS)
{
    switch(t)
    {
        case STARTCLIENT:
            return this->handleEventSTARTCLIENT(HANDLER_PARAMS_PASSED);
        case STARTSERVER:
            return this->handleEventSTARTSERVER(HANDLER_PARAMS_PASSED);
        case ADVANCEMENU:
            return this->handleEventADVANCEMENU(HANDLER_PARAMS_PASSED);
        case BACKMENU:
            return this->handleEventBACKMENU(HANDLER_PARAMS_PASSED);
        case LOADLEVEL:
            return this->handleEventLOADLEVEL(HANDLER_PARAMS_PASSED);
        case LEVELLOADEDBYALL:
            return this->handleEventLEVELLOADEDBYALL(HANDLER_PARAMS_PASSED);
        case PAUSEGAME:
            return this->handleEventPAUSEGAME(HANDLER_PARAMS_PASSED);
        case EXIT:
            return this->handleEventEXIT(HANDLER_PARAMS_PASSED);
        case USERDISCONNECT:
            return this->handleEventUSERDISCONNECT(HANDLER_PARAMS_PASSED);
        case USERFINISH:
            return this->handleEventUSERFINISH(HANDLER_PARAMS_PASSED);
        default:
            this->fatalError();
            return this->state;
    }
}

int World::handleEventSTARTCLIENT(HANDLER_PARAMS)
{

    switch(this->state)
    {
        case TITLE:
            this->state = CONNECTING;
            this->getClient()->connectServer();
            this->state = MENU1;
            return ALL_GOOD;
        default:
            //disconnect, reset as client
            return this->state; 
    }
}

int World::handleEventSTARTSERVER(HANDLER_PARAMS)
{

    switch(this->state)
    {
        case TITLE:
            this->state = LISTEN;
            this->getServer()->initListeningSocket();
            return ALL_GOOD;
        default:
            //disconnect, reset as server
            return this->state;
    }
}

int World::handleEventADVANCEMENU(HANDLER_PARAMS)
{
    /*switch(this->state)
    {

    }*/
        return 0;
}

int World::handleEventBACKMENU(HANDLER_PARAMS)
{
        return 0;
}

int World::handleEventLOADLEVEL(HANDLER_PARAMS)
{
    switch(this->state)
    {
        case MENU1:
        case MENU2:
        case LISTEN:
            this->state = LOADING_LEVEL;
            this->loadLevel(i);
            return ALL_GOOD;
        default:
            return this->state;
    }
}

int World::handleEventLEVELLOADEDBYALL(HANDLER_PARAMS)
{
    switch(this->state)
    {
        case LOADING_LEVEL:
            this->state = RACE_START;
            return ALL_GOOD;
        default:
            return this->state;
    }
}


int World::handleEventPAUSEGAME(HANDLER_PARAMS)
{
    switch(this->state)
    {
        case PAUSED:
            this->state = RACE_RUNNING;
            return ALL_GOOD;
        case RACE_RUNNING:
            this->state = PAUSED;
            return ALL_GOOD;
        default:
            return this->state;
    }

}

int World::handleEventEXIT(HANDLER_PARAMS)
{
    switch(this->state)
    {
        /* Exit the current level */
        case RACE_RUNNING:
        case RACE_START:
        case RACE_FINISH:
            /* Might want to actually unload here too, currently
            the level just gets unloaded automatically when a
            new one is loaded up. */
            if(this->isClient())
                this->state = MENU1;
            else 
                this->state = LISTEN;
            return ALL_GOOD;
        default:
            return this->state;

    }
        return 0;
}

int World::handleEventUSERDISCONNECT(HANDLER_PARAMS)
{
    /*switch(this->state)
    {

    }*/
        return 0;
}

int World::handleEventUSERFINISH(HANDLER_PARAMS)
{
    /*switch(this->state)
    {

    }*/
        return 0;
}


int World::loadLevel(int i)
{
    if(i >= this->levels.size()){
        logln(LOGERROR, "Invalid level ID: %d\n", i);
        return -1;
    }

    Level* oldLevel = this->getLevel();
    const char* levelPath = this->levels[i];
    
    logln(LOGMEDIUM, "loading level %s\n", levelPath);
    Level* newLevel = new Level(levelPath);
    newLevel->buildLevelFromFile();

    this->setLevel(newLevel);
    if(!(oldLevel==NULL))
        delete(oldLevel);

    // For now, tether our camera to the first entity.
    this->getView()->getFirstPersonCam()->changeTether(newLevel->getEntityByID(1));
}

void World::addLevel(const char*path)
{
    this->levels.push_back(path);
}

void World::addAllLevels(vector<const char*> levels)
{
    for (auto level : levels) {
        this->levels.push_back(level);
    }
}


/* Handle one tick of the graphics clock (that is, render the game) */
int World::handleGraphicsTick(float t, float dt)
{
    Camera * curCam;
    switch(this->state)
    {
        case RACE_RUNNING:
        case RACE_START:
        case RACE_FINISH:
        case PAUSED:
            /* Client-side interpolation may happen here as well */ 

            /* Update tethered camera before rendering */
            curCam = this->view->activeCamera();
            if(curCam->isTethered()){
                ((TetheredCamera*)curCam)->updateTetheredCameraPos();
            }

            this->renderAll();

            return 1;
        
        default:
            return -1;
    }
}

void World::renderAll()
{
    this->getLevel()->renderAll(this->getView(), this->getEntityRenderer(), 
        this->getSkyboxRenderer());
}

/* Handle one tick of the networks clock. This is where we handle communication
with the server/client. */
int World::handleNetworksTick(float t, float dt, int mmax)
{
    if(this->isServer())
        this->handleNetworksTickServer(t, dt, mmax);
    else
        this->handleNetworksTickClient(t, dt, mmax);

    return 1;
}

void World::handleNetworksTickServer(float t, float dt, int mmax)
{
    this->getServer()->ReadMessages(mmax);
    switch(this->state)
    {  
        case RACE_RUNNING:
        case RACE_START:
        case RACE_FINISH:
            this->sendAllUpdates();
            return;
        default:
            return;
    }
}

void World::handleNetworksTickClient(float t, float dt, int mmax)
{
    this->getClient()->ReadMessages(mmax);
}

/* Handle one tick of the physics system. If we are a server, we handle
AI input (not decision-making) in this tick as well. If we are a client,
we run the physics engine just to interpolate the position of entities. */
int World::handlePhysicsTick(float t, float dt)
{
    switch(this->state){
    case RACE_RUNNING:
    case RACE_START:
    case RACE_FINISH:
        if(this->isServer()){
            this->getLevel()->updateLevel(dt);
        }
        else{
            this->getLevel()->interpolateLevel(dt);
        }
        return 0;
    default:
        return 1;
    }
}


void World::sendAllUpdates()
{
    if(this->getLevel())
        this->getLevel()->sendPosUps(this->getServer());
}

void World::setEntData(posUpBuf* msg)
{
    if(this->getLevel())
        this->getLevel()->upEntData(msg);
}







/* Create all renderers, views, cameras, etc for the world, based off a 
default. We may eventually want to write a function to load this stuff
from a .config file instead of having it hard coded here. */
void World::worldInitalizeDefault(int isServer)
{
    // set up shaders.
    Shader *shader = new Shader();
    shader->addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/lightshader.vert"));
    shader->addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/lightshader.frag"));
    shader->build();

    Shader *skyboxShader = new Shader();
    skyboxShader->addShader(GL_VERTEX_SHADER,fileio::load_file("../assets/shaders/skyboxshader.vert"));
    skyboxShader->addShader(GL_FRAGMENT_SHADER,fileio::load_file("../assets/shaders/skyboxshader.frag"));
    skyboxShader->build();

    //create renderer for the given shader
    Renderer *r = new UnderwaterRenderer(shader);  
    Renderer *rsky = new SkyboxRenderer(skyboxShader);

    //initalize camera
    Camera *camera = new Camera();
    //position, yaw-roll, up-vector
    camera->init(vec3(-2,0,-2),vec3(0.5,0,0),vec3(0,1,0)); //location, looking-at, up
    camera->setFOV(90.0);
    camera->setNearFar(0.1, 1000.0);
    /* Add tethered Camera */
    TetheredCamera * camTeth = new TetheredCamera(FIRSTPERSON, NULL, vec3(0,3,0));

    vec3 oceanColor = vec3(0,70,95) / 256.0;
    vec3 oceanBrightColor = vec3(70,241,245) / 256.0;

    //create view
    View *view = new View(this->window);
    view->setSunlight(vec3(0.3, -1.0, 0), vec3(0.9, 0.9, 0.9), vec3(0.1, 0.1, 0.1));
    view->setFog(0, oceanColor, 0.05f, 5.0);
    view->setColoring(1, vec3(1,1,1), vec3(0.2,0.2,0.2), oceanBrightColor, oceanColor,
        0.03f, -5.0f, -30.0f);

    view->addCamera(camera);
    view->addCamera(camTeth);
    view->setFirstPersonCam(camTeth);

    this->setView(view);
    this->setEntityRenderer(r);
    this->setSkyboxRenderer(rsky);

}



/* Quickly set up the given level so that you can skip all the menu stuff.
Mostly useful for testing in-game things. */
void World::quickSetup(int level)
{
    this->loadLevel(level);
    this->state = RACE_RUNNING;
}