#ifndef _SERVER_HXX_
#define _SERVER_HXX_

#include <vector>
#include <network/ServerNetworkManager.hxx>
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

using namespace std;

class Server {
public:
    Server(short port);
    ~Server();

    /* Prepare our listening socket to listen for connections. */
    void initListeningSocket();

    /* Check for a user connection. If we found one, we will add it to our
    list of clients. This function should be called in a loop. */
    void checkConnection();

    /* Send a message to the first client with ID id. Returns the number
    of bytes sent.*/
    int messageClient(int id, short len, char*msg);

    /* Sends a message to all clients.*/
    void broadcast(short len, char *msg);

    inline int getPort() { return this->port; }

private:
    short port;
    struct sockaddr_in serverAddr;
    int listeningSocket;

    char hostname[100];

    vector<ServerNetworkManager*> clients;

    /* Adds a client with the given socketAddr to the list*/
    void addClient(int socketAddr);

    int getNextID();
};


#endif //_SERVER_HXX_