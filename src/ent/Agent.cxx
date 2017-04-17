#include "Agent.hxx"

Agent::Agent(vec3 initial_position, mat3 initial_orientation, int id, char*name, 
    EntityType type, EntityStatus status, float tick_interval) 
: Entity(initial_position, initial_orientation, id, name, type, status, tick_interval) {
    //actuator = new Actuator<this->GetType()> (this);
}


int Agent::onTick(float dt) {
	actuator->doSteering(dt);
    int res = Entity::onTick(dt);

    return res;
}