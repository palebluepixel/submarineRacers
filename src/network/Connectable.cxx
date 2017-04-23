#include "Connectable.hxx"

Connectable::Connectable()
{}

Connectable::Connectable(short port, const char *hostname)
{}

Connectable::~Connectable()
{}

/* Reads the least recently recieved message from our listening
    socket and copies it into the message buffer as a messageContainer. */
void Connectable::recieveOneMessage(int socket)
{
    // Read a packet from the UDP socket
    uint8_t bytebuf[MAX_MESSAGE_LENGTH+1];
    struct sockaddr_in src;
    socklen_t s = sizeof(sockaddr_in);
    int bytesRead;

    bytesRead = recvfrom(socket, bytebuf, MAX_MESSAGE_LENGTH, 
    	0, (struct sockaddr *)&src, &s);

    bytebuf[bytesRead] = '\0'; //make it a string

    // Create message struct
    MessageContainer *m = new MessageContainer(src, strdup((char*) &bytebuf), bytesRead+1);

    //add to the queue
    printf("Recieved the message: %s\n", bytebuf);
    this->queue.addMessage(m);

}


/* Sends a message to the given target through the given socket. */
void Connectable::sendOneMessage(MessageContainer *msg, int socket, struct sockaddr* target)
{
	printf("Sending message: %s (length: %d)\nTo: %s on socket %d\n", msg->msg, msg->msgLen,
		inet_ntoa(((struct sockaddr_in*)target)->sin_addr), socket);
	int bytesSent = sendto(socket, msg->msg, msg->msgLen, 0, target, sizeof(struct sockaddr_in));
	printf("Sent %d bytes\n", bytesSent);
}