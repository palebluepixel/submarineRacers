#ifndef _TORPEDO_HXX_
#define _TORPEDO_HXX_

#include "Agent.hxx"

class Torpedo : public Agent {
public:

    Torpedo(int ID, vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein);

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

private:
    /* For now these are set to the same value for all subs (in the constructor),
    but eventually these may vary so subs have different "stats" for acceleration
    and handling. */
    float maxAccel;
    float maxTurn;
    float maxRise;
    float maxDive;
};



struct TorpedoSteeringState : public SteeringState {
    friend class TorpedoActuator;

private:
    float rotationChange;
    float vertAngleChange;
    float acceleration;

    void reset();
};



class TorpedoActuator : public Actuator {
public:
    TorpedoActuator(Torpedo *agent);

    void doSteering(float dt);

    void turnUp(float amount);
    void turnDown(float amount);
    void turnLeft(float amount);
    void turnRight(float amount);
    void accelerate(float amount);
   
protected:
    Torpedo *agent;
    TorpedoSteeringState state;
};


#endif
