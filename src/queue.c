#include "netsim/queue.h"

ns_queue_t* ns_queue_create(size_t n)
{
  ns_queue_t* queue = malloc(sizeof(*queue));
  PASSERT(queue, NS_ERR_MALLOC);

  queue->f = 0;
  queue->r = 0;
  queue->length = 0;
  queue->n = n;
  queue->elems = calloc(queue->n, sizeof(*queue->elems));
  PASSERT(queue->elems, NS_ERR_MALLOC);

  return queue;
}

void ns_queue_destroy(ns_queue_t* q)
{
  free(q->elems);
  q->elems = NULL;
  free(q);
}

unsigned ns_queue_insert(ns_queue_t* q, ns_ip_t* elem)
{
  unsigned pos = q->r;

  q->elems[q->r] = elem;

  if (q->r == q->n - 1)
    q->r = 0;
  else
    q->r++;

  q->length++;

  return pos;
}

void ns_queue_remove(ns_queue_t* q)
{
  if (q->f == q->n - 1)
    q->f = 0;
  else
    q->f++;

  q->length--;
}
