#ifndef NS__TRANSPORT_H
#define NS__TRANSPORT_H

#include "netsim/tcp.h"
#include "netsim/udp.h"

typedef struct ns_transport_t {
  ns_transport_e type;
  union {
    ns_tcp_t* tcp;
    ns_udp_t* udp;
  } payload;
} ns_transport_t;

inline static ns_transport_t* ns_transport_create_udp(ns_udp_t* udp)
{
  ns_transport_t* transport = malloc(sizeof(*transport));
  PASSERT(transport, NS_ERR_MALLOC);

  transport->type = NS_TRANSPORT_UDP;
  transport->payload.udp = udp;

  return transport;
}

inline static ns_transport_t* ns_transport_create_tcp(ns_tcp_t* tcp)
{
  ns_transport_t* transport = malloc(sizeof(*transport));
  PASSERT(transport, NS_ERR_MALLOC);

  transport->type = NS_TRANSPORT_TCP;
  transport->payload.tcp = tcp;

  return transport;
}

inline static void ns_transport_destroy(ns_transport_t* transport)
{
  if (transport->type == NS_TRANSPORT_UDP) {
    ns_udp_destroy(transport->payload.udp);
  } else if (transport->type == NS_TRANSPORT_TCP) {
    ns_tcp_destroy(transport->payload.tcp);
  } else {
    ASSERT(0, "Unknonw transport type `%d`", transport->type);
  }

  free(transport);
}

#endif
