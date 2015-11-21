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

  // TODO expose an 'interface setter' which would let us
  //      individually init the interface with the proper
  //      queue size.
  for (int i = 0; i < router->interfaces_count; i++)
    ns_interface_init_queue(&router->interfaces[i], 100);

  return router;
}

void ns_router_destroy(ns_router_t* router)
{
  // FIXME this sucks ...
  for (int i = 0; i < router->interfaces_count; i++) {
    if (router->interfaces[i].packet_queue)
      ns_queue_destroy(router->interfaces[i].packet_queue);
  }

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
  ns_ip_t* pkt;
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

      switch (ev->type) {
        case NS_EV_TERM:
          should_stop = 1;
          read(ev->fd, &dummy_u, sizeof(uint64_t));
          break;

        case NS_EV_TICK:
          read(ev->fd, &dummy_u, sizeof(uint64_t));
          // remove packets from the queue and
          // forward them accordingly
          break;

        case NS_EV_LINK:
          pkt = ns_link_recv(ev->link);
          /* ns_queue_insert(ev->interface->packet_queue, pkt); */
          break;

        default:
          ASSERT(0, "Unknown event type :(");
      }
    }
  }

  pthread_exit(NULL);
}
