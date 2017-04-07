//using namespace std;

#include <iostream>
#include "Controller.h"
/*
Abstract class which can be implemented in order to control Agents through Actuators.

The controller can call the commands provided by the actuator to control it's steering
state. The actuator will then translate these commands to control the agent directly.
*/

class TestController : public Controller<TestAgent> {
public:
	int fee;

	TestController(TestAgent *agent, int other)
	: Controller<TestAgent>(agent) {
		fee = other;
	}


};

int main() {  
	TestAgent *ag = new TestAgent;
    TestController dc(ag, 3);  
    
    dc.actuator->doSteering();
    std::cout << dc.agent->x;
    std::cout << dc.fee;
};