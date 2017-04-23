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
#include <network/MessageQueue.hxx>
#include <functional>

#include "CommandCodes.hxx"


#define COMMAND_PARAMS short len, char *message

class NetworkManager;

typedef struct {
    const short code;
    std::function<void(NetworkManager&, COMMAND_PARAMS)> func;
    //auto func;
} handler;


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
    void pingCommand(COMMAND_PARAMS);
    void pongCommand(COMMAND_PARAMS);
    void initCommand(COMMAND_PARAMS);
    void disconnectCommand(COMMAND_PARAMS);

    static handler table[];

    void sendCommand(short code, short len, char *message);

    bool virtual processCommand(short code, short len, char *message);
    bool checkDispatch(short code, short len, char *message);

    /* The socket fd of the other endpoint of this network manager */
    int targetSocket;

    /* The address info struct of the other endpoint */
    struct sockaddr_in targetAddr;

};

//handler NetworkManager::table = { CODE_PING, &NetworkManager::pingCommand };
handler NetworkManager::table[4] = {{ CODE_PING, &NetworkManager::pingCommand }, 
                                    { CODE_PING, &NetworkManager::pongCommand }, 
                                    { CODE_INIT, &NetworkManager::initCommand },
                                    { CODE_DISCONNECT, &NetworkManager::disconnectCommand }};


struct CommandHeader {
	short code;
	short len;
};

#endif