#include "../../../util/Http/Response/Response.h" // Response
#include "../../../util/Http/http.h" // Request

#ifndef APP_H
#define APP_H

typedef struct _AppConfig AppConfig;


typedef struct App
{
  /** Configurações do app */
  AppConfig* config;
  /** Adiciona rota que processa requisições de metodo GET para um determinado recurso */
  struct App* (*get)(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
  /** Adiciona rota que processa requisições de metodo POST para um determinado recurso */
  struct App* (*post)(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
  /** Adiciona rota que processa requisições de metodo PUT para um determinado recurso */
  struct App* (*put)(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
  /** Adiciona rota que processa requisições de metodo DELETE para um determinado recurso */
  struct App* (*delete)(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
  /** Inicia servidor */
  void (*listen)(struct App* self);
  /** destroi */
  void (*destroy)(struct App** self);
} App;

/** Cria o app */
App* createApp();


#endif // APP_H