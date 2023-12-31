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
        ->get("/cashier/purchases", getAllCashierPurchases, server)
        ->post("/cashiers", createCashier, server)
        ->get("/purchases", getAllPurchases, server)
        ->get("/products", getAllProducts, server)
        ->post("/products", createProduct, server)
        ->post("/checkout", createPurchase, server)
        ->put("/checkout", addItensToPurchase, server)
        ->post("/seed", seed, server)
        ->get("/payment", getPaymentMethods, server)
        ->post("/payment", setPaymentMethods, server)
        ->listen(server);

    server->destroy(&server);

    return 0;
}
