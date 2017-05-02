#include "Submarine.hxx"
#include "Actuator.hxx"


Submarine::Submarine(vec3 initial_position, mat3 initial_orientation, char*name, 
        EntityType type, EntityStatus status, float tick_interval) :
	Agent(initial_position, initial_orientation, name, type, status, tick_interval) {
	//
}

void Submarine::switchWeapons(uint8_t weapon) {
    //TODO
}


void SubmarineSteeringState::reset() {
    rotationChange = 0; // + is left
    depthChange = 0;
    acceleration = 0;
    fireWeapon = false;
}


SubmarineActuator::SubmarineActuator(Submarine *agent) : Actuator(agent) {}

void SubmarineActuator::doSteering(float dt) {
    agent->applyTorque(glm::vec3(0, state.rotationChange, 0));
    agent->applyForce(state.acceleration * agent->getDirection());

    agent->applyForce(glm::vec3(0, state.depthChange, 0));

    if(state.fireWeapon) {
        //spawn(agent.weapon.missileType);
    }

    state.reset();
}


void SubmarineActuator::rise() {
    state.depthChange += 1;
}
void SubmarineActuator::dive() {
    state.depthChange -= 1;
}
void SubmarineActuator::turnLeft() {
    state.rotationChange += 1;
}
void SubmarineActuator::turnRight() {
    state.rotationChange -= 1;
}
void SubmarineActuator::accelerate() {
    state.acceleration += 1;
}
void SubmarineActuator::fire() {
    state.fireWeapon = true;
}
void SubmarineActuator::switchWeapons(uint8_t weapon) {
    agent->switchWeapons(weapon);
}