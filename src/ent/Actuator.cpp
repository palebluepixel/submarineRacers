//using namespace std;
#include "Actuator.h"
/*
Abstract class which can be implemented in order to control Agents.

The actuator provides useful functions to Controllers (i.e. AIs or an InputMap)
that can be used to set the steering state. doSteering is then called during
the agent's update tick, and updates it according to it's steering state.
*/

// Agent_Type is the type of the agent that this actuator is controlling


template <>
class Actuator <TestAgent> {
	 void doSteering() {
	 	agent.x = 5;
	 }
}