#include "netsim/epoll.h"

ns_epoll_t* ns_epoll_create(unsigned events_count)
{
  ns_epoll_t* epoll = malloc(sizeof(*epoll));
  PASSERT(epoll, NS_ERR_MALLOC);

  epoll->events = calloc(NS_EPOLL_MAX_EVENTS, sizeof(struct epoll_event));
  PASSERT(epoll->events, NS_ERR_MALLOC);

  PASSERT(~(epoll->fd = epoll_create1(0)), "epoll_create1: ");

  return epoll;
}

void ns_epoll_destroy(ns_epoll_t* epoll)
{
  free(epoll->events);
  epoll->events = NULL;
  free(epoll);
}

void ns_epoll_add(ns_epoll_t* epoll, int fd, uint32_t flags)
{
  struct epoll_event ev = {.data.fd = fd, .events = flags };

  PASSERT(~(epoll_ctl(epoll->fd, EPOLL_CTL_ADD, fd, &ev)), "epoll_ctl: ");
}
