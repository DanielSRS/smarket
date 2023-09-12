#include <stdlib.h> // exit
#include "../../util/Http/http.h" // createHTTPServer
#include "../../util/Socket/socket.h" // handleConnectionOnANewProcess
#include "./App/App.h"
#include "./Routes/Routes.h" // about

int main(void) {

    App* server = createApp();

    server
        ->get("/about", about, server)
        ->listen(server);

    server->destroy(&server);

    return 0;
}
