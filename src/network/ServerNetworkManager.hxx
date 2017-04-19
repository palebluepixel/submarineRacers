#ifndef _SERVER_NETWORK_MANAGER_HXX_
#define _SERVER_NETWORK_MANAGER_HXX_

/* The manager for a single client. Stored by the server. */

#include <Submarine.hxx>
#include <Actuator.hxx>
#include "NetworkManager.hxx"

class ServerNetworkManager : public NetworkManager {
public:
	ServerNetworkManager(int id, int socketAddr);
	void sendWorldDeltas(/*TODO: args?*/);

    void bindToSub(Submarine *sub);

    inline int getSocket() { return this->socketAddr; }
    inline int getID() { return this->id; }

protected:
    int socketAddr;
    int id;

	Actuator<Submarine> *actuator;
	Submarine *sub;

	void controllerStateCommand(short len, char[] message);
};

struct CommandHeader {
	short code;
	short len;
};

#endif