#include "netsim/ft.h"

/**
 * From the sample trace:
 * ----------------------
 * set route r0 10.0.0.0 0 10.1.1.0 1 192.168.3.0 2 \
 *              192.168.2.0 192.168.3.4 192.168.1.0 192.168.3.4
 * set route r1 192.168.3.0 0 192.168.2.0 1 192.168.1.0 2 \
 *              10.0.0.0 192.168.3.3 10.1.1.0 192.168.3.3
 *
 *  (10.0.0.1)
 *      h0 -------(10.0.0.2)---.
 *                           (0)\
 *                               r0(2)--(192.168.3.3)
 *                           (1)/
 *      h1 -------(10.1.1.2)---´
 *  (10.1.1.1)
 *
 *
 *
 *                                           (192.168.2.2)
 *                      .-----(192.168.2.3) ----- h2
 *                     /(1)
 * (192.168.3.4)--(0)r1
 *                     \(2)
 *                      `-----(192.168.1.2) ----- h3
 *                                           (192.168.1.1)
 */

void test1()
{
  ns_ft_t* ft = ns_ft_create(3);
  ns_interface_t i0 = { 0 };
  ns_interface_t i1 = { 0 };
  ns_interface_t i2 = { 0 };

  ns_ft_add_entry(ft, "10.0.0.0", 8, &i0);
  ns_ft_add_entry(ft, "10.1.1.0", 8, &i1);
  ns_ft_add_entry(ft, "198.162.3.0", 8, &i2);

  ASSERT(ns_ft_forward(ft, ns_pton("10.0.0.20")) != NULL, "");
  ASSERT(ns_ft_forward(ft, ns_pton("10.0.0.20")) == &i0, "");
  ASSERT(ns_ft_forward(ft, ns_pton("10.1.1.20")) == &i1, "");
  ASSERT(ns_ft_forward(ft, ns_pton("198.162.3.20")) == &i2, "");

  ns_ft_destroy(ft);
}

int main(int argc, char* argv[])
{
  TEST(test1, "performs basic forwarding based on ip masking correctly");

  return 0;
}
