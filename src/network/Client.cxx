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
        printf("%s\n", "Could not open socket");
        exit(-1);
    }
    printf("%s\n", "Created socket");

    /* Get server info by hostname */
    serverInfo = gethostbyname(this->hostname);
    if(serverInfo == NULL) {
        printf("Server with hostname %s did not exist\n",this->hostname);
        exit(-1);
    }
    printf("Got server information from hostname\n");

    /* Initalize server address info based on info from above*/
    serverAddr.sin_family = serverInfo->h_addrtype;
    memcpy((char *) &serverAddr.sin_addr.s_addr, 
        serverInfo->h_addr_list[0], serverInfo->h_length);
    serverAddr.sin_port = htons(this->port);

    /* Copy over into to network manager */
    this->nm->setTargetSocket(serverSocket);
    this->nm->setTargetAddr(serverAddr);

    printf("Successfully connected to %s at port %d %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
    
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


void Client::messageServer(short len, char *msg)
{
    struct sockaddr_in serverAddr = this->nm->getTargetAddr();
    printf("-----Sending to %s at port %d %d\n", inet_ntoa(serverAddr.sin_addr), serverAddr.sin_port, ntohs(serverAddr.sin_port));
    MessageContainer *m = new MessageContainer(serverAddr, msg, len);
    this->sendOneMessage(m, this->nm->getTargetSocket(), 
        (struct sockaddr*)&serverAddr);
    printf("%d\n", errno);
}