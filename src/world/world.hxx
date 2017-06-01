#ifndef _WORLD_HXX_
#define _WORLD_HXX_

#include <glm/ext.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <util/log.hxx>
#include <error/error.hxx>
#include <util/file.hxx>
#include <graphics/shader.hxx>
#include <graphics/TetheredCamera.hxx>
#include <graphics/renderer.hxx>
//#include <physics/PhysicsEngine.hxx>
#include <network/MessageProtocols.hxx>
#include <ent/Entity.hxx>
#include <ent/cube.hxx>
#include <ent/gadget.hxx>
#include <graphics/texture.hxx>
#include <network/Server.hxx>
#include <network/Client.hxx>
#include <network/MessageProtocols.hxx>
#include <world/Level.hxx>
#include <cstring>

using namespace glm;
using namespace std;

#define ALL_GOOD 0
#define STATE_ENUM_START ALL_GOOD + 1

#ifndef HANDLER_PARAMS
#define HANDLER_PARAMS int i
#define HANDLER_PARAMS_PASSED i
#endif

/**

ZA WARUDO

The World object manages the stateful information about the game. 
Each tick while the game is running, it calls the relevant AI functions, 
calls physics calculations, and calls render functions.

**/


enum WorldState {
    TITLE = STATE_ENUM_START,
    LISTEN, //server only, waiting for user connections or LOADLEVEL events    
    CONNECTING, //client only, waiting for WEARECONNECTED
    MENU1,
    MENU2, //more if needed
    LOADING_LEVEL,
    RACE_START,
    RACE_RUNNING,
    RACE_FINISH,
    PAUSED
};

//events that can modify the world state
enum EventType {
    STARTCLIENT = 0,
    STARTSERVER,
    WEARECONNECTED, //our client got a message from the server that we're in
    ADVANCEMENU,
    BACKMENU, //moves between menus if multiple
    LOADLEVEL,
    LEVELLOADEDBYALL, //all clients have loaded the level
    PAUSEGAME,
    EXIT, //returns to menu1
    USERDISCONNECT,
    USERFINISH
};

class World {

public:
    World(); //constructor
    ~World(); //destructor

    /* Create all renderers, views, cameras, etc for the world, based off a 
    default. We may eventually want to write a function to load this stuff
    from a .config file instead of having it hard coded here. 
    If isServer==1, then we initialize the world as a server, otherwise, as 
    a client. */
    void worldInitalizeDefault(int isServer);
    void initializeSubsDefault();
    void initalizeSubsFromFile(const char* path);

    //dispacth function for event handlers
    int handleEvent(EventType t, HANDLER_PARAMS);

    // This is what gets called by the while loop in main.
    // Checks if enough time has passed for any ruinning clocks, e.g.
    // physics tics, AI tics, render frame tics...
    // then calls those handlers. Also handles gamestate and playerstate updates
    // based on time, e.g. race timer running down or status conditions ending
    int handleTimerTic(float t, float dt);


    /* Handle one tick of the graphics clock (that is, render the game) */
    int handleGraphicsTick(float t, float dt);

    /* Handle one tick of the networks clock. This is where we handle communication
    with the server/client. 
    Will process a maximum of mmax messages (used to control how much time
    we spend doing network stuff per tick, anything leftover will be done
    next tick). Processes messages until none remain if mmax == 0 (there
    is a risk of this continuing infinitely if we always recieve a new 
    message before we finish processing the old one.*/
    int handleNetworksTick(float t, float dt, int mmax);

    /* Handle one tick of the physics system. If we are a server, we handle
    AI input (not decision-making) in this tick as well. If we are a client,
    we run the physics engine just to interpolate the position of entities. */
    int handlePhysicsTick(float t, float dt);

    /* Quickly set up the given level so that you can skip all the menu stuff.
    Mostly useful for testing in-game things. */
    void quickSetup(int level);


    //View: rendering information, camera, skybox, ground, sun, etc
    inline void setView(View *view) { this->view = view; }
    inline void setEntityRenderer(Renderer *r) { this->r = r; }
    inline void setSkyboxRenderer(Renderer *r) { this->rsky = r; }

    inline View* getView() { return this->view; }
    inline Renderer* getEntityRenderer() { return this->r; }
    inline Renderer* getSkyboxRenderer() { return this->rsky; }

    //Physics:
    //PhysicsEngine *physics;

    GLFWwindow *window;

    //reset
    void fatalError();
    void quit(); 

    //level
    int loadLevel(int i);
    void addLevel(const char*path);
    void addAllLevels(vector<const char*> levels);

    inline Level * getLevel() { return this->curLevel; }
    inline void setLevel(Level * level) { this->curLevel = level; }

    void addSub(int id, Submarine *sub);
    Submarine * getSub(int id);
    void addSubsToLevel();

    //server
    inline void setIsServer(int t) { this->isServ = t; }
    inline void setServer(Server *s) { this->server = s; }
    inline void setClient(Client *c) { this->client = c; }

    inline int isServer() { return   this->isServ; }
    inline int isClient() { return !(this->isServ); }
    inline Server* getServer() { return this->isServer() ? this->server : NULL; }
    inline Client* getClient() { return this->isClient() ? this->client : NULL; }

    inline WorldState getState() { return this->state; }

    /* Functions for parsing the entity list and sending updates to the client */
    void sendAllUpdates();

    /* Functions for setting position updates we recieve from server */
    void setEntData(posUpBuf* msg);
    Entity *moveable;
    
    View *view;



private:
    WorldState state;

    //event handlers
    int handleEventSTARTCLIENT(HANDLER_PARAMS);
    int handleEventSTARTSERVER(HANDLER_PARAMS);
    int handleEventADVANCEMENU(HANDLER_PARAMS);
    int handleEventBACKMENU(HANDLER_PARAMS);
    int handleEventLOADLEVEL(HANDLER_PARAMS);
    int handleEventLEVELLOADEDBYALL(HANDLER_PARAMS);
    int handleEventPAUSEGAME(HANDLER_PARAMS);
    int handleEventEXIT(HANDLER_PARAMS);
    int handleEventUSERDISCONNECT(HANDLER_PARAMS);
    int handleEventUSERFINISH(HANDLER_PARAMS);

    void handleNetworksTickServer(float t, float dt, int mmax);
    void handleNetworksTickClient(float t, float dt, int mmax);

    void renderAll();

    /* Uncomment stuff as it is implemented */

    Renderer *r;
    Renderer *rsky;

    Server *server;
    Client *client;
    int isServ;

    //ActiveLevel: all entities in the world, world boundaries, etc
    //ActiveLevel * activeLevel;
        //GameEntities ** curentEntities; //this could be a sorted data struct to easily get drawables, collidables, etc
        //Submarine ** subs;

    /* A vector of filenames for levels. Whenever we load a level, we build it from scratch from the file. */
    vector<const char*> levels;
    Level * curLevel;

    /* All playable submarines which are currently in the world. These will be bound to clients */
    map<int, Submarine *> subs;

    //Players
    //Player ** players; 

};


#endif  // !_WORLD_HXX_
