#ifndef _WORLD_HXX_
#define _WORLD_HXX_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <graphics/renderer.hxx>
#include <physics/PhysicsEngine.hxx>
#include <network/MessageProtocols.hxx>
#include <ent/Entity.hxx>
#include <util/log.hxx>
#include <world/Level.hxx>
#include <network/Server.hxx>
#include <network/Client.hxx>

using namespace glm;

#define ALL_GOOD 0
#define STATE_ENUM_START ALL_GOOD + 1

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
    PAUSEGAME,
    EXIT, //returns to menu1
    USERDISCONNECT,
    USERFINISH
};

class World {

public:
    World(); //constructor
    ~World(); //destructor

    //dispacth function for event handlers
    int handleEvent(EventType t);

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




    //View: rendering information, camera, skybox, ground, sun, etc
    inline void setView(View *view) { this->view = view; }
    inline void setEntityRenderer(Renderer *r) { this->r = r; }
    inline void setSkyboxRenderer(Renderer *r) { this->rsky = r; }

    inline View* getView() { return this->view; }
    inline Renderer* getEntityRenderer() { return this->r; }
    inline Renderer* getSkyboxRenderer() { return this->rsky; }

    //Physics:
    PhysicsEngine *physics;

    GLFWwindow *window;

    //reset
    void fatalError();
    void quit(); 

    //level
    int loadLevel();
    inline Level * getLevel() { return this->curLevel; }
    inline void setLevel(Level * level) { this->curLevel = level; }

    //server
    inline void setIsServer(int t) { this->isServ = t; }
    inline void setServer(Server *s) { this->server = s; }
    inline void setClient(Client *c) { this->client = c; }

    inline int isServer() { return   this->isServ; }
    inline int isClient() { return !(this->isServ); }
    inline Server* getServer() { return this->isServer() ? this->server : NULL; }
    inline Client* getClient() { return this->isClient() ? this->client : NULL; }


    /* Functions for parsing the entity list and sending updates to the client */
    void sendAllUpdates();

    /* Functions for setting position updates we recieve from server */
    void setEntData(posUpBuf* msg);
    Entity *moveable;
    
    View *view;



private:
    WorldState state;

    //event handlers
    int handleEventSTARTCLIENT();
    int handleEventSTARTSERVER();
    int handleEventWEARECONNECTED();
    int handleEventADVANCEMENU();
    int handleEventBACKMENU();
    int handleEventLOADLEVEL();
    int handleEventPAUSEGAME();
    int handleEventEXIT();
    int handleEventUSERDISCONNECT();
    int handleEventUSERFINISH();

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
    Level * curLevel;

    //Players
    //Player ** players; 

};


#endif  // !_WORLD_HXX_