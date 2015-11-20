#ifndef NS__EPOLL_H
#define NS__EPOLL_H

#include "netsim/common.h"
#include "netsim/ev.h"

#include <sys/epoll.h>
#include <sys/signal.h>

#define NS_EPOLL_MAX_EVENTS 10

typedef struct ns_epoll_t {
  struct epoll_event* events;
  unsigned events_count;
  ns_ev_t* evs;
  unsigned n;
  int fd;
} ns_epoll_t;

ns_epoll_t* ns_epoll_create(unsigned events_count);
void ns_epoll_destroy(ns_epoll_t* epoll);

void ns_epoll_add_link_ev(ns_epoll_t* epoll, ns_link_t*, int fd,
                          uint32_t flags);
void ns_epoll_add_term_ev(ns_epoll_t* epoll, int fd);
void ns_epoll_add_tick_ev(ns_epoll_t* epoll, int fd);

inline static int ns_epoll_wait(ns_epoll_t* epoll)
{
  return epoll_wait(epoll->fd, epoll->events, NS_EPOLL_MAX_EVENTS, -1);
}

#endif
