#ifndef _CLIENT_HXX_
#define _CLIENT_HXX_

#include <network/NetworkManager.hxx>
#include <string.h>

/* Wrapper class for network manager. Stores information about the server
we are connected to and contains functions for connecting. */

class Client {

public:
    Client(short port, const char* hostname);
    ~Client();

    /* Connect to the server named hostname at the given port provided to the
    constructor */
    void connectServer();

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