#ifndef _RACER_HXX_
#define _RACER_HXX_ 


class ServerNetworkManager;
class SubmarineAI;
class Submarine;

typedef enum {
    HUMAN_CONTROLLED,
    AI_CONTROLLED
} control_type;

/* Encapsulating class for controlling a submarine. Used to represent either
a human or an AI. In both cases, it has:
    * An integer player number used to identify it to other players
    * A pointer to the submarine it is bound to
    * Functions to bind and unbind it from a submarine (and the corresponding
    actuator)
If it is a human-controlled sub, it also has a pointer to the 
ServerNetworkManager for the human client it is assoicated with.
If it is an AI-controlled sub, it also has a reference to its AI.
*/


class Racer {
public:
    /* Whether we are human or AI depends on which constructor is called */
    Racer(ServerNetworkManager *client);
    Racer(SubmarineAI *ai);

    void bindToSub(Submarine *sub);
    void unbindFromSub();
    inline int isBound() {return !!this->sub;} //returns true if bound to a sub
    inline Submarine *getSub() {return this->sub;}

    inline control_type getType() {return this->type;}

    inline int getID() {return this->id;}

    /* Returns null if AI */
    inline ServerNetworkManager *getClient() {return this->client;}
    /* Returns null if human */
    inline SubmarineAI *getAI() {return this->ai;} 

private:

    /* Initialization common to both constructors */
    void initRacer();

    control_type type;

    int id;

    Submarine *sub;
    ServerNetworkManager *client;
    SubmarineAI *ai;

};

#endif