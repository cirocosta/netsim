#ifndef NS__IP_H
#define NS__IP_H

#include "netsim/common.h"
#include "netsim/transport.h"

typedef struct ns_ip_t {
  const uint32_t version : 4;       //  -+-
  const uint32_t header_length : 4; //   |  32
  const uint32_t tos : 8;           //   | bits
  uint32_t datagram_length : 16;    //  -+-

  uint32_t id : 16;
  const uint32_t flags : 3;
  const uint32_t frag_offset : 13;

  uint32_t ttl : 8;
  uint32_t upper_layer_protocol : 8;
  uint32_t checksum : 16;

  uint32_t src_ip_addr;
  uint32_t dst_ip_addr;
  ns_transport_t* payload;
} ns_ip_t;

const static ns_ip_t ns_zeroed_ip = { 0 };

inline static ns_ip_t* ns_ip_create(ns_transport_t* transport_pkt, uint32_t src,
                                    uint32_t dst)
{
  ns_ip_t* ip = malloc(sizeof(*ip));
  PASSERT(ip, NS_ERR_MALLOC);
  *ip = ns_zeroed_ip;

  ip->src_ip_addr = src;
  ip->dst_ip_addr = src;
  ip->payload = transport_pkt;

  ip->upper_layer_protocol = transport_pkt->type == NS_TRANSPORT_TCP
                                 ? NS_NUMBER_TCP_PROTOCOL
                                 : NS_NUMBER_UDP_PROTOCOL;

  return ip;
}
inline static void ns_ip_destroy(ns_ip_t* ip)
{
  if (ip->payload)
    ns_transport_destroy(ip->payload);
  free(ip);
}

#endif
