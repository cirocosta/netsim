
#ifndef NS__ADDR_H
#define NS__ADDR_H 

#include "netsim/common.h"

typedef struct ns_addr_t {
  char p_ip[16];              // presentation ip (255.255.255.255)
  uint32_t n_ip;              // numeric ip      (   0xffffffff  )
} ns_addr_t;

#endif
