#include "Router.h" // Request, Response
#include "RouterConfig.h" // NULL
#include <stdlib.h> // NULL
#include "../../../util/Logger/Logger.h" // Logger, CreateLogger

Router* addGetRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self);
Router* addPostRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self);
Router* addPutRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self);
Router* addDeleteRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self);
void dispatchRoutes(HTTPConnection* connection, void* router);
void destroyRouter(Router** self);

Router* createRouter() {
  Router* router = calloc(1, sizeof(Router));

  router->config = newRouterConfig();
  router->get = addGetRoute;
  router->post = addPostRoute;
  router->put = addPutRoute;
  router->delete = addDeleteRoute;
  router->destroy = destroyRouter;
  
  return router;
}

/** Adiciona rota que processa requisições de metodo GET para um determinado recurso */
Router* addGetRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Router");

  /** Caso self não exista */
  if (self == NULL) {
    console->error(console, "addGetRoute failed because self is NULL!\n");
    console->destroy(&console);
    return self;
  }

  Map* getRoutes = self->config->routes->get(self->config->routes, "GET");

  /** Se Router foi criado de forma incorreta e não existe o grupo GET */
  if (getRoutes == NULL) {
    console->error(console, "Malformed Router! there is no GET routes\n");
    console->destroy(&console);
    return self;
  }

  /** Se os argumentos forem invalidos */
  if (path == NULL || handler == NULL) {
    console->error(console, "passed invalid args to addGetRoute\n");
    console->destroy(&console);
    return self;
  }

  /** Salva a rota */
  getRoutes->setAny(getRoutes, (char*) path, (void*) handler);

  console->destroy(&console);

  return self;
}

/** Adiciona rota que processa requisições de metodo POST para um determinado recurso */
Router* addPostRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Router");

  /** Caso self não exista */
  if (self == NULL) {
    console->error(console, "addPostRoute failed because self is NULL!\n");
    console->destroy(&console);
    return self;
  }

  Map* postRoutes = self->config->routes->get(self->config->routes, "POST");

  /** Se Router foi criado de forma incorreta e não existe o grupo POST */
  if (postRoutes == NULL) {
    console->error(console, "Malformed Router! there is no POST routes\n");
    console->destroy(&console);
    return self;
  }

  /** Se os argumentos forem invalidos */
  if (path == NULL || handler == NULL) {
    console->error(console, "passed invalid args to addPostRoute\n");
    console->destroy(&console);
    return self;
  }

  /** Salva a rota */
  postRoutes->setAny(postRoutes, (char*) path, (void*) handler);

  console->destroy(&console);

  return self;
}

/** Adiciona rota que processa requisições de metodo PUT para um determinado recurso */
Router* addPutRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Router");

  /** Caso self não exista */
  if (self == NULL) {
    console->error(console, "addPutRoute failed because self is NULL!\n");
    console->destroy(&console);
    return self;
  }

  Map* putRoutes = self->config->routes->get(self->config->routes, "PUT");

  /** Se Router foi criado de forma incorreta e não existe o grupo PUT */
  if (putRoutes == NULL) {
    console->error(console, "Malformed Router! there is no PUT routes\n");
    console->destroy(&console);
    return self;
  }

  /** Se os argumentos forem invalidos */
  if (path == NULL || handler == NULL) {
    console->error(console, "passed invalid args to addPutRoute\n");
    console->destroy(&console);
    return self;
  }

  /** Salva a rota */
  putRoutes->setAny(putRoutes, (char*) path, (void*) handler);

  console->destroy(&console);

  return self;
}

/** Adiciona rota que processa requisições de metodo DELETE para um determinado recurso */
Router* addDeleteRoute(const char* path, void (*handler)(Request* req, Response* res), Router* self) {
  /** Cria um logger pra esse namespace */
  Logger* console = createLogger();
  console->extend(console, "Router");

  /** Caso self não exista */
  if (self == NULL) {
    console->error(console, "addDeleteRoute failed because self is NULL!\n");
    console->destroy(&console);
    return self;
  }

  Map* deleteRoutes = self->config->routes->get(self->config->routes, "DELETE");

  /** Se Router foi criado de forma incorreta e não existe o grupo DELETE */
  if (deleteRoutes == NULL) {
    console->error(console, "Malformed Router! there is no DELETE routes\n");
    console->destroy(&console);
    return self;
  }

  /** Se os argumentos forem invalidos */
  if (path == NULL || handler == NULL) {
    console->error(console, "passed invalid args to addDeleteRoute\n");
    console->destroy(&console);
    return self;
  }

  /** Salva a rota */
  deleteRoutes->setAny(deleteRoutes, (char*) path, (void*) handler);

  console->destroy(&console);

  return self;
}

/** Encontra a rota */
void dispatchRoutes(HTTPConnection* connection, void* router) {
  char* path = connection->request.path;
  Map* routes = ((Router*) router)->config->routes;
  void (*handler)(Request* req, Response* res) = NULL;

  /** Seleciona o grupo de acordo com o metodo e encontra o handler com o path */
  switch (connection->request.method){
    case GET:
      Map* getRoutes = (Map*) routes->get(routes, "GET");
      handler = getRoutes->get(getRoutes, path);
      break;

    case POST:
      Map* postRoutes = (Map*) routes->get(routes, "POST");
      handler = postRoutes->get(postRoutes, path);
      break;

    case PUT:
      Map* putRoutes = (Map*) routes->get(routes, "PUT");
      handler = putRoutes->get(putRoutes, path);
      break;

    case DELETE:
      Map* deleteRoutes = (Map*) routes->get(routes, "DELETE");
      handler = deleteRoutes->get(deleteRoutes, path);
      break;
    
    default:
      handler = NULL;
      break;
  }

  /** Se encontrado o recurso */
  if (handler != NULL) handler(&(connection->request), connection->response);

  /** Não encontrado */
  else {
    connection->response
      ->withStatusCode(404, connection->response)
      ->withStatusMessage("Not Found", connection->response)
      ->withJSON(connection->response)
      ->addStringToJson("success", "false", connection->response)
      ->addStringToJson("data", "null", connection->response)
      ->addStringToJson("message", "The resource was not found", connection->response);
  }

  /** Envia a resposta e encerra a conexão */
  connection->sendResponse(connection);
  connection->close(connection);
  connection->destroy(&connection);
}

/** Destroi router */
void destroyRouter(Router** self) {
  if (*self) return;

  (*self)->config->destroy(&((*self)->config));

  free(*self);
  *self = NULL;
}