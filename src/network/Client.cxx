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
    /* Create a socket to talk to the server */
    this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(this->serverSocket < 0) {
        printf("%s\n", "Could not open socket");
        exit(-1);
    }
    printf("%s\n", "Created socket");

    /* Get server info by hostname */
    this->serverInfo = gethostbyname(this->hostname);
    if(this->serverInfo == NULL) {
        printf("Server with hostname %s did not exist\n",this->hostname);
        exit(-1);
    }
    printf("Got server information from hostname\n");

    /* Initalize server address info based on info from above*/
    this->serverAddr.sin_family = this->serverInfo->h_addrtype;
    memcpy((char *) &this->serverAddr.sin_addr.s_addr, 
        this->serverInfo->h_addr_list[0], this->serverInfo->h_length);
    this->serverAddr.sin_port = htons(this->port);

    /* Connect */
    if (connect(this->serverSocket, (struct sockaddr *) &this->serverAddr, sizeof(this->serverAddr)) < 0) {
        printf("Could not connect to %s at port %d\n", this->hostname, this->port);
        exit(-1);
    }

    printf("Successfully connected to %s at port %d\n", this->hostname, this->port);
    
}

