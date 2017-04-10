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


template<typename Agent_Type>
struct SteeringState {};

 
template <typename Agent_Type>
class Actuator {

public:
    Actuator(Agent_Type *agent);

    // abstract function providing interface framework.
    void doSteering();
    //Should the agent pass itself into this instead?
   
protected:
    Agent_Type *agent;    // See comment under doSteering
    SteeringState<Agent_Type> steeringState;
};





/*template <>
class Actuator <TestAgent> {
public:
    Actuator(TestAgent *agent) {

        this->agent = agent;
        //steeringState = new SteeringState<Agent_Type>();
    }

    // pure virtual function providing interface framework.
    void doSteering() {
        agent->x = 5;
    }
    //Should the agent pass itself into this instead?
   
protected:
    TestAgent *agent;    // See comment under doSteering
    //SteeringState<TestAgent> steeringState;
};*/


#endif