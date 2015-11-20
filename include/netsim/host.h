#ifndef NS__HOST_H
#define NS__HOST_H

#include "netsim/common.h"
#include "netsim/interface.h"
#include "netsim/addr.h"

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

int ns_host_send_pkt(ns_host_t* host, ns_addr_t* dst_addr, ns_ip_t* pkt);

#endif
