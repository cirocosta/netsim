#ifndef NS__EPOLL_H
#define NS__EPOLL_H

#include <sys/epoll.h>
#include "netsim/common.h"

#define NS_EPOLL_MAX_EVENTS 10

typedef struct ns_epoll_t {
  struct epoll_event* events;
  unsigned events_count;
  int fd;
} ns_epoll_t;

ns_epoll_t* ns_epoll_create(unsigned events_count);
void ns_epoll_destroy(ns_epoll_t* epoll);
void ns_epoll_add(ns_epoll_t* epoll, int fd, uint32_t flags);

inline static int ns_epoll_wait(ns_epoll_t* epoll)
{
  return epoll_wait(epoll->fd, epoll->events, NS_EPOLL_MAX_EVENTS, -1);
}

#endif
