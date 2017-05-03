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

    SubmarineActuator *actuator;
    Submarine *sub;

protected:
    int id;

	void controllerStateCommand(COMMAND_PARAMS);
};

#endif