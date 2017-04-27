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
            //send connection request to server
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
            // set up port to listen for client connections
            return ALL_GOOD;
        default:
            //disconnect, reset as server
            return this->state;
    }
}

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
    /*switch(this->state)
    {

    }*/
        return 0;
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


void World::setEntData(posUpMsg* msg)
{
    this->setEntData(msg->id, msg->pos, msg->ori, msg->vel);
}

void World::setEntData(int objID, vec3 pos, vec3 ori, vec3 vel)
{
    this->moveable->setPosition(pos);
}