#include "netsim/device.h"
#include <time.h>

const static struct timespec sleep_time = {.tv_sec = 0,
                                           .tv_nsec = 400 * 1000 * 1000 };

/**
 *            interface1  interface2
 *  ------l1-------> ROUTER <-------l2 ------
 */
void test1()
{
  ns_device_t* r1 = ns_device_create(NS_DEVICE_ROUTER, 2);
  ns_link_t* l1 = ns_link_create();
  ns_link_t* l2 = ns_link_create();
  ns_ip_t* packet1 = malloc(sizeof(*packet1));
  ns_ip_t* packet2 = malloc(sizeof(*packet2));

  r1->dev.router->interfaces[0].read_link = l1;
  r1->dev.router->interfaces[1].read_link = l2;

  /* ns_device_run(r1); */

  nanosleep(&sleep_time, NULL);
  ns_link_send(l1, packet1);
  ns_link_send(l2, packet2);

  ns_thread_join(&r1->thread, NULL);

  ns_device_destroy(r1);
  ns_link_destroy(l1);
  ns_link_destroy(l2);
  free(packet1);
  free(packet2);
}

int main(int argc, char* argv[])
{
  /* TEST(test1, ""); */

  return 0;
}
