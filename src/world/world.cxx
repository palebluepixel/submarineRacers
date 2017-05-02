#include "world.hxx"


World::World() {this->state=TITLE;}

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


/* Handle one tick of the graphics clock (that is, render the game) */
int World::handleGraphicsTick(float t, float dt)
{
    /* Client-side interpolation may happen here as well */ 

    /* Update tethered camera before rendering */
    Camera * curCam = this->view->activeCamera();
    if(curCam->isTethered())
        ((TetheredCamera*)curCam)->updateTetheredCameraPos();

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
    this->getClient()->sendControllerState();
}



void World::sendAllUpdates()
{
    this->getLevel()->sendPosUps(this->getServer());
}

void World::setEntData(posUpBuf* msg)
{
    this->getLevel()->upEntData(msg);
}