#ifndef NS__HOST_H
#define NS__HOST_H

#include "netsim/common.h"
#include "netsim/interface.h"
#include "netsim/addr.h"
#include "netsim/thread.h"
#include "netsim/transport.h"

typedef struct ns_host_t {
  char name[NS_NAME_MAX];
  ns_interface_t* interfaces;

  ns_addr_t router_addr;
  ns_addr_t dns_addr;
  ns_app_e app;
} ns_host_t;

const static ns_host_t ns_zeroed_host = { 0 };

ns_host_t* ns_host_create();
void ns_host_destroy(ns_host_t*);
void* ns_host_func(void* arg);

void ns_host_send_transport_pkt(ns_host_t* host, ns_transport_t* transport,
                                uint32_t src, uint32_t dst);

#endif
