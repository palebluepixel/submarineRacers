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

    /* Tell the server what's good */
    void messageServer(short len, uint8_t*msg);

    /* Takes one message out of the message queue and sends it to
    the network manager's process command */
    int readOneMessage();

    inline short getPort() { return this->port; }
    inline char* getHost() { return strdup(this->hostname); }

private:
    short port;
    char*hostname;

    struct hostent * serverInfo;
    NetworkManager *nm;

};




#endif //_CLIENT_HXX_