#ifndef NS__CONTANTS_H
#define NS__CONTANTS_H 

#define NS_NAME_MAX 15
#define NS_ERR_MALLOC "Couldn't allocate memory"

typedef enum ns_app_e {
  NS_APP_DNSS = 1,
  NS_APP_IRCC,
  NS_APP_IRCS,
} ns_app_e;

typedef enum ns_device_e { 
  NS_DEVICE_ROUTER = 1,
  NS_DEVICE_HOST
} ns_device_e;

typedef enum ns_ev_e { 
  NS_EV_LINK = 1,
  NS_EV_TERM,
  NS_EV_TICK
} ns_ev_e;

typedef enum ns_transport_e { 
  NS_TRANSPORT_TCP = 1,
  NS_TRANSPORT_UDP
} ns_transport_e;

#define NS_NUMBER_TCP_PROTOCOL 6
#define NS_NUMBER_UDP_PROTOCOL 17

#endif
