//using namespace std;
/*
Abstract class which can be implemented in order to control Agents through Actuators.

The controller can call the commands provided by the actuator to control it's steering
state. The actuator will then translate these commands to control the agent directly.
*/

// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Actuator.h"

// Agent_Type is the type of the agent that this Controller is controlling
template <class Agent_Type> class Controller  {
   
protected:
    Actuator<Agent_Type> actuator;
};

// This is the end of the header guard
#endif