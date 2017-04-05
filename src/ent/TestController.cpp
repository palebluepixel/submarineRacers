//using namespace std;

#include <iostream>
#include "Controller.h"
/*
Abstract class which can be implemented in order to control Agents through Actuators.

The controller can call the commands provided by the actuator to control it's steering
state. The actuator will then translate these commands to control the agent directly.
*/

// Agent_Type is the type of the agent that this Controller is controlling
class TestController : Controller<TestAgent> {
    TestController() {
    }

};

int main() {  
    TestController dc;  
    int x = 3;  
};