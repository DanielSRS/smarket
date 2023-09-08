#include <stdlib.h> // exit
#include "../../util/TCP/TCP.h" // createTCPServer

int main(void) {

    TCPServer* server = createTCPServer();

    server->serve(server);

    return 0;
}
