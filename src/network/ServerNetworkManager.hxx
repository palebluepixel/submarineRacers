#ifndef _SERVER_NETWORK_MANAGER_HXX_
#define _SERVER_NETWORK_MANAGER_HXX_

/* The manager for a single client. Stored by the server. */

#include <ent/Submarine.hxx>
#include <ent/Actuator.hxx>
#include <network/NetworkManager.hxx>

class ServerNetworkManager : public NetworkManager {
public:
	ServerNetworkManager(int id);
	void sendWorldDeltas(/*TODO: args?*/);

    void bindToSub(Submarine *sub);

    inline int getID() { return this->id; }

protected:
    int id;

    bool virtual processCommand(short code, short len, uint8_t *message);
    static handler table[1];
	SubmarineActuator *actuator;
	Submarine *sub;

	void controllerStateCommand(short len, char* message);
	ControllerState previousControllerState;
};

#endif