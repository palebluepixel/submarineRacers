#ifndef _MESSAGE_PROTOCOLS_H_
#define _MESSAGE_PROTOCOLS_H_

#include <glm/glm.hpp>
//#include <util/log.hxx>
#include <cstring>

using namespace glm;

/* Message for server telling a client where an object is located and its current orientation.
Velocity can be used for interpolation. */
typedef struct {
	int id;
	vec3 pos; //position
	vec3 ori; //orientation (ypr)
	vec3 vel; //velocity
} posUpMsg;

posUpMsg *getPosUpMsg(uint8_t*msg);

#endif //!_MESSAGE_PROTOCOLS_H_