//using namespace std;
/*
Abstract class which can be implemented in order to control Agents.
The actuator provides useful functions to Controllers (i.e. AIs or an InputMap)
that can be used to set the steering state. doSteering is then called during
the agent's update tick, and updates it according to it's steering state.
*/

#ifndef _ACTUATOR_HXX_
#define _ACTUATOR_HXX_

#include "Agent.hxx"

class Actuator;
class Agent;

struct SteeringState {
    friend class Actuator;

protected:
    virtual void reset() =0;
};

 
class Actuator {

public:
    // abstract function providing interface framework.
    virtual void doSteering(float dt)=0;
};


#endif