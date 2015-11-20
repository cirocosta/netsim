#include "netsim/device.h"
#include <time.h>

const static struct timespec sleep_time = {.tv_sec = 0,
                                           .tv_nsec = 400 * 1000 * 1000 };

/**
 *
 *              l1
 *     h1 -------------+
 * (10.0.0.2)          |  (10.0.0.1)
 *                     r1 
 * (10.1.1.2)          |  (10.1.1.1)
 *     h2 -------------+
 *             l2
 *
 * test: send a package from h1 to h2
 */
void test2()
{
}


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

  ns_device_init_interface(r1, 0, l1, NULL);
  ns_device_init_interface(r1, 1, l2, NULL);
  ns_device_run(r1);

  nanosleep(&sleep_time, NULL);
  ns_link_send(l1, packet1);
  ns_link_send(l2, packet2);
  nanosleep(&sleep_time, NULL);

  ns_device_terminate(r1);
  ns_thread_join(&r1->thread, NULL);

  ns_device_destroy(r1);
  ns_link_destroy(l1);
  ns_link_destroy(l2);
  free(packet1);
  free(packet2);
}

int main(int argc, char* argv[])
{
  TEST(test1, "");

  return 0;
}
