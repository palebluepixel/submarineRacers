#include "Actuator.hxx"

Actuator::Actuator(Agent *agent) {
	printf("Created new actuator with agent %p\n", agent);
    this->agent = agent;
}