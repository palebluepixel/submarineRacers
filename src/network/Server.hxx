#ifndef _SERVER_HXX_
#define _SERVER_HXX_

#include <map>
#include <network/ServerNetworkManager.hxx>
#include <network/Connectable.hxx>

#define MAX_HOSTNAME_LENGTH 100

using namespace std;

/* Comparator for sockadd_in structs, for use in the client map */
struct sockaddr_inComparator
{
    bool operator()(const struct sockaddr_in a, const struct sockaddr_in b)
    {
        return a.sin_addr.s_addr < b.sin_addr.s_addr;
    }
};


class Server : public Connectable{
public:
    Server();
    Server(short port, const char *);
    ~Server();



    /* Returns the file descriptor for a new socket configured to listen for
    UDP datagrams on the given port. The returned socket will be bound using
    bind(). */
    int createNewUDPSocket(short port);

    /* Prepare our listening socket to listen for connections. */
    void initListeningSocket();

    /* Reads any incoming messages and then parses them. 
    Will process a maximum of mmax messages (used to control how much time
    we spend doing network stuff per tick, anything leftover will be done
    next tick). Processes messages until none remain if mmax == 0 (there
    is a risk of this continuing infinitely if we always recieve a new 
    message before we finish processing the old one. */
    void ReadMessages(uint32_t mmax);

    /* Takes a message from the back of the message queue and 
    and calls recieveMessage() from the ServerNetworkManager 
    corresponding to the source client. */
    int readOneMessage();

    /* Returns 0 if there is no client associated with the given address in our
    client list, 1 if a client with this address does exist. */
    int clientExists(struct sockaddr_in clientAddr);

    /* Send a message to the client, identified by IP address. */
    void messageClient(struct sockaddr_in clientAddr, short len, uint8_t *msg);

    /* Send a message to the client, identified by their ServerNetworkManager */
    void messageClient(ServerNetworkManager *nm, message *msg);
    void messageClient(ServerNetworkManager *nm, short len, uint8_t *msg);
    void messageClient(ServerNetworkManager *nm, short code, short len, uint8_t *payload);

    /* Sends a message to all clients.*/
    void broadcast(message *msg);
    void broadcast(short len, uint8_t *msg);
    void broadcast(short code, short len, uint8_t *payload);



    /* Resets the LoadedLevel flag for all clients and sends them a message
    telling them to load the level. The World should call the following function,
    clientsLoaded(), once every network tick after calling this function. */
    void sendLoadLevel(int level);
    int  clientsLoaded(int level);

    /* Unloads the current level, and tells all clients to do the same. */
    void exitLevel();


    inline int getPort() { return this->port; }

private:
    short port;
    struct sockaddr_in serverAddr;

    char hostname[MAX_HOSTNAME_LENGTH];

    /* Client list, stored as a map from client addresses to ServerNetworkManagers,
    which contain info for the client, such as which sub they control, their
    actuator, etc. 

    In UDP, clients are identified by their raw IP address. Whenever we recieve a
    packet, we have to check which address it is from, and then get the
    ServerNetworkManager for that address.
    */
    map<struct sockaddr_in, ServerNetworkManager*, sockaddr_inComparator> clients;

    /* Adds a client with the given socketAddr to the list*/
    ServerNetworkManager *addClient(struct sockaddr_in clientAddr);

    ServerNetworkManager *findClientByAddr(struct sockaddr_in addr);

    /* Generate a unique ID. Call this when creating a new ServerNetworkManager
    for a client. */
    int getNextID();
};


#endif //_SERVER_HXX_