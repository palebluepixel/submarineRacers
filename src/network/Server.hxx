#ifndef _SERVER_HXX_
#define _SERVER_HXX_

#include <map>
#include <network/ServerNetworkManager.hxx>

using namespace std;

/* Comparator for sockadd_in structs, for use in the client map */
struct sockaddr_inComparator
{
    bool operator()(const struct sockaddr_in a, const struct sockaddr_in b)
    {
        return a.sin_addr.s_addr < b.sin_addr.s_addr;
    }
};


class Server {
public:
    Server(short port);
    ~Server();

    /* Returns the file descriptor for a new socket configured to listen for
    UDP datagrams on the given port. The returned socket will be bound using
    bind(). */
    int createNewUDPSocket(short port);

    /* Prepare our listening socket to listen for connections. */
    void initListeningSocket();

    /* Check for a user connection. If we found one, we will add it to our
    list of clients. This function should be called in a loop. */
    void checkConnection();

    /* Reads any incoming messages and then parses them. 
    Will process a maximum of mmax messages (used to control how much time
    we spend doing network stuff per tick, anything leftover will be done
    next tick). Processes messages until none remain if mmax == 0 (there
    is a risk of this continuing infinitely if we always recieve a new 
    message before we finish processing the old one. */
    void handleNetworkTick(uint32_t mmax);

    /* Takes the frontmost recieved packet, gets the message and the source,
    and calls recieveMessage() from the ServerNetworkManager corresponding
    to the source client. */
    int readWire();

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

    /* Client list, stored as a map from client addresses to ServerNetworkManagers,
    which contain info for the client, such as which sub they control, their
    actuator, etc. 

    In UDP, clients are identified by their raw IP address. Whenever we recieve a
    packet, we have to check which address it is from, and then get the
    ServerNetworkManager for that address.
    */
    map<struct sockaddr_in, ServerNetworkManager*, sockaddr_inComparator> clients;

    /* Adds a client with the given socketAddr to the list*/
    void addClient(struct sockaddr_in clientAddr);

    int getNextID();
};


#endif //_SERVER_HXX_