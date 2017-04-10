#include "Agent.hxx"

Agent::Agent(vec2 initial_position, mat3 initial_orientation, int id, char*name, 
    EntityType type, EntityStatus status, float tick_interval) 
: Entity(initial_position, initial_orientation, id, name, type, status, tick_interval) {
    //actuator = new Actuator<this->GetType()> (this);
}


int Agent::onTick() {
    int res = Entity::onTick();
    //actuator.doSteering();

    return res;
}