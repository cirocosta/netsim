#include "netsim/router.h"
#include "netsim/device.h"

ns_router_t* ns_router_create(unsigned interfaces_count)
{
  ns_router_t* router = malloc(sizeof(*router));
  PASSERT(router, NS_ERR_MALLOC);

  *router = ns_zeroed_router;

  router->interfaces_count = interfaces_count;
  router->interfaces =
      calloc(router->interfaces_count, sizeof(*router->interfaces));
  PASSERT(router->interfaces, NS_ERR_MALLOC);

  return router;
}

void ns_router_destroy(ns_router_t* router)
{
  free(router->interfaces);
  router->interfaces = NULL;

  if (router->forwarding_table) {
    ns_ft_destroy(router->forwarding_table);
    router->forwarding_table = NULL;
  }

  free(router);
}

void* ns_router_func(void* arg)
{
  static uint64_t dummy_u;
  struct ns_device_t* device = (struct ns_device_t*)arg;
  ns_ev_t* ev = NULL;
  uint8_t should_stop = 0;
  int n;

  while (!should_stop) {
    n = ns_epoll_wait(device->epoll);
    for (int i = 0; i < n; i++) {
      if ((device->epoll->events[i].events & EPOLLERR) ||
          (device->epoll->events[i].events & EPOLLHUP) ||
          (!(device->epoll->events[i].events & EPOLLIN))) {
        LOGERR("epoll error");
        continue;
      }

      ev = (ns_ev_t*)device->epoll->events[i].data.ptr;
      read(ev->fd, &dummy_u, sizeof(uint64_t));

      switch (ev->type) {
        case NS_EV_TERM:
          LOGERR("TERMINATING!");
          should_stop = 1;
          break;
        case NS_EV_TICK:
          LOGERR("TICK!");
          break;
        case NS_EV_LINK:
          LOGERR("RECEIVED LINK!");
          /* ns_link_recv(ev->link); */
          break;
        default:
          ASSERT(0, "Unknown event type :(");
      }
    }
  }

  pthread_exit(NULL);
}
