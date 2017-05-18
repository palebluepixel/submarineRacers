#ifndef _SUBMARINE_HXX_
#define _SUBMARINE_HXX_

#include "Agent.hxx"
#include <world/ProgressTracker.hxx>

class SubmarineAI;

class Submarine : public Agent {
public:

    Submarine(int ID, vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein);
    void switchWeapons(uint8_t weapon);

    void initalizeTextures(const char* texfile);
    void initalizeVisualData();
    void initalizeMeshes();

    /* Return movement and handling parameters for this sub */
    float getMaxAccel();
    float getMaxTurn();
    float getMaxRise();
    float getMaxDive();

    vec3 color;
    char *modelfile;

    inline ProgressTracker *getPTSeek()  { return this->ptSeek; }
    inline ProgressTracker *getPTCheck() { return this->ptCheck; }

    inline int isAI() { return this->isai; }
    inline SubmarineAI * getAI() { return this->ai; }

    void bindToAI(SubmarineAI *ai);
    void unbindFromAI();

    /* Handles collision with track entities*/
    void hitSeekPoint(int id);
    void hitCheckPoint(int id);

private:
    /* For now these are set to the same value for all subs (in the constructor),
    but eventually these may vary so subs have different "stats" for acceleration
    and handling. */
    float maxAccel;
    float maxTurn;
    float maxRise;
    float maxDive;

    /* Tracks our progress through the level. On collision with a checkpoint or seekpoint,
    update the appropriate tracker. This should be reset every time a new track is loaded. 
    Seekpoints are only used by AI-controlled subs. Checkpoints are used to see if we have
    actually completed the level. */
    ProgressTracker *ptSeek;
    ProgressTracker *ptCheck;

    /* AI control */
    int isai;
    SubmarineAI *ai;
};



struct SubmarineSteeringState : public SteeringState {
    friend class SubmarineActuator;

private:
    float rotationChange;
    float depthChange;
    float acceleration;
    bool fireWeapon;

    void reset();
};



class SubmarineActuator : public Actuator {
public:
    SubmarineActuator(Submarine *agent);

    void doSteering(float dt);

    void rise(float amount);
    void dive(float amount);
    void turnLeft(float amount);
    void turnRight(float amount);
    void accelerate(float amount);
    void fire();
    void switchWeapons(uint8_t weapon);

    inline Submarine *getSub() { return this->agent; }
   
protected:
    Submarine *agent;
    SubmarineSteeringState state;
};


#endif
