#include "MessageQueue.hxx"

MessageContainer::MessageContainer(struct sockaddr_in src, uint8_t *msg, int msgLen)
{
	this->src = src;
	this->msg = (uint8_t*)malloc(msgLen);
	memcpy(this->msg, msg, msgLen);
	this->msgLen = msgLen;
}

MessageContainer::~MessageContainer()
{
	free(this->msg);
}

MessageQueue::MessageQueue()
{
	pthread_mutex_init(&this->queueLock, NULL);
}

MessageQueue::~MessageQueue()
{}

/* Add a message to the back of the queue */
void MessageQueue::addMessage(MessageContainer *m)
{
	pthread_mutex_lock(&this->queueLock);
	this->queue.push_back(m);
	pthread_mutex_unlock(&this->queueLock);
}

/* removes a message from the front of the queue and returns it */
MessageContainer *MessageQueue::readMessage()
{
	pthread_mutex_lock(&this->queueLock);
	if(this->queue.empty()){
		pthread_mutex_unlock(&this->queueLock);
		return NULL;
	}
	MessageContainer * m = this->queue.back();
	this->queue.pop_back();
	pthread_mutex_unlock(&this->queueLock);
	return m;
}