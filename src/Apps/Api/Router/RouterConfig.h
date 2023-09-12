#include "../../../util/Map/Map.h" // Map

#ifndef ROUTERCONFIG_H
#define ROUTERCONFIG_H

typedef struct _RouterConfig
{
  /** Rotas registradas */
  Map* routes;
  /** Destrou o objeto RouterConfig */
  void (*destroy)(struct _RouterConfig** self);
} RouterConfig;

/** Cria novo objeto RouterConfig */
RouterConfig* newRouterConfig();

#endif // ROUTERCONFIG_H