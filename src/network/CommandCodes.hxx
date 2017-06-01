#ifndef _COMMAND_CODES_HXX_
#define _COMMAND_CODES_HXX_

#define MAX_MESSAGE_LENGTH 512

#define CODE_PING 0
#define CODE_PONG 1

//Connection stuff
#define CODE_INIT 8
#define CODE_PLAYER_NO 9
#define CODE_DISCONNECT 10
#define CODE_SHUTDOWN 11

//Client to Server
#define CODE_CONTROLLER 16
#define CODE_PAUSE 17
#define CODE_MENU_SELECT 18
#define CODE_SUB_SELECT 19
#define CODE_UNBIND_SUB 20
#define CODE_LEVEL_SELECT 21
#define CODE_LEVEL_LOADED 22

//Server to Client
#define CODE_OBJECT_CHANGE 30
#define CODE_SUB_SELECTED 31
#define CODE_SUB_TAKEN 32
#define CODE_LOAD_LEVEL 33
#define CODE_LEVEL_START 34
#define CODE_CREATE_TORPEDO 35

//Race running and finishing
#define CODE_EXIT_LEVEL 40
#define CODE_CHECK_CLEAR 41
#define CODE_LAP_CLEAR 42
#define CODE_PLAYER_FINISH 43

#endif