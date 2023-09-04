#include "../Boolean/Boolean.h"
#include "../Map/Map.h"
#include "../Cstrings/Cstrings.h"

#ifndef HTTP_H
#define HTTP_H

#define MAX_HEADER_FIELDS 10

/** Métodos http */
typedef enum Method { GET } Method;

typedef struct Request
{
  /** Método da requisição */
  Method method;
  /** Caminho pra o recurso requisitado */
  alocatedCString path;
  /** Versão do protocolo */
  alocatedCString protocolVersion;
  /** Headers da requisição */
  Map *headers;
  /** Corpo da requisição */
  Map *data;
  /** Se houve algum erro */
  boolean thereWasErrors;
  /**
   * Destroi a request liberando todos os recursos
   */
  const void(*destroy)(struct Request* self);
} Request;


/**
 * Informações sobre a header recebida em uma requisição.
*/
typedef struct RequestHeaderInfo
{
  /** Tamanho total da header */
  int size;
  /** Quantidade de campos da header */
  int numberOfFields;
  /** Se a requisição tem mais campos que o maximo permitido */
  boolean tooManyFields;
  /**
   * Array de substrings para os campos da header.
   * 
   * Cada substring indica o incio de um campo da header.
   * 
   * IMPORTANTE: as substrings não são NULL terminated!!!
   * os tamanho são indicados pelo array fildLengths, onde:
   *  - a substring fildSubstrings[i] tem tamanho fildLengths[i]
  */
  char *fildSubstrings[MAX_HEADER_FIELDS];
  /** Tamanho de cada campo da header */
  int fildLengths[MAX_HEADER_FIELDS];
} RequestHeaderInfo;

/**
 * Dado o buffer de uma requisição. retorna informações sobre a header
 * @param requestBuffer buffer de uma requisição. Normamlmente é o buffer
 * que guarda a informação recebida por recv ou read em uma nova conexão.
 * 
 * @param length tamanho dos dados no buffer. Não deve ser a capacidade
 * total do buffer, mas dos dados gravados!!!
 * (A função recv() retorna o tamanho dos dados gravados no buffer por exemplo)
*/
RequestHeaderInfo getHeadersInfo(const char *requestBuffer, int length);

/**
 * Envia para  saída padrão as informação das headers de uma requisição
 * 
 * @param info Informações da header
*/
void IOPrintRequestHeaderInfo(const RequestHeaderInfo info);

/**
 * Dado o buffer de uma requisição. retorna uma Request
 * @param requestBuffer buffer de uma requisição. Normamlmente é o buffer
 * que guarda a informação recebida por recv ou read em uma nova conexão.
 * 
 * @param length tamanho dos dados no buffer. Não deve ser a capacidade
 * total do buffer, mas dos dados gravados!!!
 * (A função recv() retorna o tamanho dos dados gravados no buffer por exemplo)
 * 
*/
Request parseRequest(const char *requestBuffer, int length);

/**
 * Envia para  saída padrão as informações de uma Request
 * 
 * @param info Request
*/
void IOPrintRequest(Request request);

#endif // HTTP_H