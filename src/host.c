#include "netsim/host.h"

ns_host_t* ns_host_create()
{
  ns_host_t* host = malloc(sizeof(*host));
  PASSERT(host, NS_ERR_MALLOC);

  *host = ns_zeroed_host;

  return host;
}

void ns_host_destroy(ns_host_t* host)
{
  free(host);
}
