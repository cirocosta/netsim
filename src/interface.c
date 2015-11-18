#include "netsim/interface.h"

ns_interface_t* ns_interface_create()
{
  ns_interface_t* interface = malloc(sizeof(*interface));
  PASSERT(interface, NS_ERR_MALLOC);

  return interface;
}

void ns_interface_destroy(ns_interface_t* interface) { free(interface); }
