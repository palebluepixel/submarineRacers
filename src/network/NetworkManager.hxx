#ifndef _NETWORK_MANAGER_HXX_
#define _NETWORK_MANAGER_HXX_

#define bufToInt(buf) ((int*)buf)[0]

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
#include <network/MessageProtocols.hxx>

#include <network/CommandCodes.hxx>

#ifndef COMMAND_PARAMS
#define COMMAND_PARAMS short len, uint8_t *mes
#endif

class NetworkManager;

typedef struct {
    const short code;
    std::function<void(NetworkManager&, COMMAND_PARAMS)> func;
    //auto func;
} handler;


class NetworkManager {
public:
	NetworkManager(/*other args*/);

	void recieveMessage(uint8_t* message, int len);
    void sendMessage(uint8_t* message, int len); 
    void sendCommand(short code, short len, uint8_t *message);

    inline void setTargetSocket(int fd) { this->targetSocket = fd;}
    inline void setTargetAddr(struct sockaddr_in sa) { this->targetAddr = sa; }

    inline int getTargetSocket() { return this->targetSocket; }
    inline struct sockaddr_in getTargetAddr() { return this->targetAddr; }

protected:
    void pingCommand(COMMAND_PARAMS);
    void pongCommand(COMMAND_PARAMS);
    void initCommand(COMMAND_PARAMS);
    void disconnectCommand(COMMAND_PARAMS);
    void objectChangeCommand(COMMAND_PARAMS);
    void levelSelectCommand(COMMAND_PARAMS);
    void levelLoadCommand(COMMAND_PARAMS);
    void levelLoadedCommand(COMMAND_PARAMS);
    void exitLevelCommand(COMMAND_PARAMS);
    void startLevelCommand(COMMAND_PARAMS);

    static handler table[9];

    bool virtual processCommand(short code, short len, uint8_t *message);
    bool checkDispatch(short code, short len, uint8_t *message);

    /* The socket fd of the other endpoint of this network manager */
    int targetSocket;

    /* The address info struct of the other endpoint */
    struct sockaddr_in targetAddr;

};


struct CommandHeader {
	short code;
	short len;
};

#endif //!_NETWORK_MANAGER_HXX_