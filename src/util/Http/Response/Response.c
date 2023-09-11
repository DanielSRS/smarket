#include "Response.h" // Response, newRequest
#include "../../Logger/Logger.h"
#include <stdlib.h> // calloc, free
#include "../../Cstrings/Cstrings.h" // alocatedCString
#include "../../Map/Map.h" // Map

/** Dados da response */
typedef struct _ResponseData
{
  /** Código de resposta */
  unsigned code;
  /** Mensagem de status */
  alocatedCString statusMessage;
  /** Headers */
  Map* headers;
  /** Destroi o objeto ResponseData liberando a memória alocada */
  void (*destroy)(struct _ResponseData** self);
} ResponseData;

static void destroyResponse(Response** self);                           //--------- Response -------/
static Response* setResponseStatusCode(unsigned code, Response* self);
static Response* setResponseHeader(char* key, char* value, Response* self);
static Response* setResponseStatusMessage(const char* message, Response* self);

void destroyResponseData(ResponseData** self);                          //--------- ResponseData -------/
ResponseData* newResponseData();


//---- Response --------------------------------------//

Response* newResponse() {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Response");

  /** Aloca memória para a nova requisição */
  Response* res = calloc(1, sizeof(Response));

  res->destroy = destroyResponse;
  res->withStatusCode = setResponseStatusCode;
  res->withHeader = setResponseHeader;
  res->withStatusMessage = setResponseStatusMessage;

  console->debug(console, "Criando nova Response\n");

  return res;
}

/** Destroi uma resposta, liberando memória */
static void destroyResponse(Response** self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Response");

  if(!(*self)) {
    console->error(console, "Destruindo Response, mas self é NULL\n");
    return; // self é null, não é possivel destruir
  }

  free(*self);
  *self = NULL;

  console->debug(console, "Destruindo Response\n");
  console->destroy(&console);
}

/** Define o codigo de resposta */
static Response* setResponseStatusCode(unsigned code, Response* self) {
  self->data->code = code;
  return self;
}

/** Define a status message de resposta */
static Response* setResponseStatusMessage(const char* message, Response* self) {
  if(message == NULL) return self;

  /** Libera a mensagem antiga */
  if (self->data->statusMessage) free(self->data->statusMessage);

  self->data->statusMessage = duplicateString(message);
  return self;
}

/** Adiciona header na resposta */
static Response* setResponseHeader(char* key, char* value, Response* self) {
  if (key == NULL || value == NULL || self == NULL) return self;

  self->data->headers->setString(self->data->headers, key, value);

  return self;
}

// end ----------------//




/**** ResponseData ******************/

/** Cria objeto ResponseData */
ResponseData* newResponseData() {
  ResponseData* new = calloc(1, sizeof(ResponseData));

  new->destroy = destroyResponseData;
  new->code = 200;
  new->statusMessage = duplicateString("OK");
  new->headers = newMap();

  return new;
}

/** Destroi objeto ResponseData */
void destroyResponseData(ResponseData** self) {
  if(!(*self)) return;

  // libera a mensagem de status
  if((*self)->statusMessage) free((*self)->statusMessage);
  (*self)->headers->destroy(&((*self)->headers));

  free(*self);
  *self = NULL;
}

// end ----------------//
