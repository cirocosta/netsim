#ifndef NS__FT_H
#define NS__FT_H

#include "netsim/common.h"
#include "netsim/net.h"
#include "netsim/interface.h"

typedef struct ns_ft_entry_t {
  uint32_t ip_mask;
  uint8_t mask_length;
  ns_interface_t* interface;
} nt_ft_entry_t;

typedef struct ns_ft_t {
  struct ns_ft_entry_t* entries;
  uint16_t entries_count;
  uint16_t n;
} ns_ft_t;

ns_ft_t* ns_ft_create(unsigned entries_count);
void ns_ft_destroy(ns_ft_t*);
void ns_ft_add_entry(ns_ft_t* ft, const char* ip, uint8_t mask_l,
                     ns_interface_t* interface);

ns_interface_t* ns_ft_forward(ns_ft_t*, uint32_t ip);

#endif
