#ifndef _WORLD_HXX_
#define _WORLD_HXX_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <graphics/renderer.hxx>
#include <network/MessageProtocols.hxx>
#include <ent/Entity.hxx>
#include <util/log.hxx>

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


    /* Functions for setting position updates we recive from server */
    void setEntData(posUpMsg* msg);
    void setEntData(int objID, vec3 pos, vec3 ori, vec3 vel);
        Entity *moveable;


    //View: rendering information, camera, skybox, ground, sun, etc
    View *view;
    Renderer **renderers;

    GLFWwindow *window;

    //reset
    void fatalError();
    void quit();  

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

    /* Uncomment stuff as it is implemented */

    //ActiveLevel: all entities in the world, world boundaries, etc
    //ActiveLevel * activeLevel;
        //GameEntities ** curentEntities; //this could be a sorted data struct to easily get drawables, collidables, etc
        //Submarine ** subs;
    int loadLevel();

    //Players
    //Player ** players; 

};


#endif  // !_WORLD_HXX_