//using namespace std;
/*
Abstract class which can be implemented in order to control Agents through Actuators.
The controller can call the commands provided by the actuator to control it's steering
state. The actuator will then translate these commands to control the agent directly.
*/

/
#ifndef _CONTROLLER_HXX_
#define _CONTROLLER_HXX_

#include "Actuator.hxx"

// Agent_Type is the type of the agent that this Controller is controlling
template <class Agent_Type> class Controller  {
   
   //TODO put back
public:
    Actuator<Agent_Type> *actuator;
    Agent_Type *agent;

public:
    Controller(Agent_Type *agent) {
        this->agent = agent;
        //Actuator<Agent_Type> actuator(agent);
        this->actuator = new Actuator<Agent_Type>(agent);
    }
};

#endif