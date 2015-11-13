#include "netsim/link.h"

void ns_link_send(ns_link_t* link, ns_ip_t* packet)
{
  link->buffer = packet;
}

ns_ip_t* ns_link_recv(ns_link_t* link)
{
  ns_ip_t* tmp = link->buffer;
  link->buffer = NULL;

  return tmp;
}
