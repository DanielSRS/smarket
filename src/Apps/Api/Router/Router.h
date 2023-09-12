#include "../../../util/Http/Response/Response.h" // Response
#include "../../../util/Http/http.h" // Request

#ifndef ROUTER_H
#define ROUTER_H

typedef struct _RouterConfig RouterConfig;

typedef struct Router
{
  /** Configurações do router */
  RouterConfig* config;
  /** Adiciona rota que processa requisições de metodo GET para um determinado recurso */
  struct Router* (*get)(const char* path, void (*handler)(Request* req, Response* res), struct Router* self);
  /** Adiciona rota que processa requisições de metodo POST para um determinado recurso */
  struct Router* (*post)(const char* path, void (*handler)(Request* req, Response* res), struct Router* self);
  /** Adiciona rota que processa requisições de metodo PUT para um determinado recurso */
  struct Router* (*put)(const char* path, void (*handler)(Request* req, Response* res), struct Router* self);
  /** Adiciona rota que processa requisições de metodo DELETE para um determinado recurso */
  struct Router* (*delete)(const char* path, void (*handler)(Request* req, Response* res), struct Router* self);
  /** destroy */
  void (*destroy)(struct Router** self);
  /* data */
} Router;

/** Cria novo router */
Router* createRouter();

void dispatchRoutes(HTTPConnection* connection, void* router);


#endif // ROUTER_H