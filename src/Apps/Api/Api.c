#include <stdlib.h> // exit
#include "../../util/TCP/TCP.h" // createTCPServer
#include "../../util/Socket/socket.h" // handleConnectionOnANewProcess

int main(void) {

    TCPServer* server = createTCPServer();

    server->setNewConnectionHanddler(server, handleConnectionOnANewProcess);
    server->serve(server);

    server->destroy(&server);

    return 0;
}
