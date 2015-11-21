#include "netsim/table.h"

ns_table_t* ns_table_create(size_t entries_count)
{
  ns_table_t* table = malloc(sizeof(*table));
  PASSERT(table, NS_ERR_MALLOC);

  *table = ns_zeroed_table;

  table->entries_count = entries_count;
  table->entries = calloc(table->entries_count, sizeof(*table->entries));
  PASSERT(table->entries, NS_ERR_MALLOC);

  return table;
}

void ns_table_add(ns_table_t* table, int key, void* data)
{
  ASSERT(table->n < table->entries_count, "Table is full (`%lu`)",
         table->entries_count);

  table->entries[table->n].key = key;
  table->entries[table->n].data = data;
  table->n++;
}

void ns_table_destroy(ns_table_t* table)
{
  free(table->entries);
  table->entries = NULL;
  free(table);
}
