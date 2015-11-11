# netsim

> Network Simulator

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents** 

- [Protocols](#protocols)
- [Inside](#inside)
  - [`host_t`](#host_t)
  - [`router_t`](#router_t)
  - [`interface_t`](#interface_t)
  - [`addr_t`](#addr_t)
  - [`ft_t`](#ft_t)
  - [`event_t`](#event_t)
  - [`sniffer_t`](#sniffer_t)
  - [Forwarding](#forwarding)
- [Usage](#usage)
  - [Commands](#commands)
  - [Example Trace](#example-trace)
- [Output](#output)
  - [IP](#ip)
  - [TCP](#tcp)
  - [UDP](#udp)
  - [DNS](#dns)
  - [IRC](#irc)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Protocols

- Network: IP
- Transport: UDP and TCP
- Application: IRC and DNS

## Inside

IP mask: `255.255.255.0` (0xffffff00)

### `host_t`

```c
enum application_e {
  IRCC,
  IRCS,
  DNSS
};


struct host_t { 
  char name[NS_NAME_MAX];
  interface_t interface;
  addr_t router_addr;
  addr_t dns_addr;
  application_e app;
};
```

### `router_t`

```c
struct router_t { 
  char name[NS_NAME_MAX];
  interface_t** interfaces;
  uint8_t interfaces_count;

  ft_t* forwarding_table;
};
```

### `interface_t` 

```c
struct interface_t { 
  addr_t addr;
  uint8_t band;               // Mbps
  uint16_t latency;           // ms
};
```

### `addr_t`

Address. Encompasses presentation and numeric form.

```
struct addr_t {
  char p_ip[16];              // presentation ip (255.255.255.255)
  uint32_t n_ip;              // numeric ip      (   0xffffffff  )
};
```

### `ft_t`

Router's Forwarding Table

```c
struct ft_t {
  TODO 
};
```

### `event_t`

Event that gets into the system (a client performs):

```c
struct event_t {
  uint32_t ms;
  host_t* host;
  char command[NS_COMMAND_SIZE];
};
```

### `sniffer_t`

```c
struct sniffer_t {
  FILE* log_file;
  interface_t* interfaces[2];
};
```

### Forwarding

```
(iforwarding_table, addr) => interface

ip_mask : interface   -+-
ip_mask : interface    |
ip_mask : interface    |  forwarding to router interfaces by masking.
ip_mask : interface    |
ip_mask : interface   -+-

ip: router address ==> interface  -+-
ip: router address ==> interface   |  out-the-network forwarding.
ip: router address ==> interface   |  given the router addr we know the interface to
ip: router address ==> interface   |  send the packet to.
ip: router address ==> interface  -+-

  (ps.: at parsing time we're capable of resolving ip router `(address => interface)`
        and so we only really need an interface forwading table
```

We know that prefix size will always be 24b (as we're dealing with class C addresses). So, every prefix matching will take form of:

```c
if (MASK & (addr.n_ip >> 8)) {
  // do something
}
```

The forwarding table is given per-router (and so, it's not shared and different for each of them). They are constructed at parsing time.

## Usage

### Commands

``` 
- host <hname>
- router <rname> <# interfaces>
- duplex-link <hname | rname.int> <hname | router.int> <band> <latency>
- ip (<hname> <host_ip> <router_ip> <dns_server_ip> ) |
     (<hname> <interface> <ip> [[<interface> <ip>] ...] )
- route <rname> [[<ip> <interface>] ... ] <ip> [[<ip> ...] <interface>
- performance <rname> <packet_process_time> [[<interface> <bufsize>] ...]
- (ircc|ircs|dnss) <hname> <app_name>
- sniffer <hname | rname.int>  <hname | rname.int> "<file_name>"
- simulate <timestamp> <name> "<command>"
``` 

### Example Trace

```
# Creation of 4 hosts, each having a single interface

set host h0
set host h1
set host h2
set host h3


# Creation of 2 routers with 3 interfaces each

set router r0 3
set router r1 3

# link creation
#
#  (10.0.0.1)                
#      h0 -------(10.0.0.2)---.             
#                           (0)\          
#                               r0(2)--(192.168.3.3)
#                           (1)/            
#      h1 -------(10.1.1.2)---´             
#  (10.1.1.1)          
#
#
# 
#                                           (192.168.2.2)
#                      .-----(192.168.2.3) ----- h2
#                     /(1)
# (192.168.3.4)--(0)r1
#                     \(2)
#                      `-----(192.168.1.2) ----- h3
#                                           (192.168.1.1)

# set duplex-link device router.port bandwidth latency

set duplex-link h0 r0.0 10Mbps 10ms
set duplex-link h1 r0.1 5Mbps 2ms
set duplex-link r0.2 r1.0 5Mbps 2ms
set duplex-link r1.1 h2 2Mbps 20ms
set duplex-link r1.2 h3 5Mbps 2ms

# hosts configuration
# set ip <name> <host_ip> <router_ip> <dns_server_ip>
set ip h0 10.0.0.1      10.0.0.2    192.168.1.1
set ip h1 10.1.1.1      10.0.0.2    192.168.1.1
set ip h2 192.168.2.2   198.168.2.3 192.168.1.1

# 1.1.1.1 corresponds to the root DNS server that all
# servers need to be configured for as their root DNS 
# server.
set ip h3 192.168.1.1   198.168.2.3 1.1.1.1

# router configuration
# set ip <r_name> port, ip [[port, ip] ... ]
set ip r0 0 10.0.0.2        1 10.1.1.2      2 192.168.3.3
set ip r1 0 192.168.3.4     1 192.168.2.3   2 192.168.1.2

# routing configuration
# note: all networks are Class-C
set route r0 10.0.0.0 0 10.1.1.0 1 192.168.3.0 2 \
             192.168.2.0 192.168.3.4 192.168.1.0 192.168.3.4
set route r1 192.168.3.0 0 192.168.2.0 1 192.168.1.0 2 
             10.0.0.0 192.168.3.3 10.1.1.0 192.168.3.3

# routers processing time configuration
# <name> <time_to_process a packet> [[<port> <bufsize> ] ... ]
set performance r0 100us 0 1000 1 1000 2 1000
set performance r1 20us 0 1000 1 1000 2 1000

# Application Specification
set ircc h0 irc1
set ircc h1 irc2
set ircs h2 irc3
set dnss h3 dns1

# sniffer configuration
set sniffer r0.2 r1.0 "/tmp/sniffer1"
set sniffer h1 r0.1 "/tmp/sniffer2"

simulate 0.5 irc1 "CONNECT h2 6667"
simulate 0.6 irc2 "CONNECT 192.168.2.2 6667"
simulate 2.0 irc1 "USER usuario1"
simulate 4.0 irc2 "USER usuario2"
simulate 4.1 irc1 "QUIT"
simulate 5.0 irc2 "QUIT"
finish 5.0
```

## Output

- packet identifier
- timestamp 
- sniffer identification

### IP

- source IP
- destination IP
- identification of upper layer protocol (following RFC spec)
- IP header size + payload size
- TTL

### TCP

- source port
- destination port
- sequency number
- acklowledgement number (in case ACK is up)
- ACK bit
- FIN bit
- SYN bit

- Consider that the MSS is always equal to 1460 bytes.
- There's no need to implement congestion control and control flow mechanisms.

### UDP

- source port
- destination port
- UDP header size + payload from upper layer

### DNS

Just needs to return an IP address for a given name passed to it.

### IRC

CONNECT
USER
QUIT

