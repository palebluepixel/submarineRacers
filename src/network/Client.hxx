#ifndef _CLIENT_HXX_
#define _CLIENT_HXX_

#include <network/Connectable.hxx>
#include "ClientNetworkManager.hxx"
#include <ent/Submarine.hxx>

/* Wrapper class for network manager. Stores information about the server
we are connected to and contains functions for connecting. */

class Client : public Connectable {

public:
    Client();
    Client(short port, const char *hostname);
    ~Client();

    /* Connect to the server named hostname at the given port provided to the
    constructor */
    void connectServer();

    /* Tell the server what's good */
    void messageServer(message *msg);
    void messageServer(short len, uint8_t*msg);

    /* Tell the server to load the current level */
    void loadLevel(int level);

    /* Tell the server to bind us to the chosen submarine */
    void requestSub(int sub);

    /* Tell the server we want to unload the current level. Server
    will reply with a confirmation, which is when we unload. */
    void exitLevel();

    /* Reads any incoming messages and then parses them. 
    Will process a maximum of mmax messages (used to control how much time
    we spend doing network stuff per tick, anything leftover will be done
    next tick). Processes messages until none remain if mmax == 0 (there
    is a risk of this continuing infinitely if we always recieve a new 
    message before we finish processing the old one. */
    void ReadMessages(uint32_t mmax);

    /* Takes one message out of the message queue and sends it to
    the network manager's process command */
    int readOneMessage();
    void sendControllerState();

    inline void setPlayerNumber(int n) {this->playerNumber = n;}
    inline int getPlayerNumber() {return this->playerNumber;}

    inline void bindToSub(Submarine *s) {this->ourSub = s;}
    inline void unbindFromSub() {this->ourSub = NULL;}
    inline Submarine* getOurSub() {return this->ourSub;}
    inline int isBound() {return !!this->ourSub;}

    inline short getPort() { return this->port; }
    inline char* getHost() { return strdup(this->hostname); }

private:
    short port;
    char*hostname;

    struct hostent * serverInfo;
    ClientNetworkManager *nm;

    /* Sent to us by the server */
    int playerNumber;

    /* Client has to know which sub is ours so it knows where to tether the
    camera and do any other graphics stuff */
    Submarine *ourSub; //null if unbound

};




#endif //_CLIENT_HXX_