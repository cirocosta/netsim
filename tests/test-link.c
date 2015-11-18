#include "netsim/link.h"
#include "netsim/interface.h"
#include "netsim/common.h"
#include "netsim/device.h"

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

  h0->dev.host->interface.write_link = l1;
  h1->dev.host->interface.read_link = l1;

  fs_thread_create(&h0->thread.tid, NULL, test2_thread0,
                   (void*)h0->dev.host->interface.write_link);
  fs_thread_create(&h1->thread.tid, NULL, test2_thread1,
                   (void*)h1->dev.host->interface.read_link);

  fs_thread_join(h0->thread.tid, NULL);
  fs_thread_join(h1->thread.tid, NULL);

  ns_device_destroy(h0);
  ns_device_destroy(h1);
  ns_link_destroy(l1);
}

void test3()
{
  ns_device_t* h0 = ns_device_create(NS_DEVICE_HOST, 1);
  ns_device_t* r1 = ns_device_create(NS_DEVICE_ROUTER, 1);
  ns_link_t* l1 = ns_link_create();

  h0->dev.host->interface.write_link = l1;
  r1->dev.router->interfaces[0]->read_link = l1;

  fs_thread_create(&h0->thread.tid, NULL, test2_thread0,
                   (void*)h0->dev.host->interface.write_link);
  fs_thread_create(&r1->thread.tid, NULL, test2_thread1,
                   (void*)r1->dev.router->interfaces[0]->read_link);

  fs_thread_join(h0->thread.tid, NULL);
  fs_thread_join(r1->thread.tid, NULL);

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
