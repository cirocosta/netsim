#ifndef NS__IP_H
#define NS__IP_H 

#include "netsim/common.h"

typedef struct ns_ip_t {
  const uint32_t version:4;           //  -+-
  const uint32_t header_length:4;    //   |  32
  const uint32_t tos:8;               //   | bits
  uint32_t datagram_length:16;        //  -+-

  uint32_t id:16;                   
  const uint32_t flags:3;
  const uint32_t frag_offset:13;

  uint32_t ttl:8;           
  uint32_t upper_layer_protocol:8;
  uint32_t checksum:16;

  uint32_t src_ip_addr;
  uint32_t dst_ip_addr;
  void* payload;
} ns_ip_t;

#endif
