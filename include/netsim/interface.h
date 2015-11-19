#ifndef NS__INTERFACE_H
#define NS__INTERFACE_H

#include "netsim/common.h"
#include "netsim/addr.h"
#include "netsim/link.h"
#include "netsim/queue.h"

typedef struct ns_interface_t {
  ns_addr_t addr;
  ns_link_t* read_link;
  ns_link_t* write_link;
  ns_queue_t* packet_queue;
} ns_interface_t;

ns_interface_t* ns_interface_create();
void ns_interface_destroy(ns_interface_t*);

inline static void ns_interface_init_queue(ns_interface_t* interface,
                                           unsigned queue_size)
{
  interface->packet_queue = ns_queue_create(queue_size);
}

#endif
