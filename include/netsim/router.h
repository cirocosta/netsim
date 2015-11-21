#ifndef NS__ROUTER_T
#define NS__ROUTER_T

#include "netsim/common.h"
#include "netsim/interface.h"
#include "netsim/ft.h"
#include "netsim/thread.h"
#include "netsim/epoll.h"
#include "netsim/ev.h"
#include "netsim/table.h"

struct ns_device_t;

typedef struct ns_router_t {
  char name[NS_NAME_MAX];
  ns_interface_t* interfaces;
  uint8_t interfaces_count;

  uint16_t processing_time;
  ns_ft_t* forwarding_table;
} ns_router_t;

const static ns_router_t ns_zeroed_router = { 0 };

ns_router_t* ns_router_create(unsigned interfaces_count);
void ns_router_destroy(ns_router_t*);
void* ns_router_func(void* arg);

inline static void ns_router_init_ft(ns_router_t* router, unsigned ft_count)
{
  router->forwarding_table = ns_ft_create(ft_count);
}

#endif
