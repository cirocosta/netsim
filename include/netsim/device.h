#ifndef NS__DEVICE_H
#define NS__DEVICE_H

#include "netsim/common.h"
#include "netsim/thread.h"
#include "netsim/link.h"
#include "netsim/host.h"
#include "netsim/router.h"

typedef union ns_dev_u {
  ns_host_t* host; 
  ns_router_t* router; 
} ns_dev_u;

typedef struct ns_device_t {
  ns_thread_t thread;
  ns_dev_u dev;
  ns_device_e type;

  uint8_t running;
} ns_device_t;

const static ns_device_t ns_zeroed_device = { 0 };

ns_device_t* ns_device_create(ns_device_e type, unsigned interfaces_count);
void ns_device_destroy(ns_device_t*);
void ns_device_run(ns_device_t*);

#endif
