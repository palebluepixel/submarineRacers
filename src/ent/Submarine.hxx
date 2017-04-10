#ifndef _SUBMARINE_HXX_
#define _SUBMARINE_HXX_

#include "Agent.hxx"

class Submarine : public Agent {
public:
    Submarine(vec2 initial_position, mat3 initial_orientation, int id, char*name, 
        EntityType type, EntityStatus status, float tick_interval);
};

#endif


