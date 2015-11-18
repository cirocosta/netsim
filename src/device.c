#include "netsim/device.h"

ns_device_t* ns_device_create(ns_device_e type)
{
  ns_device_t* device = malloc(sizeof(*device));
  PASSERT(device, NS_ERR_MALLOC);

  *device = ns_zeroed_device;

  switch (type) {
    case NS_DEVICE_ROUTER:
      ASSERT(0, "NOT SUPPORTED!");
      break;
    case NS_DEVICE_HOST:
      device->dev.host = ns_host_create();
      break;
  }

  return device;
}

void ns_device_destroy(ns_device_t* device)
{
  switch (device->type) {
    case NS_DEVICE_ROUTER:
      ASSERT(0, "NOT SUPPORTED!");
      break;
    case NS_DEVICE_HOST:
      ns_host_destroy(device->dev.host);
      device->dev.host = NULL;
      break;
  }

  free(device);
}
