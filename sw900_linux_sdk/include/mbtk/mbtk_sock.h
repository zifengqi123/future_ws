
/*
*
*
* Author : lb
* Date   : 2022/3/5 15:33:37
*
*/
#ifndef _MBTK_SOCK_H
#define _MBTK_SOCK_H
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

#include "mbtk_type.h"
#include "mbtk_sock2.h"


#define MBTK_BUFF_SIZE 2048

typedef struct
{
    union
    {
        struct sockaddr_in6 dst_sock_addr6;
        struct sockaddr_in serverSockAddr;
    }addr;
    uint16 ip_version;
    uint16 addr_len;
} mbtk_ipv4v6_addr;

typedef enum
{
    MBTK_ADDR_IPV4,
    MBTK_ADDR_IPV6,
    MBTK_ADDR_IPV4V6
} mbtk_addr_family_e;

typedef enum
{
    MBTK_SOCK_TCP_COM = 0,
    MBTK_SOCK_UDP_COM
} mbtk_sock_type_e;


typedef struct
{
    int16 net_id;
#ifdef MBTK_PLATFORM_QCOMM
    sint15 app6_id;
    sint15 task_handle;
    uint32 ip_filter;
    //ds_policy_info_type policy_info;    // No use
#endif
    bool keep_alive;
    uint32 recv_buff_size;
} mbtk_net_info_s;

typedef struct
{
    int fd;
    char host[50];
    int port;
    bool is_ssl;
    bool use_cert;
    mbtk_sock_type_e sock_type;
    mbtk_addr_family_e addr_family;
    mbtk_buffer_t read_buff;

    mbtk_ipv4v6_addr addr;
    uint16 temp;    // Must add (addr.addr_len should be uint32)
} mbtk_sock_info_s;

int sock_net_open(mbtk_net_info_s *net_info, mbtk_addr_family_e addr_family);

int sock_open(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, uint32 timeout, int *err);

int sock_addr_set(mbtk_net_info_s *net_info,mbtk_sock_info_s *sock_info,void *host, uint32 port);

int sock_bind(int sockfd, bool is_ipv6, uint16 port);

int sock_read(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, void *buffer, uint32 buf_len, uint32 timeout, int *err);

int sock_read_async(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, void *buffer, uint32 buf_len, uint32 timeout, int *err);

int sock_readline(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, void *buffer, uint32 buf_len, uint32 timeout, int *err);

int sock_write(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, const void *buffer, uint32 buf_len, uint32 timeout, int *err);

int sock_recv(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, void *buffer, uint32 buf_len, uint32 timeout);

int sock_sendto(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, const void *buffer, uint32 buf_len, uint32 timeout);

int sock_close(mbtk_net_info_s *net_info, mbtk_sock_info_s *sock_info, uint32 timeout, int *err);

int sock_net_close(mbtk_net_info_s *net_info, uint32 timeout, int *err);

#endif /* _MBTK_SOCK_H */
