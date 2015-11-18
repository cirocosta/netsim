#include "netsim/link.h"

ns_link_t* ns_link_create()
{
  ns_link_t* link = malloc(sizeof(*link));
  PASSERT(link, NS_ERR_MALLOC);

  *link = ns_zeroed_link;

  PASSERT(sem_init(&link->empty, 0, 1) == 0, "sem_init: ");
  PASSERT(sem_init(&link->full, 0, 0) == 0, "sem_init: ");

  return link;
}

void ns_link_destroy(ns_link_t* link)
{
  PASSERT(sem_destroy(&link->empty) == 0, "sem_destroy: ");
  PASSERT(sem_destroy(&link->full) == 0, "sem_destroy: ");

  free(link);
}

void ns_link_send(ns_link_t* link, ns_ip_t* packet)
{
  sem_wait(&link->empty);
  link->buffer = packet;
  sem_post(&link->full);
}

ns_ip_t* ns_link_recv(ns_link_t* link)
{
  ns_ip_t* tmp;

  sem_wait(&link->full);
  tmp = link->buffer;
  link->buffer = NULL;
  sem_post(&link->empty);

  return tmp;
}
