#include "App.h"
#include <stdlib.h> // NULL
#include "../Router/Router.h" // Router, createRouter

typedef struct _AppConfig
{
  HTTPServer* server;
  Router* router;
  void (*destroy)(struct _AppConfig** self);
} AppConfig;

void destroyAppConfig(AppConfig** self);
AppConfig* newAppConfig(HTTPServer* server, Router* router);
void destroyApp(App** self);
App* appget(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
App* apppost(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
App* appput(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
App* appdelete(const char* path, void (*handler)(Request* req, Response* res), struct App* self);
void applisten(App* self);

App* createApp() {
  App* app = calloc(1, sizeof(App));
  HTTPServer* server = createHTTPServer();
  Router* router = createRouter();

  server
    ->setNewConnectionHanddler(server, dispatchRoutes)
    ->setContext((void*) router, server);

  app->destroy = destroyApp;
  app->config = newAppConfig(server, router);
  app->get = appget;
  app->post = apppost;
  app->put = appput;
  app->delete = appdelete;
  app->listen = applisten;

  return app;
}

/** Destroi app config */
void destroyAppConfig(AppConfig** self) {
  if (!(*self)) return;

  (*self)->server->destroy(&((*self)->server));
  (*self)->router->destroy(&((*self)->router));

  free(*self);
  *self = NULL;
}

/** Destroi app */
void destroyApp(App** self) {
  if (!(*self)) return;

  (*self)->config->destroy(&((*self)->config));

  free(*self);
  *self = NULL;
}

/** Cria appconfig */
AppConfig* newAppConfig(HTTPServer* server, Router* router) {
  AppConfig* config = calloc(1, sizeof(AppConfig));

  config->destroy = destroyAppConfig;
  config->router = router;
  config->server = server;

  return config;
}

/** GET */
App* appget(const char* path, void (*handler)(Request* req, Response* res), struct App* self) {
  self->config->router->get(path, handler, self->config->router);
  return self;
}

/** POST */
App* apppost(const char* path, void (*handler)(Request* req, Response* res), struct App* self) {
  self->config->router->post(path, handler, self->config->router);
  return self;
}

/** PUT */
App* appput(const char* path, void (*handler)(Request* req, Response* res), struct App* self) {
  self->config->router->put(path, handler, self->config->router);
  return self;
}

/** DELETE */
App* appdelete(const char* path, void (*handler)(Request* req, Response* res), struct App* self) {
  self->config->router->delete(path, handler, self->config->router);
  return self;
}

/** Inicia servidor */
void applisten(App* self) {
  self->config->server->serve(self->config->server);
}
