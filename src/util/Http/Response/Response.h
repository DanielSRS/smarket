#include "../../Cstrings/Cstrings.h"
#include "../../Map/Map.h" // Map

#ifndef RESPONSE_H
#define RESPONSE_H

/** Dados da response */
typedef struct _ResponseData ResponseData;

/** Estrutura da resposta de uma requisição http */
typedef struct Response
{
  /** Dados e configurações da resposta */
  ResponseData* data;
  /** Define o codigo de status da resposta */
  struct Response* (*withStatusCode)(unsigned code, struct Response* self);
  /** Define a mensagem de status da resposta */
  struct Response* (*withStatusMessage)(const char* message, struct Response* self);
  /** Adiciona header na resposta */
  struct Response* (*withHeader)(char* key, char* value, struct Response* self);
  /** Define o corpo da resposta como JSON */
  struct Response* (*withJSON)(struct Response* self);
  /** Adiciona string ao corpo*/
  struct Response* (*addStringToJson)(char* key, char* value, struct Response* self);
  /** Adiciona map ao corpo*/
  struct Response* (*addObjectToJson)(char* key, Map* object, struct Response* self);
  /** Formata como string para poder enviar pela rede */
  alocatedCString (*toString)(struct Response* self);
  /** Destroi o objeto Response liberando a memória alocada */
  void (*destroy)(struct Response** self);
} Response;

/** Cria uma nova resopnse */
Response* newResponse();

#endif // RESPONSE_H