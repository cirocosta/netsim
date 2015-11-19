#ifndef NS__QUEUE_H
#define NS__QUEUE_H

#include "netsim/common.h"
#include "netsim/ip.h"

typedef struct ns_queue_t {
  ns_ip_t** elems;
  unsigned f;
  unsigned r;
  size_t n;
  size_t length;
} ns_queue_t;

ns_queue_t* ns_queue_create(size_t n);
void ns_queue_destroy(ns_queue_t* q);
void ns_queue_remove(ns_queue_t* q);
unsigned ns_queue_insert(ns_queue_t* q, ns_ip_t* elem);

inline static int ns_queue_empty(ns_queue_t* q) { return q->f == q->r; }

inline static ns_ip_t* ns_queue_back(ns_queue_t* q)
{
  if (!ns_queue_empty(q))
    return q->elems[q->r];
  return NULL;
}
inline static ns_ip_t* ns_queue_front(ns_queue_t* q)
{
  if (!ns_queue_empty(q))
    return q->elems[q->f];
  return NULL;
}

#endif
