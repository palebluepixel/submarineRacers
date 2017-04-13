#include "Submarine.hxx"
#include "Actuator.hxx"


Submarine::Submarine(vec3 initial_position, mat3 initial_orientation, int id, char*name, 
        EntityType type, EntityStatus status, float tick_interval) :
	Agent(initial_position, initial_orientation, id, name, type, status, tick_interval) {
	//
}



template<>
struct SteeringState <Submarine> {
    friend class Actuator<Submarine>;

private:
    float rotationChange;
    float depthChange;
    float acceleration;
    bool fireWeapon;

    void reset() {
        rotationChange = 0;
        depthChange = 0;
        acceleration = 0;
        fireWeapon = false;
    }
};



template <>
class Actuator <Submarine> {
public:
    Actuator(Submarine *agent) {
        this->agent = agent;
    }

    void doSteering() {
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
   
protected:
    Submarine *agent;
    SteeringState<Submarine> state;
};