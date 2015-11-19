#include "netsim/simulator.h"

ns_simulator_t* ns_simulator_create(unsigned devices_count)
{
  ns_simulator_t* simulator = malloc(sizeof(*simulator));
  PASSERT(simulator, NS_ERR_MALLOC);

  return simulator;
}

void ns_simulator_destroy(ns_simulator_t* simulator)
{
  while (simulator->devices_count-- > 0)
    ns_device_destroy(simulator->devices[simulator->devices_count]);
  free(simulator);
}
