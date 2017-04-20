#ifndef _NETWORK_MANAGER_HXX_
#define _NETWORK_MANAGER_HXX_

#define RECV_PARSE_BUFFER_SIZE MAX_MESSAGE_LENGTH*2 + 1

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include "CommandCodes.hxx"
#include <util/buffer.hxx>

typedef enum {
    COMPLETEMESSAGE = 0,
    PARTIALMESSAGE,
    EMPTY
} recvBufferStatus;

class NetworkManager {
public:
	NetworkManager(/*other args*/);
    
    /* Checks if any messages have come in through this socket recently,
    and copies them into the recv buffer if so */
    void readWire();

    /* Parses the recv buffer to check for complete messages. 
        * If it finds a complete message, it returns COMPLETEMESSAGE, copies the 
        message into msgDst, and removes the message from the buffer. msgDst must
        be large enough to contain an entire complete message, that is, be
        MAX_MESSAGE_LENGTH+1 bytes.
        * If it finds an incomplete message, it returns PARTIALMESSAGE.
        * If the buffer contains no complete or partial messages, returns EMPTY.
    The caller should follow up a call to parseRecvBuffer with another call to recv,
    and, if recv returned a positive number of bytes, another call to parseRecvBuffer.
    */
    recvBufferStatus parseRecvBuffer(char *msgDst); 

	void recieveMessage(char* message, int len);
    void sendMessage(char* message, int len);

    inline void setTargetSocket(int fd) { this->targetSocket = fd;}
    inline void setTargetAddr(struct sockaddr_in sa) { this->targetAddr = sa; }

    inline int getTargetSocket() { return this->targetSocket; }

protected:
	//TODO do maps for dispatch table

	void sendCommand(short code, short len, char* message);

	void processCommand(short code, short len, char* message);
	void pingCommand(short len, char* message);

    /* The socket fd of the other endpoint of this network manager */
    int targetSocket;

    /* The address info struct of the other endpoint */
    struct sockaddr_in targetAddr;

    /* 
    A buffer that we use to hold messages as we are parsing them. A single
    call to read MAX_MESSAGE_LENGTH bytes may return:
        * an entire message
        * a partial message
        * an entire packet followed by part of the next message
        * part of the first message followed by part of the next message
    Because
        1) different messages have different lengths or variable lengths and
        thus a read of MAX_MESSAGE_LENGTH will likely return more than one message
        2) calls to recv return an unpredictable number of bytes for 
        ~~kernal reasons~~

    Therefore, we read into a temporary buffer for parsing. Whenever the buffer
    contains a complete contigous message, we remove it from the buffer and
    send it to the message parser. The buffer is implemented as a circular buffer
    with capacity to hold two complete messages. 

    Calls to recv data into the buffer must be careful to not read more bytes than
    the buffer has in its remaining capacity. 
    */
    circular_buffer_t recvBuff;

};

struct CommandHeader {
	short code;
	short len;
};

#endif