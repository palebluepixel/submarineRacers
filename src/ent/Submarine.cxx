#include "Submarine.hxx"
#include "Actuator.hxx"


Submarine::Submarine(vec3 initial_position, mat3 initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval) :
	Agent(initial_position, initial_orientation, name, type, status, tick_interval) {
	//
}


void SubmarineSteeringState::reset() {
    rotationChange = 0;
    depthChange = 0;
    acceleration = 0;
    fireWeapon = false;
}


SubmarineActuator::SubmarineActuator(Submarine *agent) : Actuator(agent) {}

void SubmarineActuator::doSteering() {
    //agent.rotation += state.rotationChange;
    //Not sure what this will be called in the physics engine
    //agent.AddAcceleration(state.acceleration * agent.rotation);

    //Again, not sure
    //changeDepth(state.depth);

    if(state.fireWeapon) {
        //spawn(agent.weapon.missileType);
    }

    state.reset();
}