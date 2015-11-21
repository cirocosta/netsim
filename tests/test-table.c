#include "netsim/table.h"

typedef struct double_t {
  double number;
} double_t;

void test1()
{
  double_t d1 = {.number = 100000.0 };
  double_t d2 = {.number = 600000.0 };
  double_t d3 = {.number = 200000.0 };
  double_t d4 = {.number = 300000.0 };

  ns_table_t* table = ns_table_create(4);

  ns_table_add(table, 93, &d1);
  ns_table_add(table, 12, &d2);
  ns_table_add(table, 1, &d4);
  ns_table_add(table, 103, &d3);

  ns_table_init(table);

  ASSERT(ns_table_get(table, 93) == &d1, "");
  ASSERT(ns_table_get(table, 1) == &d4, "");
  ASSERT(ns_table_get(table, 103) == &d3, "");
  ASSERT(ns_table_get(table, 12) == &d2, "");

  ns_table_destroy(table);
}

int main(int argc, char* argv[])
{
  TEST(test1, "");

  return 0;
}
