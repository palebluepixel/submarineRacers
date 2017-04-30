#ifndef _SUBMARINE_HXX_
#define _SUBMARINE_HXX_

#include "Agent.hxx"

class Submarine : public Agent {
public:
    Submarine(int ID, vec3 initial_position, mat3 initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval);

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

    void doSteering();
   
protected:
    Submarine *agent;
    SubmarineSteeringState state;
};


#endif
