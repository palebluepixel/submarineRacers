#include "Client.hxx"
#include <cerrno>

Client::Client(){}

Client::Client(short port, const char*hostname)
{
    this->port = port;
    this->hostname = strdup(hostname);

    this->nm = new NetworkManager();
}

Client::~Client()
{
    //No need to close the connection, if we are freeing this client
    //its because we are shutting down
}

void Client::connectServer()
{
    int serverSocket;
    struct sockaddr_in serverAddr;
    struct hostent *serverInfo;

    /* Create a socket to talk to the server */
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverSocket < 0) {
        log(LOGERROR, "%s\n", "Could not open socket");
        exit(-1);
    }

    /* Get server info by hostname */
    serverInfo = gethostbyname(this->hostname);
    if(serverInfo == NULL) {
        log(LOGERROR, "Server with hostname %s did not exist\n",this->hostname);
        exit(-1);
    }

    /* Initalize server address info based on info from above*/
    serverAddr.sin_family = serverInfo->h_addrtype;
    memcpy((char *) &serverAddr.sin_addr.s_addr, 
        serverInfo->h_addr_list[0], serverInfo->h_length);
    serverAddr.sin_port = htons(this->port);

    /* Copy over into to network manager */
    this->nm->setTargetSocket(serverSocket);
    this->nm->setTargetAddr(serverAddr);

    this->commSocket = serverSocket;
    this->initalizeListeningThread();

    log(LOGLOW,"Successfully connected to %s at port %d %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), serverAddr.sin_port);
    
}


/* Reads any incoming messages and then parses them. 
Will process a maximum of mmax messages (used to control how much time
we spend doing network stuff per tick, anything leftover will be done
next tick). Processes messages until none remain if mmax == 0 (there
is a risk of this continuing infinitely if we always recieve a new 
message before we finish processing the old one. */
void Client::handleNetworkTick(uint32_t mmax)
{

}


/* Takes one message out of the message queue and sends it to
the network manager's process command */
void Client::readOneMessage()
{
    MessageContainer *m = this->queue.readMessage();
    if(m==NULL) // no messages
        return;

    this->nm->recieveMessage(m->msg, m->msgLen);

    delete(m);
}

void Client::messageServer(short len, uint8_t *msg)
{
    this->nm->sendMessage(msg, len);
}