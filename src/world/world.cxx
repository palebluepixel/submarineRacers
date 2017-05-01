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

int World::handleEvent(EventType t)
{
    switch(t)
    {
        case STARTCLIENT:
            return this->handleEventSTARTCLIENT();
        case STARTSERVER:
            return this->handleEventSTARTSERVER();
        case WEARECONNECTED:
            return this->handleEventWEARECONNECTED();
        case ADVANCEMENU:
            return this->handleEventADVANCEMENU();
        case BACKMENU:
            return this->handleEventBACKMENU();
        case LOADLEVEL:
            return this->handleEventLOADLEVEL();
        case PAUSEGAME:
            return this->handleEventPAUSEGAME();
        case EXIT:
            return this->handleEventEXIT();
        case USERDISCONNECT:
            return this->handleEventUSERDISCONNECT();
        case USERFINISH:
            return this->handleEventUSERFINISH();
        default:
            this->fatalError();
            return this->state;
    }
}

int World::handleEventSTARTCLIENT()
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

int World::handleEventSTARTSERVER()
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

/* unused */
int World::handleEventWEARECONNECTED()
{
    switch(this->state)
    {
        case CONNECTING:
            this->state = MENU1;
            return ALL_GOOD;
        default:
            return this->state;
    }
}

int World::handleEventADVANCEMENU()
{
    /*switch(this->state)
    {

    }*/
        return 0;
}

int World::handleEventBACKMENU()
{
    /*switch(this->state)
    {

    }*/
        return 0;
}

int World::handleEventLOADLEVEL()
{
    Level *level;
    switch(this->state)
    {
        case MENU1:
        case MENU2:
            this->state = LOADING_LEVEL;
            level = new Level();
            level->buildLevelFromFile();
            this->setLevel(level);
            this->state = RACE_START;
            return ALL_GOOD;
        default:
            return this->state;
    }
}

int World::handleEventPAUSEGAME()
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

int World::handleEventEXIT()
{
    /*switch(this->state)
    {

    }*/
        return 0;
}

int World::handleEventUSERDISCONNECT()
{
    /*switch(this->state)
    {

    }*/
        return 0;
}

int World::handleEventUSERFINISH()
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
    /* Client-side interpolation may happen here as well */ 

    /* Update tethered camera before rendering */
    Camera * curCam = this->view->activeCamera();
    if(curCam->isTethered()){
        ((TetheredCamera*)curCam)->updateTetheredCameraPos();
    }

    this->renderAll();

    return 1;
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
    this->sendAllUpdates();
}

void World::handleNetworksTickClient(float t, float dt, int mmax)
{
    this->getClient()->ReadMessages(mmax);
}



void World::sendAllUpdates()
{
    this->getLevel()->sendPosUps(this->getServer());
}

void World::setEntData(posUpBuf* msg)
{
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
    view->setSunlight(vec3(-0.3, -1.0, 0), vec3(0.9, 0.9, 0.9), vec3(0.1, 0.1, 0.1));
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