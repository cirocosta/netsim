#ifndef NS__LINK_H
#define NS__LINK_H

#include "netsim/common.h"
#include "netsim/ip.h"
#include <semaphore.h>

typedef struct ns_link_t {
  uint16_t latency; // ms
  uint8_t band;     // Mbps
  ns_ip_t* buffer;  // one at a time

  sem_t empty;
  sem_t full;

  // sniffer struct provides a
  // sniffer* sniffing_function;  // function to call when actions
  // occur on this link so that we
  // can log them when a sniffer
  // attaches.
} ns_link_t;

const static ns_link_t ns_zeroed_link = { 0 };


ns_link_t* ns_link_create();
void ns_link_destroy(ns_link_t*);

void ns_link_send(ns_link_t* link, ns_ip_t* packet);
ns_ip_t* ns_link_recv(ns_link_t* link);

#endif
