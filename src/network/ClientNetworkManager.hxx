#ifndef _CLIENT_NETWORK_MANAGER_HXX_
#define _CLIENT_NETWORK_MANAGER_HXX_

/* The manager for a single client. Stored by the server. */

#include <network/NetworkManager.hxx>

class ClientNetworkManager : public NetworkManager {
public:
    ClientNetworkManager();
    void sendControllerState();

protected:
    static handler table[0];
};

#endif
