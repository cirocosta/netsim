#include "netsim/ft.h"

ns_ft_t* ns_ft_create(unsigned entries_count)
{

  ns_ft_t* ft = malloc(sizeof(*ft));
  PASSERT(ft, NS_ERR_MALLOC);

  ft->n = 0;
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

void ns_ft_add_entry(ns_ft_t* ft, const char* ip, uint8_t mask_l,
                     ns_interface_t* interface)
{
  ASSERT(ft->n < ft->entries_count, "FT reached its maximum of `%u`.",
         ft->entries_count);

  ft->entries[ft->n].ip_mask = ns_pton(ip);
  ft->entries[ft->n].mask_length = mask_l;
  ft->entries[ft->n].interface = interface;

  ft->n++;
}
