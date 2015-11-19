#include "netsim/link.h"

ns_link_t* ns_link_create()
{
  ns_link_t* link = malloc(sizeof(*link));
  PASSERT(link, NS_ERR_MALLOC);

  *link = ns_zeroed_link;
  link->efd = eventfd(0, EFD_SEMAPHORE);

  return link;
}

void ns_link_destroy(ns_link_t* link) { free(link); }

void ns_link_send(ns_link_t* link, ns_ip_t* packet)
{
  const static uint64_t inc = 1;

  PASSERT(write(link->efd, &inc, sizeof(uint64_t)) == sizeof(uint64_t),
          "write efd: ");
  link->buffer = packet;
}

ns_ip_t* ns_link_recv(ns_link_t* link)
{
  uint64_t u;
  ns_ip_t* tmp;

  PASSERT(read(link->efd, &u, sizeof(uint64_t)) == sizeof(uint64_t),
          "read efd: ");
  tmp = link->buffer;

  return tmp;
}
