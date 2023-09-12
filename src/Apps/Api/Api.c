#include <stdlib.h> // exit
#include "../../util/Http/http.h" // createHTTPServer
#include "../../util/Socket/socket.h" // handleConnectionOnANewProcess
#include "./App/App.h"
#include "./Routes/Routes.h" // about

int main(void) {

    App* server = createApp();

    server
        ->get("/about", about, server)
        ->get("/cashiers/list", listCashiers, server)
        ->post("/cashiers/registry", registryCashier, server)
        ->get("/purchases/all", getAllPurchases, server)
        ->listen(server);

    server->destroy(&server);

    return 0;
}
