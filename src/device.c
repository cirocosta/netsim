#include "netsim/device.h"

ns_device_t* ns_device_create(ns_device_e type, unsigned interfaces_count)
{
  ns_device_t* device = malloc(sizeof(*device));
  PASSERT(device, NS_ERR_MALLOC);

  *device = ns_zeroed_device;
  device->type = type;

  switch (type) {
    case NS_DEVICE_ROUTER:
      device->dev.router = ns_router_create(interfaces_count);
      break;
    case NS_DEVICE_HOST:
      device->dev.host = ns_host_create();
      break;
  }

  device->epoll = ns_epoll_create(interfaces_count * 2 + 1);
  device->term_fd = eventfd(0, EFD_SEMAPHORE);
  device->tick_fd = eventfd(0, EFD_SEMAPHORE);

  ns_epoll_add_term_ev(device->epoll, device->term_fd);
  ns_epoll_add_tick_ev(device->epoll, device->tick_fd);

  return device;
}

void ns_device_destroy(ns_device_t* device)
{
  switch (device->type) {
    case NS_DEVICE_ROUTER:
      ns_router_destroy(device->dev.router);
      device->dev.router = NULL;
      break;
    case NS_DEVICE_HOST:
      ns_host_destroy(device->dev.host);
      device->dev.host = NULL;
      break;
  }

  ns_epoll_destroy(device->epoll);
  device->epoll = NULL;

  free(device);
}

void ns_device_run(ns_device_t* device)
{
  switch (device->type) {
    case NS_DEVICE_ROUTER:
      ns_thread_create(&device->thread, NULL, ns_router_func, (void*)device);
      break;
    case NS_DEVICE_HOST:
      ASSERT(0, "Not Supported!");
      break;
  }

  device->running = 1;
}

void ns_device_init_interface(ns_device_t* device, uint8_t interface_num,
                              ns_link_t* read_link, ns_link_t* write_link)
{
  ns_interface_t* interfaces = device->type == NS_DEVICE_HOST
                                   ? device->dev.host->interfaces
                                   : device->dev.router->interfaces;

  interfaces[interface_num].read_link = read_link;
  interfaces[interface_num].write_link = write_link;

  if (read_link) {
    ns_epoll_add_link_ev(device->epoll, interfaces[interface_num].read_link,
                         interfaces[interface_num].read_link->efd,
                         EPOLLIN | EPOLLET);
  }

  if (write_link) {
    ns_epoll_add_link_ev(device->epoll, interfaces[interface_num].write_link,
                         interfaces[interface_num].write_link->efd,
                         EPOLLOUT | EPOLLET);
  }
}

void ns_device_tick(ns_device_t* device)
{
  const static uint64_t u = 1UL;

  PASSERT(write(device->tick_fd, &u, sizeof(uint64_t)) == sizeof(uint64_t),
          "write: ");
}

void ns_device_terminate(ns_device_t* device)
{
  const static uint64_t u = 1UL;

  PASSERT(write(device->term_fd, &u, sizeof(uint64_t)) == sizeof(uint64_t),
          "write: ");
}
