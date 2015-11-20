#include "netsim/epoll.h"

ns_epoll_t* ns_epoll_create(unsigned events_count)
{
  ns_epoll_t* epoll = malloc(sizeof(*epoll));
  PASSERT(epoll, NS_ERR_MALLOC);

  epoll->n = 0;
  epoll->events_count = events_count;
  epoll->events = calloc(epoll->events_count, sizeof(*epoll->events));
  PASSERT(epoll->events, NS_ERR_MALLOC);

  epoll->evs = calloc(epoll->events_count, sizeof(*epoll->evs));
  PASSERT(epoll->evs, NS_ERR_MALLOC);
  PASSERT(~(epoll->fd = epoll_create1(0)), "epoll_create1: ");

  return epoll;
}

void ns_epoll_destroy(ns_epoll_t* epoll)
{
  free(epoll->evs);
  epoll->evs = NULL;
  free(epoll->events);
  epoll->events = NULL;
  free(epoll);
}

// FIXME deduplicate this stuff below.

void ns_epoll_add_link_ev(ns_epoll_t* epoll, ns_link_t* link, int fd,
                          uint32_t flags)
{
  ASSERT(epoll->n < epoll->events_count,
         "Can't add more than `%u` events to this epoll", epoll->events_count);

  struct epoll_event event = { 0 };
  ns_ev_t* ev = &epoll->evs[epoll->n++];

  ev->type = NS_EV_LINK;
  ev->link = link;
  ev->fd = fd;

  event.data.ptr = (void*)ev;
  event.events = flags;

  PASSERT(~(epoll_ctl(epoll->fd, EPOLL_CTL_ADD, fd, &event)), "epoll_ctl: ");
}

void ns_epoll_add_term_ev(ns_epoll_t* epoll, int fd)
{
  ASSERT(epoll->n < epoll->events_count,
         "Can't add more than `%u` events to this epoll", epoll->events_count);

  struct epoll_event event = { 0 };
  ns_ev_t* ev = &epoll->evs[epoll->n++];

  ev->type = NS_EV_TERM;
  ev->link = NULL;
  ev->fd = fd;

  event.data.ptr = (void*)ev;
  event.events = EPOLLET | EPOLLIN;

  PASSERT(~(epoll_ctl(epoll->fd, EPOLL_CTL_ADD, fd, &event)), "epoll_ctl: ");
}

void ns_epoll_add_tick_ev(ns_epoll_t* epoll, int fd)
{
  ASSERT(epoll->n < epoll->events_count,
         "Can't add more than `%u` events to this epoll", epoll->events_count);

  struct epoll_event event = { 0 };
  ns_ev_t* ev = &epoll->evs[epoll->n++];

  ev->type = NS_EV_TICK;
  ev->link = NULL;
  ev->fd = fd;

  event.data.ptr = (void*)ev;
  event.events = EPOLLET | EPOLLIN;

  PASSERT(~(epoll_ctl(epoll->fd, EPOLL_CTL_ADD, fd, &event)), "epoll_ctl: ");
}
