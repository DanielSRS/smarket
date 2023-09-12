#include "../Boolean/Boolean.h"
#include "../Map/Map.h"
#include "../Cstrings/Cstrings.h"
#include <stdint.h> // uint16_t
#include "./Response/Response.h" // Response

#ifndef HTTP_H
#define HTTP_H

#define MAX_HEADER_FIELDS 10

/** Métodos http */
typedef enum Method { GET, POST, PUT, DELETE } Method;

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

/** Configurações de um servidor http */
typedef struct _httpServerConfig httpServerConfig;

/** Informações de uma conexão tcp estabelecida */
typedef struct _HTTPConnectionInfo HTTPConnectionInfo;

/**
 * Conexão http
*/
typedef struct HTTPConnection
{
  HTTPConnectionInfo* connectionInfo;
  /** Informações sobre o processamento das headers da requisição */
  RequestHeaderInfo headerInfo;
  /** Requisição recebido */
  Request request;
  /** Resposta a ser enviada */
  Response* response;
  /**
   * Send the response object
   * 
  */
  void (*sendResponse)(struct HTTPConnection* self);
  /**
   * Fecha a conexão
  */
  void (*close)(struct HTTPConnection* self);
  /**
   * Destrói o objeto HTTPConnection.
   * 
   * Se a conexão ainda estiver aberta, é fechada antes da
   * destruição do objeto.
  */
  void (*destroy)(struct HTTPConnection** self);
} HTTPConnection;


/**
 * Servidor http
*/
typedef struct HTTPServer
{
  /**
   * configurações do servidor
  */
  httpServerConfig* serverConfiguration;
  /**
   * Servidor http começa a escutar por conexões
  */
  void (*serve)(struct HTTPServer* self);
  /**
   * Define o tamanho da file no backlog
  */
  struct HTTPServer* (*setBacklogSize)(struct HTTPServer* self, unsigned int newSize);
  /**
   * Define a porta que o servidor vai escutar por conexões
  */
  struct HTTPServer* (*setPort)(struct HTTPServer* self, uint16_t newPort);
  /**
   * Define uma função para lidar com uma nova conexão
  */
  struct HTTPServer* (*setNewConnectionHanddler)(struct HTTPServer* self, void (*handdler)(HTTPConnection* newConnection, void* context));
  /**
   * Define uma valor ao contexto
  */
  struct HTTPServer* (*setContext)(void* context, struct HTTPServer* self);
  /**
   * Destrói o objeto HTTPServer
  */
  void (*destroy)(struct HTTPServer** self);
} HTTPServer;

/** Cria um servidor http */
HTTPServer* createHTTPServer();

#endif // HTTP_H