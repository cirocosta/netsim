#include "netsim/router.h"

ns_router_t* ns_router_create(unsigned interfaces_count)
{
  ns_router_t* router = malloc(sizeof(*router));
  PASSERT(router, NS_ERR_MALLOC);

  *router = ns_zeroed_router;
  router->interfaces_count = interfaces_count;
  router->interfaces = calloc(interfaces_count, sizeof(*router->interfaces));
  PASSERT(router->interfaces, NS_ERR_MALLOC);

  for (int i = 0; i < router->interfaces_count; i++)
    router->interfaces[i] = ns_interface_create();

  return router;
}

void ns_router_destroy(ns_router_t* router)
{
  while (router->interfaces_count-- > 0)
    ns_interface_destroy(router->interfaces[router->interfaces_count]);
  free(router->interfaces);
  router->interfaces = NULL;

  free(router);
}
