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

/* Message for server to send back to client telling them their player number */
inline message *createPlayerNoMessage(int p) {return createMessageIntPayload(CODE_PLAYER_NO, p);}



/* =========== SUBMARINE SELECTION =========== */

/* Tell the server we want to select the submarine with the given index */
inline message *createSubSelectMessage(int sub) {return createMessageIntPayload(CODE_SUB_SELECT, sub);}

/* Tell the server to unbind our sub */
inline message *createUnbindSubMessage() {return createMessage(CODE_UNBIND_SUB, 0, NULL);}

/* Tell the client the submarine is already taken, so they have to choose another one */
inline message *createSubTakenMessage() {return createMessage(CODE_SUB_TAKEN, 0, NULL);}

/* Tell all clients who has chosen the given sub */
message *createSubSelectedMessage(int player, int sub);



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



/* ============== TRACK PROGRESS ============= */

/* Tells a client that the have cleared a given checkpoint and should update visual info */
inline message *createCheckClearMsg(int check) {return createMessageIntPayload(CODE_CHECK_CLEAR, check);}

/* Tells a client that they have cleared a lap and should update visual info */
inline message *createLapClearMsg() {return createMessage(CODE_LAP_CLEAR, 0, NULL);}

/* Tell a client that a player p has finished the race and come in position n */
message *createPlayerFinishMessage(int p, int n);


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