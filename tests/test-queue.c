#include "netsim/queue.h"

#include <stdio.h>

void test1()
{
  ns_queue_t* q = ns_queue_create(10);
  ns_ip_t* hue = malloc(sizeof(ns_ip_t));
  ns_ip_t* br = malloc(sizeof(ns_ip_t));
  ns_ip_t* front;

  ASSERT(ns_queue_empty(q) == 1, "Must start empty");

  ASSERT(ns_queue_insert(q, hue) == 0, "Must receive it's 'ticket' correctly");
  front = ns_queue_front(q);
  ASSERT(front == hue, "");

  ASSERT(ns_queue_insert(q, br) == 1, "Must receive it's 'ticket' correctly");
  ASSERT(ns_queue_empty(q) == 0, "Not empty after inserts");

  front = ns_queue_front(q);
  ASSERT(front == hue, "Didn't removed, must remain in front");

  free(front);
  ns_queue_remove(q);
  ASSERT(ns_queue_empty(q) == 0, "");

  front = ns_queue_front(q);
  ASSERT(front == br, "");

  ns_queue_remove(q);
  ASSERT(ns_queue_empty(q) == 1, "Ends empty after removing");

  ns_queue_destroy(q);
  free(br);
}

void test2()
{
  ns_queue_t* q = ns_queue_create(2);
  ns_ip_t* ip1 = malloc(sizeof(ns_ip_t));
  ns_ip_t* ip2 = malloc(sizeof(ns_ip_t));
  ns_ip_t* front;

  ASSERT(ns_queue_empty(q) == 1, "Must start empty");
  ASSERT(ns_queue_insert(q, ip1) == 0, "Must receive it's 'ticket' correctly");
  ASSERT(ns_queue_insert(q, ip2) == 1, "Must receive it's 'ticket' correctly");
  ASSERT(ns_queue_empty(q) == 0, "Not empty after inserts");
  ASSERT(ns_queue_full(q) == 1, "But full!");

  free(ip1);
  free(ip2);

  ns_queue_destroy(q);
}

int main(int argc, char* argv[])
{
  TEST(test1, "basic ops");
  TEST(test2, "full queue");

  return 0;
}
