#include "netsim/host.h"
#include "netsim/device.h"

ns_host_t* ns_host_create()
{
  ns_host_t* host = malloc(sizeof(*host));
  PASSERT(host, NS_ERR_MALLOC);

  *host = ns_zeroed_host;
  host->interfaces = malloc(sizeof(*host->interfaces));
  PASSERT(host->interfaces, NS_ERR_MALLOC);

  return host;
}

void ns_host_destroy(ns_host_t* host)
{
  free(host->interfaces);
  free(host);
}

void* ns_host_func(void* arg)
{
  struct ns_device_t* device = (struct ns_device_t*)arg;
  uint8_t should_stop = 0;
  static uint64_t dummy_u;
  int n = 0;
  ns_ev_t* ev = NULL;
  ns_ip_t* pkt = NULL;
  ns_link_t* link = NULL;
  ns_host_t* host = device->dev.host;

  while (!should_stop) {
    n = ns_epoll_wait(device->epoll);
    for (int i = 0; i < n; i++) {
      if ((device->epoll->events[i].events & EPOLLERR) ||
          (device->epoll->events[i].events & EPOLLHUP)) {
        LOGERR("epoll error");
        continue;
      }

      if (!(device->epoll->events[i].events & EPOLLIN))
        continue;

      ev = (ns_ev_t*)device->epoll->events[i].data.ptr;

      switch (ev->type) {
        case NS_EV_TERM:
          should_stop = 1;
          read(ev->fd, &dummy_u, sizeof(uint64_t));
          break;

        case NS_EV_LINK:
          pkt = ns_link_recv(host->interfaces[0].read_link);
          LOGERR("pkt came!");
          // process the packet -->
          //  pass it to the transport layer
          ns_ip_destroy(pkt);
          break;

        default:
          ASSERT(0, "ev->type not supported");
      }
    }
  }

  pthread_exit(NULL);
}

void ns_host_send_transport_pkt(ns_host_t* host, ns_transport_t* transport,
                                uint32_t src, uint32_t dst)
{
  ns_ip_t* pkt = ns_ip_create(transport, src, dst);
  ns_link_send(host->interfaces[0].write_link, pkt);
}
