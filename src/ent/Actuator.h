//using namespace std;
/*
Abstract class which can be implemented in order to control Agents.

The actuator provides useful functions to Controllers (i.e. AIs or an InputMap)
that can be used to set the steering state. doSteering is then called during
the agent's update tick, and updates it according to it's steering state.
*/

// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <type_traits>

#include "TestAgent.h"
 
template <typename Agent_Type>
class Actuator {

public:
    Actuator(Agent_Type *agent);

    // pure virtual function providing interface framework.
    void doSteering();
    //Should the agent pass itself into this instead?
   
protected:
    Agent_Type *agent;    // See comment under doSteering
    //SteeringState<Agent_Type> steeringState;
};
 







template <>
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
};


// This is the end of the header guard
#endif