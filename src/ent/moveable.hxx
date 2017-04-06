#ifndef _MOVEABLE_HXX_
#define _MOVEABLE_HXX_


#include "glm/glm.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using namespace glm;


class Moveable {

public:

/*
 * going to hold off on constructors until I know how this interacts with
 * collidable in the hierarchy
 * this also means I'm not putting in any functions to interact with position
 */

    vec2 set_velocity(vec2); // re-calculates drag too
    vec2 get_velocity();

    vec2 get_acceleration();
    vec2 set_acceleration(vec2); // check against the max as well

    double get_max_acceleration();
    double set_max_acceleration(double); // rubber-banding enemies

    // not sure if this'll be necessary but ok
    virtual int moveable_overwrite(vec2 velocity, vec2 acceleration, double max_acc, double drag);
    virtual int moveable_prepare_message();

    int adjust_velocity(double t_step);

protected:

/*
 * I'm not yet quite sure which of these should be 3d
 */

    vec2 velocity;

    vec2 acceleration;
    double max_acceleration; // should probably just be a magnitude

    double drag; // always opposes velocity

};

#endif
