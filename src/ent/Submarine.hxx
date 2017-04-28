#ifndef _SUBMARINE_HXX_
#define _SUBMARINE_HXX_

#include "Agent.hxx"

class Submarine : public Agent {
public:
    Submarine(vec3 initial_position, mat3 initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval);
    void switchWeapons(uint8_t weapon);
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

    void rise();
    void dive();
    void turnLeft();
    void turnRight();
    void accelerate();
    void fire();
    void switchWeapons(uint8_t weapon);
   
protected:
    Submarine *agent;
    SubmarineSteeringState state;
};


#endif
