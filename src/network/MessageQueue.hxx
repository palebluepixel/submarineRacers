#ifndef _MESSAGEQUEUE_HXX_
#define _MESSAGEQUEUE_HXX_

#include <deque>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class MessageContainer {
public:
	MessageContainer(struct sockaddr_in src, char *msg, int msgLen);
	~MessageContainer();

	struct sockaddr_in src;
	char *msg;
	int msgLen;
};



/* Threadsafe double-ended queue for storing messages. All locking
is handled inside of the message add and removal fucntions, the caller
needs to do no locking. */
class MessageQueue {
public:
	MessageQueue();
	~MessageQueue();

	/* Add a message to the back of the queue */
	void addMessage(MessageContainer *m);

	/* removes a message from the front of the queue and returns it */
	MessageContainer *readMessage();

protected:
	pthread_mutex_t queueLock;

	deque<MessageContainer *> queue;

};

#endif //!_MESSAGEQUEUE_HXX_