#ifndef NS__UDP_H
#define NS__UDP_H

#include "netsim/common.h"

typedef struct ns_udp_t {
  uint16_t src_port;
  uint16_t dst_port;

  void* payload;
} ns_udp_t;

const static ns_udp_t ns_zeroed_udp = { 0 };

inline static ns_udp_t* ns_udp_create(void* payload, uint16_t src_port,
                                      uint16_t dst_port)
{
  ns_udp_t* udp = malloc(sizeof(*udp));
  PASSERT(udp, NS_ERR_MALLOC);

  udp->src_port = src_port;
  udp->dst_port = dst_port;
  udp->payload = payload;

  return udp;
}

inline static void ns_udp_destroy(ns_udp_t* udp_pkt) { free(udp_pkt); }

#endif
