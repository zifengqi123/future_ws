/*
* MBTK DHCP Header.
*
* Author : lb
* Date   : 2021/8/20 13:38:03
*
*/
#ifndef _MBTK_DHCP_H
#define _MBTK_DHCP_H
#include <netinet/in.h>

#include "mbtk_type.h"

#define PORT_BOOTP_SERVER 67
#define PORT_BOOTP_CLIENT 68
#define OP_BOOTREQUEST 1
#define OP_BOOTREPLY   2
#define FLAGS_BROADCAST 0x8000
#define HTYPE_ETHER    1
#define DHCP_MSG_FIXED_SIZE 236

/* first four bytes of options are a cookie to indicate that
** the payload are DHCP options as opposed to some other BOOTP
** extension.
*/
#define OPT_COOKIE1          0x63
#define OPT_COOKIE2          0x82
#define OPT_COOKIE3          0x53
#define OPT_COOKIE4          0x63

/* BOOTP/DHCP options - see RFC 2132 */
#define OPT_PAD              0

#define OPT_SUBNET_MASK      1     /* 4 <ipaddr> */
#define OPT_TIME_OFFSET      2     /* 4 <seconds> */
#define OPT_GATEWAY          3     /* 4*n <ipaddr> * n */
#define OPT_DNS              6     /* 4*n <ipaddr> * n */
#define OPT_DOMAIN_NAME      15    /* n <domainnamestring> */
#define OPT_BROADCAST_ADDR   28    /* 4 <ipaddr> */

#define OPT_REQUESTED_IP     50    /* 4 <ipaddr> */
#define OPT_LEASE_TIME       51    /* 4 <seconds> */
#define OPT_MESSAGE_TYPE     53    /* 1 <msgtype> */
#define OPT_SERVER_ID        54    /* 4 <ipaddr> */
#define OPT_PARAMETER_LIST   55    /* n <optcode> * n */
#define OPT_MESSAGE          56    /* n <errorstring> */
#define OPT_CLASS_ID         60    /* n <opaque> */
#define OPT_CLIENT_ID        61    /* n <opaque> */
#define OPT_END              255

/* DHCP message types */
#define DHCPDISCOVER         1
#define DHCPOFFER            2
#define DHCPREQUEST          3
#define DHCPDECLINE          4
#define DHCPACK              5
#define DHCPNAK              6
#define DHCPRELEASE          7
#define DHCPINFORM           8

typedef unsigned long long msecs_t;

typedef struct {
    uint8_t op;           /* BOOTREQUEST / BOOTREPLY    */
    uint8_t htype;        /* hw addr type               */
    uint8_t hlen;         /* hw addr len                */
    uint8_t hops;         /* client set to 0            */

    uint32_t xid;         /* transaction id             */

    uint16_t secs;        /* seconds since start of acq */
    uint16_t flags;

    uint32_t ciaddr;      /* client IP addr             */
    uint32_t yiaddr;      /* your (client) IP addr      */
    uint32_t siaddr;      /* ip addr of next server     */
                          /* (DHCPOFFER and DHCPACK)    */
    uint32_t giaddr;      /* relay agent IP addr        */

    uint8_t chaddr[16];  /* client hw addr             */
    char sname[64];      /* asciiz server hostname     */
    char file[128];      /* asciiz boot file name      */

    uint8_t options[1024];  /* optional parameters        */
} dhcp_msg;

typedef struct {
    uint32_t type;

    uint32_t ipaddr;
    uint32_t gateway;
    uint32_t prefixLength;

    uint32_t dns1;
    uint32_t dns2;

    uint32_t serveraddr;
    uint32_t lease;
} dhcp_info;

int mbtk_do_dhcp(const char *name);

#endif /* _MBTK_DHCP_H */

