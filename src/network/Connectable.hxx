#ifndef _CONNECTABLE_HXX_
#define _CONNECTABLE_HXX_

#include <network/MessageQueue.hxx>
#include <network/NetworkManager.hxx>
#include <string.h>
#include <util/log.hxx>
#include <pthread.h>

/* A function to be called as the main function of a new thread.
Constantly checks the socket for incoming messages and reads them
into the queue to be processed by the main thread. */
void * listeningDeamon(void *threadid);

/* Encapsulating class for message handling behavior common to both Servers
and Clients. You should never create a Connectable itself. */
class Connectable {
public:
	Connectable();
	Connectable(short port, const char *hostname);
	~Connectable();

	/* Reads the least recently recieved message from the given socket
    socket and copies it into the message buffer as a messageContainer. */
    void recieveOneMessage(int socket);

    inline int getSocket() { return this->commSocket; } 

protected:

	/* Queue for storing messages. A seperate thread will add messages to this
    queue, and the main thread will pull messages out and parse them. */
    MessageQueue queue;

    /* Thread for listening to socket */
    pthread_t t;

    /* Creates a new thread which constantly reads the socket and adds
    incoming messages to the message buffer. */
    void initalizeListeningThread();

    /* Socket for all client-server communications */
    int commSocket;

};


#endif //!_CONNECTABLE_HXX_