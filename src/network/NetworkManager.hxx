#ifndef _NETWORK_MANAGER_HXX_
#define _NETWORK_MANAGER_HXX_

#define RECV_PARSE_BUFFER_SIZE MAX_MESSAGE_LENGTH*2 + 1

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
#include <util/buffer.hxx>

typedef enum {
    COMPLETEMESSAGE = 0,
    PARTIALMESSAGE,
    EMPTY
} recvBufferStatus;

class NetworkManager {
public:
	NetworkManager(/*other args*/);

	void recieveMessage(char* message, int len);
    void sendMessage(char* message, int len);

    inline void setTargetSocket(int fd) { this->targetSocket = fd;}
    inline void setTargetAddr(struct sockaddr_in sa) { this->targetAddr = sa; }

    inline int getTargetSocket() { return this->targetSocket; }
    inline struct sockaddr_in getTargetAddr() { return this->targetAddr; }

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