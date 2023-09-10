#include <stdlib.h> // exit
#include "../../util/Http/http.h" // createHTTPServer
#include "../../util/Socket/socket.h" // handleConnectionOnANewProcess

int main(void) {

    HTTPServer* server = createHTTPServer();

    server
        ->setNewConnectionHanddler(server, handleConnectionOnANewProcess)
        // ->setContext(server, "Sou o contexto!!") // http server ainda não tem contexto
        ->serve(server);

    server->destroy(&server);

    return 0;
}
