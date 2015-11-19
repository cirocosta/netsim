#ifndef NS__SIMULATOR_H
#define NS__SIMULATOR_H

#include "netsim/common.h"
#include "netsim/device.h"

typedef struct ns_simulator_t {
  ns_device_t** devices;
  unsigned devices_count;
} ns_simulator_t;

ns_simulator_t* ns_simulator_create(unsigned devices_count);
void ns_simulator_destroy(ns_simulator_t* simulator);

#endif
