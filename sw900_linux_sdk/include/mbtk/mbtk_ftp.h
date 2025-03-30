/*************************************************************
 Description:
 MBTK FTP Protocol Head File.
 Author:
 LiuBin
 Date:
 2020/10/28 11:49:19
 *************************************************************/
#ifndef _MBTK_FTP_H
#define _MBTK_FTP_H

#include "mbtk_file.h"
#include "mbtk_sock.h"
/*************************************************************
 Constants and Macros
 *************************************************************/
#define MBTK_FTP_IP_MAX 50
#define MBTK_FTP_FILE_NAME_MAX 50
#define FTP_SERVICE_PORT_DEF 21
#define FTP_SERVER_USER_NAME_MAX 30
#define FTP_SERVER_USER_PASS_MAX FTP_SERVER_USER_NAME_MAX

/*************************************************************
 Definitions:enum,struct,union,class
 *************************************************************/
typedef enum
{
    FTP_STATE_NON,
    FTP_STATE_CONNECTING,
    FTP_STATE_CONNECTED, /* FTP connected. */
    FTP_STATE_READY, /* FTP logn in success. */
    FTP_STATE_CMD_PROCESS
} mbtk_ftp_state_enum;

typedef enum
{
    FTP_CMD_CONN, FTP_CMD_PORT,       // Active mode(IPV4).
    FTP_CMD_EPRT,       // Active mode(IPV6).
    FTP_CMD_PASV,       // Passive mode(IPV4).
    FTP_CMD_EPSV,       // Passive mode(IPV6).
    FTP_CMD_LIST,       // List files in current directory.
    FTP_CMD_NLST,       // List files in current directory.
    FTP_CMD_PWD,        // Get current directory.
    FTP_CMD_CWD,        // Set current directory.
    FTP_CMD_MKD,        // Create directory.
    FTP_CMD_RMD,        // Remove directory.
    FTP_CMD_STAT,       // Get service status.
    FTP_CMD_TYPE,       // Set data type[A-Ascii,E-EBCDIC,I-Binary].
    FTP_CMD_SIZE,       // Get file size.
    FTP_CMD_DELE,       // Delete file.
    FTP_CMD_MDTM,       // Get file modify time.
    FTP_CMD_PUT,       // Update file.
    FTP_CMD_GET,       // Download file.
    FTP_CMD_QUIT
} mbtk_ftp_cmd_enum;

typedef enum
{
    FTP_ERR_SUCCESS,
    FTP_ERR_PARAM_SET,
    FTP_ERR_CLI_FULL,
    FTP_ERR_NET_CONN,
    FTP_ERR_LOGIN_DENIED,
    FTP_ERR_NET_READ,
    FTP_ERR_NET_WRITE,
    FTP_ERR_NET_CLOSE,
    FTP_ERR_NET_TIMEOUT,
    FTP_ERR_FILE_NO_FOUND,
    FTP_ERR_FILE_EXIST,
    FTP_ERR_DATA_SOCK_BUSY,
    FTP_ERR_EFS_FILE,
    FTP_ERR_UNKNOWN
} mbtk_ftp_error_enum;

typedef enum
{
    FTP_MODE_PASSIVE, FTP_MODE_ACTION
} mbtk_ftp_mode_enum;

typedef enum
{
    FTP_DATA_TYPE_A,    // Ascii
    FTP_DATA_TYPE_E,    // EBCDIC
    FTP_DATA_TYPE_I     // Binary
} mbtk_ftp_data_type_enum;

typedef enum
{
    FTP_SOCK_CTRL = 0,
    FTP_SOCK_DATA,
    FTP_SOCK_MAX
} mbtk_ftp_sock_type_enum;

typedef int mbtk_ftp_handle;
typedef void (*mbtk_data_cb_func)(void *data, uint32 data_len);

typedef struct
{
    sint16 fd;
    uint8 host[MBTK_FTP_IP_MAX + 1];
    uint16 port;
} mbtk_ftp_sock_s;

typedef struct
{
    int fd;
    uint8 remote_name[MBTK_FTP_FILE_NAME_MAX * 2 + 1];
    uint8 local_name[MBTK_FTP_FILE_NAME_MAX * 2 + 1];
    uint8 modify_time[20];
    mbtk_data_cb_func data_cb;
    bool is_download;
    uint32 size_count;
    uint32 size_send;
} mbtk_ftp_file_trans_info_s;

typedef struct
{
    uint8 name[FTP_SERVER_USER_NAME_MAX + 1];
    uint8 pass[FTP_SERVER_USER_PASS_MAX + 1];
} mbtk_ftp_user_info_s;

typedef enum
{
    FTP_AUTH_TYPE_NON,
    FTP_AUTH_TYPE_IMPLICIT,
    FTP_AUTH_TYPE_EXPLICIT_SSL,
    FTP_AUTH_TYPE_EXPLICIT_TLS
} mbtk_ftp_auth_type_enum;

typedef struct
{
    mbtk_ftp_handle handle; // 1 - 5
    mbtk_ftp_auth_type_enum auth_type;
//    bool ssl_enable;
//    bool is_ipv6;
//    bool use_cert;
    bool is_trans;      // Is download or update.
    bool is_data_sock_busy;      // Data socket is busy.
    //bool should_reconn; // Should reconnect.

    mbtk_ftp_user_info_s user;
    mbtk_ftp_state_enum state;
    //mbtk_ftp_sock_s ctrl_sock;
    mbtk_ftp_file_trans_info_s file_trans;
    //sint16 data_sock_fd;
    mbtk_ftp_mode_enum data_mode;
    mbtk_net_info_s net_info;
    mbtk_sock_info_s sock_info[FTP_SOCK_MAX];
    mbtk_init_info ftp_ssl_info;
    mbtk_init_info ftp_ssl_info_data;
    mbtk_sock_handle ftp_ssl_handle;
    mbtk_sock_handle ftp_ssl_handle_data;
    mbtk_sock_info *ftp_sock_ssl_info;
    mbtk_sock_info *ftp_sock_ssl_info_data;
    mbtk_sock_session session;
    mbtk_sock_session session_data;

} mbtk_ftp_info_s;

typedef struct mbtk_ftp_dir_info
{
    uint8 name[MBTK_FTP_FILE_NAME_MAX + 1];
    bool is_file;
    uint32 size;    // Directory is 0

    struct mbtk_ftp_dir_info *next;
    void (*ftp_ls_cb_typedef)(void *file_list_cb);
} mbtk_ftp_file_info_s;

/*************************************************************
 Extern variables
 *************************************************************/
mbtk_ftp_handle mbtk_ftp_init(const void* host, uint16 port, mbtk_ftp_auth_type_enum auth_type,
                              bool is_ipv6, bool use_cert);
mbtk_ftp_error_enum mbtk_ftp_reconfig(mbtk_ftp_handle handle,const void* host, uint16 port, mbtk_ftp_auth_type_enum auth_type,
                                      bool is_ipv6, bool use_cert);
mbtk_ftp_error_enum mbtk_ftp_deinit(mbtk_ftp_handle handle);
mbtk_ftp_error_enum mbtk_ftp_quit(mbtk_ftp_handle handle);
mbtk_ftp_info_s* mbtk_ftp_info_get(mbtk_ftp_handle handle);
mbtk_ftp_error_enum mbtk_ftp_login(mbtk_ftp_handle handle, void *user,
                                   void *pass);
mbtk_ftp_error_enum mbtk_ftp_net_close(mbtk_ftp_handle handle);
mbtk_ftp_error_enum mbtk_ftp_pwd(mbtk_ftp_handle handle, void *path);
mbtk_ftp_error_enum mbtk_ftp_cd(mbtk_ftp_handle handle, void *path);
mbtk_ftp_error_enum mbtk_ftp_dir_ls(mbtk_ftp_handle handle,
                                    mbtk_ftp_file_info_s *list_head);
uint32 mbtk_ftp_file_size(mbtk_ftp_handle handle, void *path);
mbtk_ftp_error_enum mbtk_ftp_file_del(mbtk_ftp_handle handle, void *path);
mbtk_ftp_error_enum mbtk_ftp_dir_mkdir(mbtk_ftp_handle handle, void *path);
mbtk_ftp_error_enum mbtk_ftp_dir_rmdir(mbtk_ftp_handle handle, void *path);
mbtk_ftp_error_enum mbtk_ftp_mode_set(mbtk_ftp_handle handle,
                                      mbtk_ftp_mode_enum mode);
mbtk_ftp_error_enum mbtk_ftp_data_type_set(mbtk_ftp_handle handle,
        mbtk_ftp_data_type_enum data_type);
uint32 mbtk_ftp_download_start(mbtk_ftp_handle handle, void *remote_path,
                               void *local_path, mbtk_data_cb_func data_cb);
uint32 mbtk_ftp_download_continue(mbtk_ftp_handle handle);
int mbtk_ftp_upload_start(mbtk_ftp_handle handle, const void *remote_path,
                          const void *local_path, uint32 size_byte);
int mbtk_ftp_upload_send(mbtk_ftp_handle handle, const void *data,uint16 data_len);
mbtk_ftp_error_enum mbtk_ftp_trans_reset(mbtk_ftp_handle handle);
mbtk_ftp_handle mbtk_ftp_upload_end(mbtk_ftp_handle handle);

#ifdef MBTK_PLATFORM_QCOMM
int mbtk_ftp_task_handle_set(mbtk_ftp_handle handle,sint15 task_handle);
mbtk_ftp_error_enum mbtk_ftp_get_ip_and_port(char *ipBuf_out,
                                    int *port,int iptype);

#endif

/*************************************************************
 Public Function Declaration
 *************************************************************/

#endif /* _MBTK_FTP_H */

