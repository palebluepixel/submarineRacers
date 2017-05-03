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





/* ================= GENERIC ================ */

/* General message struct */ 
typedef struct {
	short code;
	short len;
	uint8_t *msg;
} message;

/* Create a message */
message * createMessage(short code, short len, uint8_t *msg);

/* Create a message who's payload is the byte representation of an int */
message *createMessageIntPayload(short code, int payload);

/* Free the payload of a message after use */
void deleteMessage(message *m);





/* ============= CONNECTION INIT ============= */

/* Message for client initating connection with server */
inline message *createInitMsg() { return createMessage(CODE_INIT, 0, NULL); }




/* ============== LEVEL LOADING ============== */

/* Tells the server which level the client requested in the level select menu */
inline message *createLevelSelectMsg(int level) {return createMessageIntPayload(CODE_LEVEL_SELECT, level);}

/* Tells the other clients which level has been selected to load */
inline message *createLevelLoadMsg(int level) {return createMessageIntPayload(CODE_LOAD_LEVEL, level);}

/* Tells the server we finished loading the level */
inline message *createLevelLoadedMsg(int level) {return createMessageIntPayload(CODE_LEVEL_LOADED, level);}

/* Tells the client that the race is starting on the current level. This will most likely begin a countdown */
inline message *createStartLevelMsg() {return createMessage(CODE_LEVEL_START, 0, NULL);}

/* Tells either the server or client to unload the current level and go back to menu screen */
inline message *createExitLevelMsg() {return createMessage(CODE_EXIT_LEVEL, 0, NULL);}





/* ========== SERVER POSITION UPDATES ======== */

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

#endif //!_MESSAGE_PROTOCOLS_H_