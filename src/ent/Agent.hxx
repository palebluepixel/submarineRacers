#ifndef _AGENT_HXX_
#define _AGENT_HXX_

#include "Actuator.hxx"
#include "Entity.hxx"

class Agent : public Entity {

protected:
    //void (*actuatorSteering)(): 
    //TODO destructors for this, actuator, and controller

public:
    Agent(vec2 initial_position, mat3 initial_orientation, int id, char*name, 
        EntityType type, EntityStatus status, float tick_interval);

    int onTick();
};


#endif