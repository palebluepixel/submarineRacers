#include "MessageQueue.hxx"

MessageQueue::MessageQueue()
{
	pthread_mutex_init(&this->queueLock, NULL);
}

MessageQueue::~MessageQueue()
{}

/* Add a message to the back of the queue */
void MessageQueue::addMessage(messageContainer *m)
{
	pthread_mutex_lock(&this->queueLock);
	this->queue.push_back(m);
	pthread_mutex_unlock(&this->queueLock);
}

/* removes a message from the front of the queue and returns it */
messageContainer *MessageQueue::readMessage()
{
	pthread_mutex_lock(&this->queueLock);
	if(this->queue.empty()){
		pthread_mutex_unlock(&this->queueLock);
		return NULL;
	}
	messageContainer * m = this->queue.back();
	this->queue.pop_back();
	pthread_mutex_unlock(&this->queueLock);
	return m;
}