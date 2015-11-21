#ifndef NS__TCP_H
#define NS__TCP_H

#include "netsim/common.h"

typedef struct ns_tcp_t {
  uint16_t src_port;
  uint16_t dst_port;

  uint32_t seq_num;
  uint32_t ack_num;
  uint8_t ack : 1;
  uint8_t fin : 1;
  uint8_t syn : 1;

  void* payload;
} ns_tcp_t;

const static ns_tcp_t ns_zeroed_tcp = { 0 };

inline static ns_tcp_t* ns_tcp_create(void* payload, uint16_t src_port,
                                      uint16_t dst_port)
{
  ns_tcp_t* tcp = malloc(sizeof(*tcp));
  PASSERT(tcp, NS_ERR_MALLOC);
  *tcp = ns_zeroed_tcp;

  return tcp;
}

inline static void ns_tcp_destroy(ns_tcp_t* tcp_pkt) { free(tcp_pkt); }

#endif
