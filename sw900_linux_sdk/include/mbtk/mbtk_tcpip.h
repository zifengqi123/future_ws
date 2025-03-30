/*
    TCP/IP AT header.
*/
/******************************************************************************

                          EDIT HISTORY FOR FILE

  WHEN        WHO       WHAT,WHERE,WHY
--------    --------    -------------------------------------------------------
2023/9/24      lb    Initial version

******************************************************************************/
#ifndef _MBTK_TCPIP_H
#define _MBTK_TCPIP_H
#include "mbtk_type.h"
#include "mbtk_sock2.h"

#define MBTK_TCPIP_LINK_MAX 4

typedef enum {
    MBTK_TCPIP_ERR_SUCCESS,
    MBTK_TCPIP_ERR_NET_UNAVAILABLE,     /* Network unavailable. */
    MBTK_TCPIP_ERR_NET_HANDLE,     /* Network handle error. */
    MBTK_TCPIP_ERR_ARG,     /* Parameter error. */
    MBTK_TCPIP_ERR_LINK_UNAVAILABLE,     /* Link unavailable. */
    MBTK_TCPIP_ERR_LINK_NOT_CONNECT,     /* Link not connect. */




    MBTK_TCPIP_ERR_UNKNOWN
} mbtk_tcpip_err_enum;

typedef enum {
    MBTK_TCPIP_TYPE_CLIENT,
    MBTK_TCPIP_TYPE_SERVER
} mbtk_tcpip_type_enum;

typedef void (*mbtk_tcpip_net_callback_func)(int state, const char* addr);
typedef void (*mbtk_tcpip_sock_callback_func)(int link_id, int state);
typedef void (*mbtk_tcpip_read_callback_func)(int link_id, const char* data, int data_len);

typedef struct {
    int link_id;
    char ser_addr[256];
    int ser_port;
    mbtk_sock_type prot_type; // TCP/UDP
    mbtk_tcpip_type_enum tcpip_type; // Only support client.
    int local_port;
    bool ack_support;
    bool ssl_support;
    bool ignore_cert;
    uint32 heartbeat_time;
    uint32 delay_time;

    mbtk_tcpip_read_callback_func read_cb;
} mbtk_tcpip_info_t;

typedef struct {
    int link_id;
    int sock_fd;
    int state;
    int recv_data_len;
} mbtk_tcpip_tcp_state_info_s;

mbtk_tcpip_err_enum mbtk_tcpip_net_open(mbtk_tcpip_net_callback_func net_cb, mbtk_tcpip_sock_callback_func sock_cb);
mbtk_tcpip_err_enum mbtk_tcpip_net_close();
mbtk_tcpip_err_enum mbtk_tcpip_sock_open(const mbtk_tcpip_info_t *tcpip_info);
mbtk_tcpip_err_enum mbtk_tcpip_sock_close(int link_id);

int mbtk_tcpip_send(int link_id, const char* data, int data_len, const char* ser_addr, int ser_port);
int mbtk_tcpip_read(int link_id, char* buff, int buff_size);

/*
* Get the data traffic of the specified link. Return -1 if fail.
*/
int mbtk_tcpip_data_traffic_get(int link_id);

/*
* Reset the data traffic of the specified link.
*/
mbtk_tcpip_err_enum mbtk_tcpip_data_traffic_reset(int link_id);

/*
* Return 0 if disconnected, 1 for connected, other for fail.
*/
int mbtk_tcpip_link_state_get(int link_id);

/*
* Get TCP state informations.
*/
int mbtk_tcpip_info_get(int link_id, mbtk_tcpip_tcp_state_info_s *state_info);

/*
* Set socket auto read callback function,NULL for close auto read.
*/
void mbtk_tcpip_set_read_cb(int link_id, mbtk_tcpip_read_callback_func read_cb);

#endif /* _MBTK_TCPIP_H */
