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


/* Prepare our listening socket. This is the socket we use for all communication
with clients. In UDP, we distinguish between the source and destination of
traffic using IP addresses, whereas in TCP we would have had one socket per
connection. */
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

/* addClient() first checks if the server already has a ServerNetworkManager object
to handle this client address. If this address does not have a connection, we create 
a new ServerNetworkManager object for this client and add it to the list. */
void Server::addClient(struct sockaddr_in clientAddr) 
{
    //TODO: if exists, dont add

    ServerNetworkManager *client = new ServerNetworkManager(this->getNextID());
    client->setTargetAddr(clientAddr);

    this->clients.insert(make_pair(clientAddr, client));

    printf("Added new client with ID %d, and address %s\n", client->getID(), inet_ntoa(client->getTargetAddr().sin_addr));
}

/* Returns an ID that has not been taken yet. */
int Server::getNextID()
{
    static int ID = -1;
    return ++ID;
}