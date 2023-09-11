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
  /** Destroi o objeto Response liberando a memória alocada */
  void (*destroy)(struct Response** self);
} Response;

/** Cria uma nova resopnse */
Response* newResponse();

#endif // RESPONSE_H