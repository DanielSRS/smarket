#include <stdlib.h> // exit
#include "../../util/TCP/TCP.h" // createTCPServer
#include "../../util/Socket/socket.h" // handleConnectionOnANewProcess

int main(void) {

    TCPServer* server = createTCPServer();

    server
        ->setNewConnectionHanddler(server, handleConnectionOnANewProcess)
        ->setContext(server, "Sou o contexto!!")
        ->serve(server);

    server->destroy(&server);

    return 0;
}
