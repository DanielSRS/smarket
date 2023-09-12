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
