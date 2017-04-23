#ifndef _MESSAGEQUEUE_HXX_
#define _MESSAGEQUEUE_HXX_

#include <deque>
#include <pthread.h>
#include <network/ServerNetworkManager.hxx>

using namespace std;

typedef struct messageContainer {
	struct sockaddr_in src;
	char *msg;
	int msgLen;
} messageContainer;

/* Threadsafe double-ended queue for storing messages. All locking
is handled inside of the message add and removal fucntions, the caller
needs to do no locking. */
class MessageQueue {
public:
	MessageQueue();
	~MessageQueue();

	/* Add a message to the back of the queue */
	void addMessage(messageContainer *m);

	/* removes a message from the front of the queue and returns it */
	messageContainer *readMessage();

protected:
	pthread_mutex_t queueLock;

	deque<messageContainer *> queue;

};

#endif //!_MESSAGEQUEUE_HXX_