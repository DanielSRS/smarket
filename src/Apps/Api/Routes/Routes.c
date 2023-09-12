#include "Routes.h" // Request, Response
#include "../../../util/Map/Map.h" // Map

/** Retorna informações sobre a API */
void about(Request* req, Response* res, void* context) {
  Map* appState = (Map*) context;
  alocatedCString appVersion = appState->get(appState, "version");

  Map* data = newMap();
  data->setString(data, "version", appVersion ? appVersion : "unknown");

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", data, res)
    ->addStringToJson("message", "Informações sobre a api retornadas com sucesso", res);
}

/** Retorna a lista dos caixas registrados */
void listCashiers(Request* req, Response* res, void* context) {
  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* caixas = database->get(database, "cashier");   // tabela de caixas
  int numberOfCashiers = caixas->length;              // quantidade de caixas na tabela
  char** keys = caixas->getKeys(caixas);

  List* listaDeCaixas = newList();
  for (int i = 0; i < numberOfCashiers; i++) {
    listaDeCaixas->pushMap(listaDeCaixas, (Map*) caixas->get(caixas, keys[i]));
  }

  Map* data = newMap();
  data->setList(data, "cashiers", listaDeCaixas);

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", data, res)
    ->addStringToJson("message", "Lista de caixas retornado com sucesso", res);
}
