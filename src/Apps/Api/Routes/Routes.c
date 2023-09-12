#include "Routes.h" // Request, Response
#include "../../../util/Map/Map.h" // Map
#include <stdlib.h> // NULL

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

/** Retorna a lista dos caixas registrados */
void registryCashier(Request* req, Response* res, void* context) {
  /** Caixa a ser registrado */
  char* id = (char*) req->data->get(req->data, "id");

  /** Se id for nulo */
  if (id == NULL) {
    Map* data = newMap();
    List* errors = newList();
    errors->pushString(errors, "id é requerido");
    errors->pushString(errors, "id deve ser uma string");
    data->setList(data, "errors", errors);
    res
      ->withStatusCode(400, res)
      ->withStatusMessage("Bad Request", res)
      ->withJSON(res)
      ->addStringToJson("sucess", "false", res)
      ->addObjectToJson("data", data, res)
      ->addStringToJson("message", "Erro ao registrar caixa", res);
    
    return;
  }

  /** Global state */
  Map* appState = (Map*) context;

  /** Banco de dados */
  Map* database = appState->get(appState, "db");
  Map* caixas = database->get(database, "cashier");   // tabela de caixas

  Map* responseData = newMap();
  Map* caixa = caixas->get(caixas, id);

  /** Se caixa ja existe */
  if (caixa != NULL) {
    responseData->setString(responseData, "id", (char*) caixa->get(caixa, "id"));
  } else {
    Map* novoCaixa = newMap();
    novoCaixa->setString(novoCaixa, "id", id);
    caixas->setMap(caixas, id, novoCaixa);
    responseData->setString(responseData, "id", id);
  }

  res
    ->withStatusCode(200, res)
    ->withStatusMessage("OK", res)
    ->withJSON(res)
    ->addStringToJson("sucess", "true", res)
    ->addObjectToJson("data", responseData, res)
    ->addStringToJson("message", "Caixa registrado com sucesso", res);
}
