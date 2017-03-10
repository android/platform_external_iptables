/* This is the userspace/kernel interface for Generic IP Chains,
   required for libc6. */
#ifndef _FWCHAINS_KERNEL_HEADERS_H
#define _FWCHAINS_KERNEL_HEADERS_H

#include <limits.h>

<<<<<<< HEAD   (b67f29 Revert "Add '-w' option to ip[6]tables-restore")
#if defined(__ANDROID__) || (defined(__GLIBC__) && __GLIBC__ == 2)
=======
>>>>>>> BRANCH (b013e3 iptables 1.6.1 release)
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/if.h>
#include <sys/types.h>
#endif
