#ifndef NS__TABLE_H
#define NS__TABLE_H

#include "netsim/common.h"

typedef struct ns_table_entry_t {
  int key;
  void* data;
} ns_table_entry_t;

typedef struct ns_table_t {
  size_t entries_count;
  struct ns_table_entry_t* entries;
  size_t n;
} ns_table_t;

const static ns_table_entry_t ns_zeroed_table_entry = { 0 };
const static ns_table_t ns_zeroed_table = { 0 };

ns_table_t* ns_table_create(size_t entries_count);
void ns_table_add(ns_table_t* table, int key, void* data);
void ns_table_destroy(ns_table_t* table);

static int ns_table_comp_f(const void* a, const void* b)
{
  return ((ns_table_entry_t*)a)->key - ((ns_table_entry_t*)b)->key;
}

static inline void ns_table_init(ns_table_t* table)
{
  qsort(table->entries, table->entries_count, sizeof(*table->entries),
        ns_table_comp_f);
}

static inline void* ns_table_get(ns_table_t* table, int key)
{
  void* res =
      bsearch((const void*)(intptr_t)&key, table->entries, table->entries_count,
              sizeof(*table->entries), ns_table_comp_f);

  if (!res)
    return NULL;

  return ((ns_table_entry_t*)res)->data;
}

#endif
