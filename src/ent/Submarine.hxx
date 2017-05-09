#ifndef _SUBMARINE_HXX_
#define _SUBMARINE_HXX_

#include "Agent.hxx"

class Submarine : public Agent {
public:

    Submarine(int ID, vec3 initial_position, quaternion initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval, vec3 color, char *modelfilein);
    void switchWeapons(uint8_t weapon);

    void initalizeTextures(const char* texfile);
    void initalizeVisualData();
    void initalizeMeshes();

    vec3 color;
    char *modelfile;
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
   
protected:
    Submarine *agent;
    SubmarineSteeringState state;
};


#endif
