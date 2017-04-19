#include "server.hxx"

Server::Server(short port) 
{
    memset(&this->serverAddr, 0, sizeof(struct sockaddr_in));
    this->port = port;

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
    this->listeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP);
    if(listeningSocket == -1)
    {
        printf("%s\n", "Could not open socket");
        exit(-1);
    }

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(port); //I guess this has to be in network order?
    this->serverAddr.sin_addr.s_addr = INADDR_ANY;

    setsockopt(this->listeningSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    /*bind the listening socket */
    if(bind(this->listeningSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
    {
        printf("%s %d\n", "Socket bind() failed", this->listeningSocket);
        close(this->listeningSocket);
        exit(-1);
    }

    /* listen on the listeningSocket */
    if(listen(this->listeningSocket, 5) == -1)
    {
        perror("Socket listen() failed");
        close(this->listeningSocket);
        exit(-1);
    }
}

/* Check for a user connection. If we found one, we will add it to our
list of clients. This function should be called in a lopp. */
void Server::checkConnection()
{
    // Create an empty clientAddr struct
    struct sockaddr_in * clientAddr = malloc(sizeof(struct sockaddr_in));

    int handlingSocket;

    // Accept incoming connections
    if( (handlingSocket = accept(listeningSocket, (struct sockaddr *) clientAddr, &sinSize)) == -1) {
        printf("%s\n", "Socket accept() failed");
        free(clientAddr);
        return;
    }

    // GOT EM
    printf("Got a new connection: handingSocket # %d\n", handlingSocket);

    this->addClient(handlingSocket);

    free(clientAddr);
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
    ServerNetworkManager *client = new ServerNetworkManager(this->getNextID(), socketAddr);

    this->clients.push_back(client);

    printf("Added new client with ID %d, and address %d\n", client->getID(), client->getSocket());
}

/* Returns an ID that has not been taken yet. */
int Server::getNextID()
{
    static int ID = -1;
    return ++ID;
}