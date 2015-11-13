#include "netsim/link.h"
#include "netsim/interface.h"
#include "netsim/common.h"

void test1()
{
  ns_link_t l1;
  ns_ip_t packet;
  
  ns_link_send(&l1, &packet);

  ASSERT(ns_link_recv(&l1) == &packet, "");
}

int main(int argc, char *argv[])
{

  TEST(test1, "Single-thread link send and recv");
  
  return 0;
}
