#ifndef NS__INTERFACE_H
#define NS__INTERFACE_H 

#include "netsim/common.h"
#include "netsim/addr.h"
#include "netsim/link.h"

typedef struct ns_interface_t { 
  ns_addr_t addr;
  ns_link_t* read_link;
  ns_link_t* write_link;
  /* queue_t* packets_queue; */
} ns_interface_t;


ns_interface_t* ns_interface_create();
void ns_interface_destroy(ns_interface_t*);

#endif
