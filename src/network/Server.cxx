#include "Server.hxx"

Server::Server(short port) 
{
    memset(&this->serverAddr, 0, sizeof(struct sockaddr_in));
    this->port = port;

}

Server::~Server()
{}


/* Returns the file descriptor for a new socket configured to listen for
UDP datagrams on the given port. The returned socket will be bound using
bind(). */
int Server::createNewUDPSocket(short port)
{
    const int yes=1;
    int sock;
    struct sockaddr_in ourAddr;

    /*create socket */
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        printf("%s\n", "Could not open socket");
        exit(-1);
    }

    ourAddr.sin_family = AF_INET;
    ourAddr.sin_port = htons(port); //I guess this has to be in network order?
    ourAddr.sin_addr.s_addr = INADDR_ANY;

    // Reuse address if it is already bound
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    /*bind the listening socket */
    if(bind(sock, (struct sockaddr *) &ourAddr, sizeof(ourAddr)) < 0)
    {
        printf("%s %d\n", "Socket bind() failed", sock);
        close(sock);
        exit(-1);
    }

    return sock;
}


/* Prepare our listening socket to listen for connections. */
void Server::initListeningSocket()
{
    this->listeningSocket = createNewUDPSocket(this->port);

    gethostname(this->hostname, 100);

    printf("Began listening on socket %d. We are %s@%s\n", this->listeningSocket, this->hostname, inet_ntoa(this->serverAddr.sin_addr));
    printf("NOTE: 0.0.0.0 IS FINE, THATS THE WILDCARD ADDRESS IN THIS CASE\n"); 
}

/* Send a message to the first client with ID id. Returns the number
of bytes sent.*/
int Server::messageClient(int id, short len, char*msg)
{
    return 0;
}

/* Sends a message to all clients.*/
void Server::broadcast(short len, char *msg)
{

}

/* 
Once a client messages us on our listening socket, we will create a new socket which
is dedicated just to communication with that client. This way we can avoid conflicts
between data being recieved from multiple clients at the same time. 

formConnection() first checks if the server already has a ServerNetworkManager object
to handle this client address, and if so resends the "connected" message to that client
but does not other work. If this address does not have a connection, we:
1. find an unused port on which we can communicate exclusively with this client
2. create and bind a new socket which we will use to communicate with this client
3. create a new ServerNetworkManager for this client and add it to the map
4. send a "connected" message to the client informing them of the port for their
   communication socket.
*/
void Server::formConnection(struct sockaddr_in *client)
{

}


/* Adds a client with the given socketAddr to the list*/
void Server::addClient(int socketAddr) 
{
    ServerNetworkManager *client = new ServerNetworkManager(this->getNextID());
    client->setTargetSocket(socketAddr);
    //have to set targetAddr only if we go to UDP

    this->clients.push_back(client);

    printf("Added new client with ID %d, and address %d\n", client->getID(), client->getTargetSocket());
}

/* Returns an ID that has not been taken yet. */
int Server::getNextID()
{
    static int ID = -1;
    return ++ID;
}

/* Return the next unbound port. We assume that all ports greater than the one of our listening
socket is unused. We could obviously make this more sophisticated later. */
int server::getNextPort()
{
    static short p = PORT;
    return ++port;
}