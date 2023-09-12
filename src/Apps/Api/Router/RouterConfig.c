#include "RouterConfig.h" // Map
#include <stdlib.h> // NULL

/** Destrou o objeto RouterConfig */
void destroyRouterConfig(RouterConfig** self) {
  if (!(*self)) return;

  (*self)->routes->destroy(&((*self)->routes));

  free(*self);
}

RouterConfig* newRouterConfig() {
  Map* routes = newMap();
  RouterConfig* config = calloc(1, sizeof(RouterConfig));

  routes->nest(routes, "GET");
  routes->nest(routes, "HEAD");
  routes->nest(routes, "POST");
  routes->nest(routes, "PUT");
  routes->nest(routes, "DELETE");
  routes->nest(routes, "CONNECT");
  routes->nest(routes, "OPTIONS");
  routes->nest(routes, "TRACE");
  routes->nest(routes, "PATCH");

  config->destroy = destroyRouterConfig;
  config->routes = routes;
}
