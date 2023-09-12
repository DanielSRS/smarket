/**
 * Rotas registradas
*/
#include "../../../util/Http/Response/Response.h" // Response
#include "../../../util/Http/http.h" // Request

/** Retorna informações sobre a API */
void about(Request* req, Response* res, void* context);

void listCashiers(Request* req, Response* res, void* context);

void registryCashier(Request* req, Response* res, void* context);