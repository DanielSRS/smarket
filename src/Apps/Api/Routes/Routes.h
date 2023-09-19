#ifndef ROUTES_H
#define ROUTES_H

/**
 * Rotas registradas
*/
#include "../../../util/Http/Response/Response.h" // Response
#include "../../../util/Http/http.h" // Request
#include "./Seed/Seed.h"

/** Retorna informações sobre a API */
void about(Request* req, Response* res, void* context);

void listCashiers(Request* req, Response* res, void* context);

void createCashier(Request* req, Response* res, void* context);

/** retorna a lista de compras realizadas */
void getAllPurchases(Request* req, Response* res, void* context);

void getAllProducts(Request* req, Response* res, void* context);

void createProduct(Request* req, Response* res, void* context);

void createPurchase(Request* req, Response* res, void* context);

void getAllCashierPurchases(Request* req, Response* res, void* context);

void addItensToPurchase(Request* req, Response* res, void* context);

#endif // ROUTES_H