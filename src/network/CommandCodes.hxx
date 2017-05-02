#ifndef _COMMAND_CODES_HXX_
#define _COMMAND_CODES_HXX_

#define MAX_MESSAGE_LENGTH 512

#define CODE_PING 0
#define CODE_PONG 1

//Connection stuff
#define CODE_INIT 8
#define CODE_DISCONNECT 9
#define CODE_SHUTDOWN 10

//Client to Server
#define CODE_CONTROLLER 16
#define CODE_PAUSE 17
#define CODE_MENU_SELECT 18
#define CODE_LEVEL_SELECT 19
#define CODE_LEVEL_LOADED 20

//Server to Client
#define CODE_OBJECT_CHANGE 30
#define CODE_LOAD_LEVEL 31

#endif