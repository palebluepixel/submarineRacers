#ifndef _SERVER_NETWORK_MANAGER_HXX_
#define _SERVER_NETWORK_MANAGER_HXX_

/* The manager for a single client. Stored by the server. */

#include <ent/Submarine.hxx>
#include <ent/Actuator.hxx>
#include <network/NetworkManager.hxx>

class ServerNetworkManager : public NetworkManager {
public:
	ServerNetworkManager();

    void bindToRacer(Racer *racer);
    void unbindFromRacer();
    inline Racer *getRacer() {return this->racer;}

    Submarine *getSubmarine();
    SubmarineActuator *getActuator();

    inline int getID() { return this->id; }

    inline void setLoadedLevel(int s) { this->loadedLevel = s;}
    inline int  getLoadedLevel() {return this->loadedLevel;}
	inline void setWhichLevel(int s) { this->whichLevel = s;}
    inline int  getWhichLevel() {return this->whichLevel;}


protected:
    int id;

    /* Racer struct for this ServerNetworkManager. Contains information on 
    which submarine we are bound to, if any. */
    Racer *racer;

	void controllerStateCommand(COMMAND_PARAMS);

    /* Whenever we send a new LOAD LEVEL message, set this to 0 for all clients.
    When a client sends us LEVEL LOADED, we set it back to 1. Once all clients
    have it set to 1, they have all loaded the level and we are ready to start
    the race. */
    int loadedLevel;
    int whichLevel;

};

#endif