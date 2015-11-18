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


#endif
