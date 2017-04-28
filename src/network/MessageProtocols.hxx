#ifndef _MESSAGE_PROTOCOLS_H_
#define _MESSAGE_PROTOCOLS_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
//#include <util/log.hxx>
#include <cstring>
#include <network/CommandCodes.hxx>

class Entity;

using namespace glm;
typedef glm::tquat<float> quaternion;

/* General message struct */ 
typedef struct {
	short code;
	short len;
	uint8_t *msg;
} message;

message * createMessage(short code, short len, uint8_t *msg);
void deleteMessage(message *m);


/* Message for client initating connection with server */
inline message *createInitMsg() { return createMessage(CODE_INIT, 0, NULL); }


/* Message for server telling a client where an object is located and its current orientation.
Velocity can be used for interpolation. */
typedef struct {
	int id;
	vec3 pos; //position
	quaternion ori; //orientation
	vec3 vel; //velocity
} posUpBuf;

/* Convert a byte buffer into its coresponding popUpBuf */
posUpBuf *getPosUpBuf(uint8_t*msg);

/* Create a posUpBuf for the entity's current fields */
posUpBuf *createPosUpBuf(Entity *ent);
message *createPosUpMsg(Entity *ent);




struct ControllerState {
    uint8_t switchWeaponsKey;   // 0 for nothing pressed, n for weapon n
    bool riseKey;
    bool diveKey;
    bool leftKey;
    bool rightKey;
    bool forwardKey;
    bool reverseKey;
    bool fireKey;
    bool secondaryKey;
    bool sonarKey;
    //TODO mouse data
};



#endif //!_MESSAGE_PROTOCOLS_H_