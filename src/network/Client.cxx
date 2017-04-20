#include "Client.hxx"

Client::Client(short port, const char*hostname)
{
    this->port = port;
    this->hostname = strdup(hostname);
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
    this->nm.setTargetSocket(serverSocket);
    this->nm.setTargetAddr(serverAddr);


    printf("Successfully connected to %s at port %d\n", this->hostname, this->port);
    
}

