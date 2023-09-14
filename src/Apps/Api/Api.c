#include <stdlib.h> // exit
#include "../../util/Http/http.h" // createHTTPServer
#include "../../util/Socket/socket.h" // handleConnectionOnANewProcess
#include "./App/App.h"
#include "./Routes/Routes.h" // about

int main(void) {

    App* server = createApp();

    server
        ->get("/about", about, server)
        ->get("/cashiers", listCashiers, server)
        ->post("/cashiers", createCashier, server)
        ->get("/purchases/all", getAllPurchases, server)
        ->get("/products", getAllProducts, server)
        ->post("/products", createProduct, server)
        ->listen(server);

    server->destroy(&server);

    return 0;
}
