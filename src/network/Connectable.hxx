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

    /* Reads any incoming messages and then parses them. 
    Will process a maximum of mmax messages (used to control how much time
    we spend doing network stuff per tick, anything leftover will be done
    next tick). Processes messages until none remain if mmax == 0 (there
    is a risk of this continuing infinitely if we always recieve a new 
    message before we finish processing the old one. */
    void ReadMessages(uint32_t mmax);

    /* Process one message from the message buffer. This has no functionality
    in the Connectbale class and should be implemented in server / client */
    virtual int ReadOneMessage();

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