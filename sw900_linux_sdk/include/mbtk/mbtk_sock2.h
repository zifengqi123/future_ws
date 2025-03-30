/*************************************************************
Description:
    mbtk_sock.h
    Used to define mobiletek sockets interfaces
Author:
    YangDagang
Date:
    2019-1-30
*************************************************************/
#ifndef __MBTK_SOCK2_H__
#define __MBTK_SOCK2_H__

#include "mbtk_type.h"
#include "mbtk_net_control.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MBTK_SOCK_EVENT_READ 0x01

/*************************************************************
    Constants and Macros
*************************************************************/
typedef int mbtk_sock_handle;
typedef int mbtk_sock_session;

/*************************************************************
    Definitions:enum,struct,union
*************************************************************/
typedef enum
{
    /*common error code (-99 ~ 0)*/
    MBTK_SOCK_SUCCESS = 0,
    MBTK_SOCK_ERROR = -1,
    MBTK_SOCK_EMEMORY = -2,
    MBTK_SOCK_EARG = -3, /* Invalid buffer or argument */
    MBTK_SOCK_EDNS = -4, /* DNS error */
    MBTK_SOCK_ETIMEOUT = -5, /* Timeout */
    /*socket error code */
    MBTK_SOCK_EEOF = 0, /* end of file */
    MBTK_SOCK_EBADF = 100, /* Invalid socket descriptor */
    MBTK_SOCK_EFAULT= 101,   /* Invalid buffer or argument */
    MBTK_SOCK_EWOULDBLOCK = 102, /* Operation would block */
    MBTK_SOCK_EAFNOSUPPORT = 103, /* Address family not supported */
    MBTK_SOCK_EPROTOTYPE = 104, /* Wrong protocol for socket type */
    MBTK_SOCK_ESOCKNOSUPPORT = 105, /* Socket parameter not supported */
    MBTK_SOCK_EPROTONOSUPPORT = 106, /* Socket parameter not supported */
    MBTK_SOCK_EMFILE = 107, /* No more sockets available for opening */
    MBTK_SOCK_EOPNOTSUPP = 108, /* Operation not supported */
    MBTK_SOCK_ETIMEDOUT = 116, /* Connection attempt timed out */
    MBTK_SOCK_ECONNRESET = 117, /* Connection reset */
    MBTK_SOCK_ECONNABORTED = 118, /* Connection aborted */
    MBTK_SOCK_EPIPE = 119,   /* Broken pipe */
    MBTK_SOCK_ENETDOWN = 120, /* Network subsystem unavailable */
    MBTK_SOCK_ENETNONET = 202, /* network subsystem unavailable */
    /*unkonw error code (-1000 ~ ...)*/
    MBTK_SOCK_EUNKONW = -1000     /*unkonw error*/
}mbtk_sock_errno;

typedef enum
{
    MBTK_SOCK_TCP = 0,
    MBTK_SOCK_UDP
} mbtk_sock_type;

typedef enum
{
    MBTK_NET_LINUX = 0, // Default for Linux socket.
    MBTK_NET_QUAL,      // Qcomm modem socket.
    MBTK_NET_MAX
} mbtk_net_type;

typedef struct {
    int state;
    char* addr;
    char* if_name;
} mbtk_net_cb_info_s;

typedef struct {
    mbtk_sock_session sock_fd;
    int event;
    mbtk_sock_type sock_type;
} mbtk_sock_cb_info_s;

typedef void (*mbtk_net_cb_func)(mbtk_sock_handle handle, mbtk_net_cb_info_s *net_info);
typedef void (*mbtk_sock_cb_func)(mbtk_sock_handle handle, mbtk_sock_cb_info_s *sock_info);

//Used to provide external and to transmit information
typedef struct
{
    mbtk_sock_type type; // socket type:TCP or UDP
    bool is_support_ssl; //is suppoert ssl
    bool ingnore_cert; //is ingnore certificate
    char address[256]; //server address
    int port; //server port
    char local_address[256]; //Local address
    int local_port; // Local port
    int ftp_ssl_support;
} mbtk_sock_info;

typedef struct
{
    char if_name[100];
    mbtk_net_type net_type;
    mbtk_net_cb_func net_cb;
    mbtk_sock_cb_func sock_cb;
} mbtk_init_info;

#ifdef MBTK_POLARSSL_SUPPORT
#define DFL_SERVER_NAME         "asr"
#define DFL_SERVER_ADDR         NULL
#define DFL_SERVER_PORT         4433
#define DFL_REQUEST_PAGE        "/"
#define DFL_REQUEST_SIZE        -1
#define DFL_DEBUG_LEVEL         0
#define DFL_NBIO                0
#define DFL_CA_FILE             "/ca.crt"
#define DFL_CA_PATH             "/ca.crt"
#define DFL_CRT_FILE            "/client.crt"
#define DFL_KEY_FILE            "/client.key"
#define DFL_PSK                 ""
#define DFL_PSK_IDENTITY        "Client_identity"
#define DFL_FORCE_CIPHER        0
#define DFL_RENEGOTIATION       SSL_RENEGOTIATION_DISABLED
#define DFL_ALLOW_LEGACY        SSL_LEGACY_NO_RENEGOTIATION
#define DFL_RENEGOTIATE         0
#define DFL_EXCHANGES           1
#define DFL_MIN_VERSION         SSL_MINOR_VERSION_3
#define DFL_MAX_VERSION         SSL_MINOR_VERSION_3
#define DFL_AUTH_MODE           SSL_VERIFY_REQUIRED
#define DFL_MFL_CODE            SSL_MAX_FRAG_LEN_NONE
#define DFL_TRUNC_HMAC          0
#define DFL_RECONNECT           0
#define DFL_RECO_DELAY          0
#define DFL_TICKETS             SSL_SESSION_TICKETS_ENABLED
#define DFL_ALPN_STRING         NULL
#endif

/*************************************************************
    Variables:local,extern
*************************************************************/

/*************************************************************
    Local Function Declaration
*************************************************************/

/*************************************************************
    Extern Function Declaration
*************************************************************/
extern mbtk_sock_handle mbtk_sock_init(mbtk_init_info *info);
extern mbtk_sock_session mbtk_sock_open(mbtk_sock_handle handle,mbtk_sock_info *info,
                unsigned int timeout,
                int *mbtk_errno);
extern int mbtk_sock_write(mbtk_sock_handle handle,mbtk_sock_session session,
                void *buffer,
                unsigned int buf_len,
                unsigned int timeout,
                int *mbtk_errno);
extern int mbtk_sock_read(mbtk_sock_handle handle,mbtk_sock_session session,
            void *buffer,
            unsigned int buf_len,
            unsigned int timeout,
            int *mbtk_errno);
extern int mbtk_sock_read_async(mbtk_sock_handle handle,mbtk_sock_session session,
            void *buffer,
            unsigned int buf_len);
extern int mbtk_sock_read_sync(mbtk_sock_handle handle,mbtk_sock_session session,
            void *buffer,
            unsigned int buf_len);
extern int mbtk_sock_close(mbtk_sock_handle handle,mbtk_sock_session session,
            unsigned int timeout,
            int *mbtk_errno);
extern int mbtk_sock_deinit(mbtk_sock_handle handle);

extern int mbtk_ssl_init_func(mbtk_sock_handle handle ,bool ingnore_cert,mbtk_sock_session fd);
extern int mbtk_ssl_close_func(mbtk_sock_handle handle ,bool ingnore_cert,mbtk_sock_session fd);
extern int mbtk_sock_readline(mbtk_sock_handle handle,mbtk_sock_session session,
            void *buffer,
            unsigned int buf_len,
            unsigned int timeout,
            int *mbtk_errno,
            int *read_line_count,
            char *buf_ptr);
/*
* Get TCP RECV buffer data length.
*/
int mbtk_sock_tcp_recv_len_get(mbtk_sock_handle handle,mbtk_sock_session session);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __MBTK_SOCK2_H__ */
