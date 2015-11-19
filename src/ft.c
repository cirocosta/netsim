#include "netsim/ft.h"

ns_ft_t* ns_ft_create(unsigned entries_count)
{

  ns_ft_t* ft = malloc(sizeof(*ft));
  PASSERT(ft, NS_ERR_MALLOC);

  ft->entries_count = entries_count;
  ft->entries = calloc(ft->entries_count, sizeof(*ft->entries));
  PASSERT(ft->entries, NS_ERR_MALLOC);

  return ft;
}

void ns_ft_destroy(ns_ft_t* ft)
{
  free(ft->entries);
  ft->entries = NULL;
  free(ft);
}

ns_interface_t* ns_ft_forward(ns_ft_t* ft, uint32_t ip)
{
  for (int i = 0; i < ft->entries_count; i++) {
    if ((ft->entries[i].ip_mask >> ft->entries[i].mask_length) ==
        (ip >> ft->entries[i].mask_length)) {
      return ft->entries[i].interface;
    }
  }

  return NULL;
}
