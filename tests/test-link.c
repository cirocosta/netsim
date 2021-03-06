#include "netsim/link.h"
#include "netsim/device.h"
#include <time.h>

const static struct timespec sleep_time = {.tv_sec = 0,
                                           .tv_nsec = 100 * 1000 * 1000 };

void test1()
{
  ns_link_t* link = ns_link_create();
  ns_ip_t packet;

  ns_link_send(link, &packet);
  ASSERT(ns_link_recv(link) == &packet, "");

  ns_link_destroy(link);
}

void* test2_thread0(void* arg)
{
  ns_link_t* link = (ns_link_t*)arg;
  ns_ip_t* packet = malloc(sizeof(*packet));

  nanosleep(&sleep_time, NULL);

  packet->id = 666;
  ns_link_send(link, packet);

  pthread_exit(0);
}

void* test2_thread1(void* arg)
{
  ns_link_t* link = (ns_link_t*)arg;

  ns_ip_t* packet = ns_link_recv(link);
  ASSERT(packet->id == 666, "`%d` != `666`", packet->id);

  free(packet);
  pthread_exit(0);
}

void test2()
{
  ns_device_t* h0 = ns_device_create(NS_DEVICE_HOST, 1);
  ns_device_t* h1 = ns_device_create(NS_DEVICE_HOST, 1);
  ns_link_t* l1 = ns_link_create();

  h0->dev.host->interfaces[0].write_link = l1;
  h1->dev.host->interfaces[0].read_link = l1;

  ns_thread_create(&h1->thread, NULL, test2_thread1,
                   (void*)h1->dev.host->interfaces[0].read_link);
  ns_thread_create(&h0->thread, NULL, test2_thread0,
                   (void*)h0->dev.host->interfaces[0].write_link);

  ns_thread_join(&h0->thread, NULL);
  ns_thread_join(&h1->thread, NULL);

  ns_device_destroy(h0);
  ns_device_destroy(h1);
  ns_link_destroy(l1);
}

void test3()
{
  ns_device_t* h0 = ns_device_create(NS_DEVICE_HOST, 1);
  ns_device_t* r1 = ns_device_create(NS_DEVICE_ROUTER, 1);
  ns_link_t* l1 = ns_link_create();

  h0->dev.host->interfaces[0].write_link = l1;
  r1->dev.router->interfaces[0].read_link = l1;

  ns_thread_create(&h0->thread, NULL, test2_thread0,
                   (void*)h0->dev.host->interfaces[0].write_link);
  ns_thread_create(&r1->thread, NULL, test2_thread1,
                   (void*)r1->dev.router->interfaces[0].read_link);

  ns_thread_join(&h0->thread, NULL);
  ns_thread_join(&r1->thread, NULL);

  ns_device_destroy(h0);
  ns_device_destroy(r1);
  ns_link_destroy(l1);
}

int main(int argc, char* argv[])
{
  TEST(test1, "Single-thread link send and recv - hosts");
  TEST(test2, "multi-thread link send and recv - hosts");
  TEST(test3, "multi-thread link send and recv - router and host");

  return 0;
}
