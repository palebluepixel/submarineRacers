#include "Server.hxx"

Server::Server(){}

Server::Server(short port, const char*) 
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
        log(LOGERROR, "%s\n", "Could not open socket");
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
        log(LOGERROR, "%s %d\n", "Socket bind() failed", sock);
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
    this->commSocket = createNewUDPSocket(this->port);

    gethostname(this->hostname, MAX_HOSTNAME_LENGTH);

    this->initalizeListeningThread();

    log(LOGLOW, "Began listening on socket %d. We are %s@%s\n", this->commSocket, this->hostname, inet_ntoa(this->serverAddr.sin_addr));
    log(LOGLOW, "NOTE: 0.0.0.0 IS FINE, THATS THE WILDCARD ADDRESS IN THIS CASE\n"); 
}


/* Reads any incoming messages and then parses them. 
Will process a maximum of mmax messages (used to control how much time
we spend doing network stuff per tick, anything leftover will be done
next tick). Processes messages until none remain if mmax == 0 (there
is a risk of this continuing infinitely if we always recieve a new 
message before we finish processing the old one. */
void Server::handleNetworkTick(uint32_t mmax)
{
    
}


/* Takes one message from the message queue, finds the client who sent it,
    and calls recieveMessage() from the ServerNetworkManager corresponding
    to the source client. */
void Server::readOneMessage()
{

    MessageContainer *m = this->queue.readMessage();
    if(m==NULL) // no messages
        return;

    ServerNetworkManager *client;
    /* If a client with this address is in our list, we will call
    it's recieveMessage function. Otherwise, we add it, then call
    recieveMessage. */
    if(!clientExists(m->src))
        client = addClient(m->src);
    else
        client = this->findClientByAddr(m->src);

    client->recieveMessage(m->msg, m->msgLen);

    delete(m);
}




/* Send a message to the client, identified by IP address. */
void Server::messageClient(struct sockaddr_in clientAddr, short len, char *msg)
{
    if(!clientExists(clientAddr))
        return;
    ServerNetworkManager *client = this->findClientByAddr(clientAddr);

    this->messageClient(client, len, msg);

}

/* Send a message to the client, identified by their ServerNetworkManager */
void Server::messageClient(ServerNetworkManager *nm, short len, char *msg)
{
    nm->sendMessage(msg, len);
}


/* Sends a message to all clients.*/
void Server::broadcast(short len, char *msg)
{
    /* Loop through the network manager for all clients */
    /* If we ever made client adding / removing threaded, we would have to
    lock the client map here */
    ServerNetworkManager *nm;
    map<struct sockaddr_in, ServerNetworkManager*, sockaddr_inComparator>::iterator iter;
    for(iter = this->clients.begin(); iter != this->clients.end(); iter++)
    {
        nm = get<1>(*iter); //map returns a (k,v) pair, get<1> return the value
        this->messageClient(nm, len, msg);
    }
}

/* Returns 0 if there is no client associated with the given address in our
client list, 1 if a client with this address does exist. */
int Server::clientExists(struct sockaddr_in clientAddr)
{
    return this->clients.find(clientAddr) != this->clients.end();
}

/* addClient() first checks if the server already has a ServerNetworkManager object
to handle this client address. If this address does not have a connection, we create 
a new ServerNetworkManager object for this client and add it to the list. */
ServerNetworkManager* Server::addClient(struct sockaddr_in clientAddr) 
{
    // If it exists, don't add
    if(clientExists(clientAddr))
        return this->clients[clientAddr];

    ServerNetworkManager *client = new ServerNetworkManager(this->getNextID());
    client->setTargetAddr(clientAddr);
    client->setTargetSocket(this->commSocket);

    this->clients.insert(make_pair(clientAddr, client));

    log(LOGMEDIUM, "Added new client with ID %d, and address %s\n", client->getID(), inet_ntoa(client->getTargetAddr().sin_addr));

    return client;
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