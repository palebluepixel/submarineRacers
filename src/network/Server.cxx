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

    gethostname(this->hostname, MAX_HOSTNAME_LENGTH);

    printf("Began listening on socket %d. We are %s@%s\n", this->listeningSocket, this->hostname, inet_ntoa(this->serverAddr.sin_addr));
    printf("NOTE: 0.0.0.0 IS FINE, THATS THE WILDCARD ADDRESS IN THIS CASE\n"); 
}


/* Reads the least recently recieved message from our listening
    socket and copies it into the message buffer as a messageContainer. */
void Server::recieveOneMessage()
{
    // Read a packet from the UDP socket
    uint8_t bytebuf[MAX_MESSAGE_LENGTH+1];
    struct sockaddr_in msgsrc;
    socklen_t s = sizeof(sockaddr_in);
    int bytesRead;

    bytesRead = recvfrom(this->listeningSocket, bytebuf, MAX_MESSAGE_LENGTH,
        0, (struct sockaddr*)&msgsrc, &s);

    bytebuf[bytesRead] = '\0'; //make it a string

    // Create message struct
    messageContainer *m = (messageContainer *)malloc(sizeof(messageContainer));
    m->src = msgsrc;
    m->msgLen = bytesRead; //I think we dont actually need this
    m->msg = strdup((char*) &bytebuf); //this is safe because we added a '\0'

    //add to the queue
    this->queue.addMessage(m);

}

/* Takes one message from the message queue, finds the client who sent it,
    and calls recieveMessage() from the ServerNetworkManager corresponding
    to the source client. */
void Server::readOneMessage()
{

    messageContainer *m = this->queue.readMessage();
    ServerNetworkManager *client = this->findClientByAddr(m->src);

    client->recieveMessage(m->msg, m->msgLen);

    free(m->msg);
    free(m);
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


ServerNetworkManager *Server::findClientByAddr(struct sockaddr_in addr)
{
    return this->clients[addr];
}

/* Returns an ID that has not been taken yet. */
int Server::getNextID()
{
    static int ID = -1;
    return ++ID;
}