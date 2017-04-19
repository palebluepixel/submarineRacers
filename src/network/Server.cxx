#include "Server.hxx"

timeval TIMEWAIT;

Server::Server(short port) 
{
    memset(&this->serverAddr, 0, sizeof(struct sockaddr_in));
    this->port = port;

    TIMEWAIT.tv_sec = 0;
    TIMEWAIT.tv_usec = 3000;

}

Server::~Server()
{
    //This should shut down connections
}


/* Prepare our listening socket to listen for connections. */
void Server::initListeningSocket()
{
    const int yes=1;

    /*create listening socket */
    this->listeningSocket = socket(PF_INET, SOCK_DGRAM, 0);
    if(listeningSocket < 0)
    {
        printf("%s\n", "Could not open socket");
        exit(-1);
    }
    printf("Created listening socket\n");

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(port); //I guess this has to be in network order?
    this->serverAddr.sin_addr.s_addr = INADDR_ANY;

    setsockopt(this->listeningSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    /*bind the listening socket */
    if(bind(this->listeningSocket, (struct sockaddr *) &this->serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("%s %d\n", "Socket bind() failed", this->listeningSocket);
        close(this->listeningSocket);
        exit(-1);
    }
    printf("Bound listening socket\n");

    /* listen on the listeningSocket */
    /*if(listen(this->listeningSocket, 5) == -1)
    {
        perror("Socket listen() failed");
        close(this->listeningSocket);
        exit(-1);
    } TCP */

    gethostname(this->hostname, 100);

    printf("Began listening on socket %d. We are %s@%s\n",this->listeningSocket, this->hostname, inet_ntoa(this->serverAddr.sin_addr));
    printf("NOTE: 0.0.0.0 IS FINE, THATS THE WILDCARD ADDRESS IN THIS CASE\n"); 
}

/* Check for a user connection. If we found one, we will add it to our
list of clients. This function should be called in a loop. Nonblocking.*/
void Server::checkConnection()
{
    /*//check if we have a connection waiting
    fd_set tmp; FD_ZERO(&tmp); FD_SET(this->listeningSocket, &tmp);
    if (select(NULL, &tmp, NULL, NULL, &TIMEWAIT)<=0)
        return;
    //    printf("%s\n", "Error in select()");
    //if(!FD_ISSET(this->listeningSocket, &tmp)) 
    //    return;
    //printf("Got Connection!\n");

    // Create an empty clientAddr struct
    socklen_t sinSize = sizeof(struct sockaddr_in);
    struct sockaddr_in * clientAddr = (struct sockaddr_in*)malloc(sinSize);

    int handlingSocket;

    // Accept incoming connections
    if( (handlingSocket = accept(this->listeningSocket, (struct sockaddr *) clientAddr, &sinSize)) == -1) {
        printf("%s\n", "Socket accept() failed");
        free(clientAddr);
        return;
    }

    // GOT EM
    printf("Got a new connection: handingSocket # %d\n", handlingSocket);

    this->addClient(handlingSocket);

    free(clientAddr); TCP */
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