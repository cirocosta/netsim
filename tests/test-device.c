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
  ns_router_t* router = r1->dev.router;

  ns_device_init_interface(r1, 0, l1, NULL);
  ns_device_init_interface(r1, 1, l2, NULL);
  ns_device_run(r1);

  nanosleep(&sleep_time, NULL);
  ns_link_send(l1, packet1);
  ns_link_send(l2, packet2);
  nanosleep(&sleep_time, NULL);

  ASSERT(ns_queue_front(router->interfaces[0].packet_queue) == packet1, "");
  ASSERT(ns_queue_front(router->interfaces[1].packet_queue) == packet2, "");

  ns_device_terminate(r1);
  ns_thread_join(&r1->thread, NULL);

  ns_device_destroy(r1);
  ns_link_destroy(l1);
  ns_link_destroy(l2);
  free(packet1);
  free(packet2);
}

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
  ns_device_t* r1 = ns_device_create(NS_DEVICE_ROUTER, 2);
  ns_link_t* l1 = ns_link_create();
  ns_link_t* l2 = ns_link_create();
  ns_ip_t* packet1 = malloc(sizeof(*packet1));
  ns_router_t* router = r1->dev.router;

  packet1->dst_ip_addr = ns_pton("10.1.1.2");
  packet1->src_ip_addr = ns_pton("10.0.0.2");

  ns_device_init_interface(r1, 0, l1, NULL);
  ns_device_init_interface(r1, 1, NULL, l2);
  ns_router_init_ft(router, 2);

  ns_ft_add_entry(router->forwarding_table, "10.0.0.0", 8,
                  &router->interfaces[0]);
  ns_ft_add_entry(router->forwarding_table, "10.1.1.0", 8,
                  &router->interfaces[1]);

  ns_device_run(r1);

  ASSERT(l2->buffer == NULL, "");
  nanosleep(&sleep_time, NULL);

  ns_link_send(l1, packet1);
  ASSERT(l1->buffer == packet1, "");
  nanosleep(&sleep_time, NULL);

  ns_device_tick(r1);
  nanosleep(&sleep_time, NULL);

  ASSERT(l1->buffer == NULL, "");
  ASSERT(l2->buffer == packet1, "packet was correctly forwarded");

  ns_device_terminate(r1);
  ns_thread_join(&r1->thread, NULL);

  ns_device_destroy(r1);
  ns_link_destroy(l1);
  ns_link_destroy(l2);
  free(packet1);
}

/**
 *
 *     h1 --------+
 * (10.0.0.2)     |
 *                | l1
 * (10.1.1.2)     |
 *     h2 --------+
 *
 *
 * test: send udp pkt from h1 to h2
 */
void test3()
{
  ns_device_t* h1 = ns_device_create(NS_DEVICE_HOST, 2);
  ns_device_t* h2 = ns_device_create(NS_DEVICE_HOST, 2);
  ns_link_t* l1 = ns_link_create();
  ns_transport_t* udp_pkt =
      ns_transport_create_udp(ns_udp_create(NULL, 8080, 8080));

  ns_device_init_interface(h1, 0, NULL, l1);
  ns_device_init_interface(h2, 0, l1, NULL);

  ns_device_run(h1);
  ns_device_run(h2);
  nanosleep(&sleep_time, NULL);

  ns_host_send_transport_pkt(h1->dev.host, udp_pkt, ns_pton("10.0.0.2"),
                             ns_pton("10.1.1.2"));
  nanosleep(&sleep_time, NULL);

  ns_device_terminate(h1);
  ns_device_terminate(h2);
  ns_thread_join(&h1->thread, NULL);
  ns_thread_join(&h2->thread, NULL);

  ns_link_destroy(l1);
  ns_device_destroy(h1);
  ns_device_destroy(h2);
}

int main(int argc, char* argv[])
{
  TEST(test1, "Sending packages to devices' interfaces' buffers");
  TEST(test2, "");
  TEST(test3, "");

  return 0;
}
