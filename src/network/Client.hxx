#ifndef _CLIENT_HXX_
#define _CLIENT_HXX_

#include <network/Connectable.hxx>

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

    /* Reads any incoming messages and then parses them. 
    Will process a maximum of mmax messages (used to control how much time
    we spend doing network stuff per tick, anything leftover will be done
    next tick). Processes messages until none remain if mmax == 0 (there
    is a risk of this continuing infinitely if we always recieve a new 
    message before we finish processing the old one. */
    void handleNetworkTick(uint32_t mmax);

    /* Tell the server what's good */
    int messageServer(short len, char*msg);

    inline short getPort() { return this->port; }
    inline char* getHost() { return strdup(this->hostname); }

private:
    short port;
    char*hostname;

    struct hostent * serverInfo;
    NetworkManager nm;

};




#endif //_CLIENT_HXX_