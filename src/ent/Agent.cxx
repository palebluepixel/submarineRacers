#include "Agent.hxx"

Agent::Agent(int ID, vec3 initial_position, quaternion initial_orientation, char*name, 
    EntityType type, EntityStatus status, float tick_interval) 
: Entity(ID, initial_position, initial_orientation, name, type, status, tick_interval) {
    //actuator = new Actuator<this->GetType()> (this);
}


int Agent::onTick(float dt) {
	  actuator->doSteering(dt);
    int res = Entity::onTick(dt);

    return res;
}