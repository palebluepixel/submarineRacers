//using namespace std;
/*
Abstract class which can be implemented in order to control Agents.
The actuator provides useful functions to Controllers (i.e. AIs or an InputMap)
that can be used to set the steering state. doSteering is then called during
the agent's update tick, and updates it according to it's steering state.
*/

#ifndef _ACTUATOR_HXX_
#define _ACTUATOR_HXX_

#include <type_traits>

#include "Agent.hxx"

template <typename Agent_Type>
class Actuator;

template<typename Agent_Type>
struct SteeringState {
    friend class Actuator<Agent_Type>;

public:
    SteeringState() {
        reset();
    }

private:
    virtual void reset() {}
};

 
template <typename Agent_Type>
class Actuator {

public:
    Actuator(Agent_Type *agent);

    // abstract function providing interface framework.
    void doSteering();
   
protected:
    Agent_Type *agent;
    SteeringState<Agent_Type> state;
};


#endif