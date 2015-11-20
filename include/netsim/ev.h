#ifndef NS__EV_H
#define NS__EV_H

#include "netsim/common.h"
#include "netsim/link.h"

typedef struct ns_ev_t {
  ns_ev_e type;
  int fd;
  ns_link_t* link;
} ns_ev_t;

#endif
