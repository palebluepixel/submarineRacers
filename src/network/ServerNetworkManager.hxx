#ifndef _SERVER_NETWORK_MANAGER_HXX_
#define _SERVER_NETWORK_MANAGER_HXX_

#include <Submarine.hxx>
#include <Actuator.hxx>
#include "NetworkManager.hxx"

class ServerNetworkManager : public NetworkManager {
public:
	ServerNetworkManager(Submarine *player/*other args*/);
	void sendWorldDeltas(/*TODO: args?*/);

protected:
	Actuator<Submarine> *actuator;
	Submarine *sub;

	void controllerStateCommand(short len, char[] message);
};

struct CommandHeader {
	short code;
	short len;
};

#endif