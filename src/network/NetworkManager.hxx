#ifndef _NETWORK_MANAGER_HXX_
#define _NETWORK_MANAGER_HXX_

#include <sys/types.h>
#include <sys/socket.h>
#include "CommandCodes.hxx"

class NetworkManager {
public:
	NetworkManager(/*other args*/);
	void recieveMessage(char[] message, int len);

protected:
	//TODO do maps for dispatch table

	void sendMessage(char[] message, int len);
	void sendCommand(short code, short len, char[] message);

	void processCommand(short code, short len, char[] message);
	void pingCommand(short len, char[] message);
};

struct CommandHeader {
	short code;
	short len;
};

#endif