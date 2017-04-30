#ifndef _AGENT_HXX_
#define _AGENT_HXX_

#include "Actuator.hxx"
#include "Entity.hxx"

class Actuator;

class Agent : public Entity {

protected:
	Actuator *actuator;
    //TODO destructors for this, actuator, and controller

public:
    Agent(int ID, vec3 initial_position, mat3 initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval);

    int onTick(float dt);
};



#endif