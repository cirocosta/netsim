#ifndef NS__NET_H
#define NS__NET_H

#include "netsim/common.h"
#include <arpa/inet.h>

inline static uint32_t ns_pton(const char* src)
{
  uint32_t res;

  PASSERT(inet_pton(AF_INET, src, &res) == 1, "inet_pton (input=`%s`):", src);

  return ntohl(res);
}

inline static void ns_ntop(const uint32_t* ip, char* dst, unsigned n)
{
  PASSERT(inet_ntop(AF_INET, ip, dst, n) != NULL, "");
}


#endif
