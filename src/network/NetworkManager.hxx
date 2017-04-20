#ifndef _NETWORK_MANAGER_HXX_
#define _NETWORK_MANAGER_HXX_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include "CommandCodes.hxx"

class NetworkManager {
public:
	NetworkManager(/*other args*/);
    
    /* Checks if any messages have come in through this socket recently,
    and copies them into the circular buffer */
    void readWire();

	void recieveMessage(char* message, int len);
    void sendMessage(char* message, int len);

    inline void setTargetSocket(int fd) { this->targetSocket = fd;}
    inline void setTargetAddr(struct sockaddr_in sa) { this->targetAddr = sa; }

    inline int getTargetSocket() { return this->targetSocket; }

protected:
	//TODO do maps for dispatch table

	void sendCommand(short code, short len, char* message);

	void processCommand(short code, short len, char* message);
	void pingCommand(short len, char* message);

    /* The socket fd of the other endpoint of this network manager */
    int targetSocket;

    /* The address info struct of the other endpoint */
    struct sockaddr_in targetAddr;
};

struct CommandHeader {
	short code;
	short len;
};

#endif