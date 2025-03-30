/*************************************************************
Description:
    MBTK HTTP Header file.
Author:
    LiuBin
Date:
    2020/4/29 17:25:55
*************************************************************/
#ifndef _MBTK_HTTP_2_H
#define _MBTK_HTTP_2_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "mbtk_type.h"
#include "mbtk_sock2.h"


/*************************************************************
    Constants and Macros
*************************************************************/
#define MBTK_HTTP_URI_MAX 512
#define MBTK_HTTP_HOST_MAX 50
#define MBTK_HTTP_URL_MAX (MBTK_HTTP_URI_MAX + MBTK_HTTP_HOST_MAX)
#define MBTK_HTTP_PORT_DEF 80
#define MBTK_HTTPS_PORT_DEF 443
#define HTTP_HANDLE_MAX 3
#define HTTP_SESSION_MAX 5
#define HTTP_REQUEST_HEADER_MAX 30
#define HTTP_RESPONSE_HEADER_MAX 30
#define HTTP_CONTENT_LEN_MAX 1024
#define HTTP_MSG_LEN_MAX 1024

#define HTTP_IPC_MSG_PATH "/dev"
#define HTTP_IPC_MSG_ID 'H'

/*************************************************************
    Definitions:enum,struct,union,class
*************************************************************/
typedef enum {
    HTTP_VERSION_1_0 = 0,
    HTTP_VERSION_1_1,
    HTTP_VERSION_2,
    HTTP_VERSION_3
} mbtk_http_version_enum;

typedef enum {
    HTTP_OPTION_HEAD = 0,
    HTTP_OPTION_GET,
    HTTP_OPTION_POST,
    HTTP_OPTION_PUT,
    HTTP_OPTION_DELETE,
    HTTP_OPTION_OPTIONS,
    HTTP_OPTION_TRACE,
    HTTP_OPTION_CONNECT,
    HTTP_OPTION_LINK,
    HTTP_OPTION_UNLINK
} mbtk_http_option_enum;

#if 0
typedef enum {
    HTTP_HEADER_GENERAL = 0,
    HTTP_HEADER_REQUEST,
    HTTP_HEADER_RESPONSE,
    HTTP_HEADER_ENTITY,
    HTTP_HEADER_OTHERS
} mbtk_http_header_type_e;

typedef enum {
    // General field start.
    HTTP_HEADER_INDEX_CACHE_CONTROL = 0,
    HTTP_HEADER_INDEX_CONNECTION,
    HTTP_HEADER_INDEX_DATE,
    HTTP_HEADER_INDEX_PRAGMA,
    HTTP_HEADER_INDEX_TRAILER,
    HTTP_HEADER_INDEX_TRANSFER_ENCODING,
    HTTP_HEADER_INDEX_UPGRADE,
    HTTP_HEADER_INDEX_VIA,
    HTTP_HEADER_INDEX_WARNING,
    // General field end.
    // Request field start.
    HTTP_HEADER_INDEX_ACCEPT,
    HTTP_HEADER_INDEX_ACCEPT_CHARSET,
    HTTP_HEADER_INDEX_ACCEPT_ENCODING,
    HTTP_HEADER_INDEX_ACCEPT_LANGUAGE,
    HTTP_HEADER_INDEX_AUTHORIZATION,
    HTTP_HEADER_INDEX_EXPECT,
    HTTP_HEADER_INDEX_FROM,
    HTTP_HEADER_INDEX_HOST,
    HTTP_HEADER_INDEX_IF_MATCH,
    HTTP_HEADER_INDEX_IF_MODIFIED_SINCE,
    HTTP_HEADER_INDEX_IF_NONE_MATCH,
    HTTP_HEADER_INDEX_IF_RANGE,
    HTTP_HEADER_INDEX_IF_UNMODIFIED_SINCE,
    HTTP_HEADER_INDEX_MAX_FORWARDS,
    HTTP_HEADER_INDEX_PROXY_AUTHORIZATION,
    HTTP_HEADER_INDEX_RANGE,
    HTTP_HEADER_INDEX_REFERER,
    HTTP_HEADER_INDEX_TE,
    HTTP_HEADER_INDEX_USER_AGENT,
    // Request field end.
    // Response field start.
    HTTP_HEADER_INDEX_ACCEPT_RANGES,
    HTTP_HEADER_INDEX_AGE,
    HTTP_HEADER_INDEX_ETAG,
    HTTP_HEADER_INDEX_LOCATION,
    HTTP_HEADER_INDEX_PROXY_AUTHENTICATE,
    HTTP_HEADER_INDEX_RETRY_AFTER,
    HTTP_HEADER_INDEX_SERVER,
    HTTP_HEADER_INDEX_VARY,
    HTTP_HEADER_INDEX_WWW_AUTHENTICATE,
    // Response field end.
    // Entity field start.
    HTTP_HEADER_INDEX_ALLOW,
    HTTP_HEADER_INDEX_CONTENT_ENCODING,
    HTTP_HEADER_INDEX_CONTENT_LANGUAGE,
    HTTP_HEADER_INDEX_CONTENT_LENGTH,
    HTTP_HEADER_INDEX_CONTENT_LOCATION,
    HTTP_HEADER_INDEX_CONTENT_MD5,
    HTTP_HEADER_INDEX_CONTENT_RANGE,
    HTTP_HEADER_INDEX_CONTENT_TYPE,
    HTTP_HEADER_INDEX_EXPIRES,
    HTTP_HEADER_INDEX_LAST_MODIFIED,
    // Entity field end.
    // Other field start.
    HTTP_HEADER_INDEX_SET_COOKIE,
    HTTP_HEADER_INDEX_COOKIE,
    HTTP_HEADER_INDEX_X_FRAME_OPTIONS,
    HTTP_HEADER_INDEX_X_XSS_PROTECTION,
    HTTP_HEADER_INDEX_DNT,
    HTTP_HEADER_INDEX_P3P
    // Other field end.
} mbtk_http_header_index_e;
#endif

typedef enum {
    HTTP_SESSION_STATE_NON = 0,
    HTTP_SESSION_STATE_CONN,
    HTTP_SESSION_STATE_WRITE_HEADER,
    HTTP_SESSION_STATE_WRITE_CONTENT,
    HTTP_SESSION_STATE_WRITE_END,
    HTTP_SESSION_STATE_READ_HEADER,
    HTTP_SESSION_STATE_READ_CONTENT,
    HTTP_SESSION_STATE_READ_END
} http_session_state_e;

typedef enum {
    MBTK_HTTP_STATE_CODE_200 = 200,
    MBTK_HTTP_STATE_CODE_204 = 204,
    MBTK_HTTP_STATE_CODE_206 = 206,
    MBTK_HTTP_STATE_CODE_301 = 301,
    MBTK_HTTP_STATE_CODE_302 = 302,
    MBTK_HTTP_STATE_CODE_303 = 303,
    MBTK_HTTP_STATE_CODE_304 = 304,
    MBTK_HTTP_STATE_CODE_307 = 307,
    MBTK_HTTP_STATE_CODE_400 = 400,
    MBTK_HTTP_STATE_CODE_401 = 401,
    MBTK_HTTP_STATE_CODE_403 = 403,
    MBTK_HTTP_STATE_CODE_404 = 404,
    MBTK_HTTP_STATE_CODE_500 = 500,
    MBTK_HTTP_STATE_CODE_503 = 503
} mbtk_http_state_code_e;

typedef enum {
    MBTK_HTTP_DATA_NON = 0,
    MBTK_HTTP_DATA_HEADER,
    MBTK_HTTP_DATA_CONTENT,
    MBTK_HTTP_DATA_COMPLETE
} mbtk_http_data_type_enum;

typedef void (*mbtk_http_data_callback_func)(
    int session_id, mbtk_http_data_type_enum type,
    void *data,int data_len);

typedef struct mbtk_http_header {
    char name[30];
    char *value;
} mbtk_http_header_t;

typedef struct {
    int header_cnt;
    mbtk_http_header_t *req_h[HTTP_REQUEST_HEADER_MAX];

    int content_len;    // Post content lenght
    int content_len_send; // Post content lenght for send.
    char *content;
} mbtk_http_session_req_t;

typedef struct {
    int state_code;
    mbtk_http_version_enum version;

    int content_length;
    bool is_chunked;
    int header_cnt;
    mbtk_http_header_t *rsp_h[HTTP_RESPONSE_HEADER_MAX];
} mbtk_http_session_rsp_t;

typedef struct mbtk_http_session{
    int handle_id;
    int id;
    int sock_fd;
    int file_fd;
    http_session_state_e state;
    char host[MBTK_HTTP_HOST_MAX + 1];
    mbtk_http_option_enum option;
    mbtk_http_version_enum version;
    char uri[MBTK_HTTP_URI_MAX + 1];
    int port;
    bool is_ssl;
    bool ingnore_cert;  //FALSE:need cert

    mbtk_http_session_req_t req;
    mbtk_http_session_rsp_t rsp;
} mbtk_http_session_t;

typedef struct mbtk_http_handle{
    int id;
    bool show_rsp_header;
    mbtk_http_data_callback_func data_cb;

    int session_cnt;
    mbtk_http_session_t *session[HTTP_SESSION_MAX];
} mbtk_http_handle_t;

#if 0
typedef enum {
    HTTP_MSG_SESSION_CREATE_REQ,
    HTTP_MSG_SESSION_CREATE_RSP,
    HTTP_MSG_SESSION_FREE_REQ,
    HTTP_MSG_SESSION_FREE_RSP,
    HTTP_MSG_SESSION_URL_SET_REQ,
    HTTP_MSG_SESSION_URL_SET_RSP,
    HTTP_MSG_SESSION_HEAD_SET_REQ,
    HTTP_MSG_SESSION_HEAD_SET_RSP,
    HTTP_MSG_SESSION_CONTENT_SET_REQ,
    HTTP_MSG_SESSION_CONTENT_SET_RSP,
    HTTP_MSG_SESSION_ACTION_REQ,
    HTTP_MSG_SESSION_ACTION_RSP,
    HTTP_MSG_SESSION_CONTENT_SEND_REQ,
    HTTP_MSG_SESSION_CONTENT_SEND_RSP
} mbtk_http_msg_type_enum;
#endif

typedef enum {
    HTTP_MSG_SESSION_CREATE,
    HTTP_MSG_SESSION_FREE,
    HTTP_MSG_SESSION_HEAD_SET,
    HTTP_MSG_SESSION_CONTENT_SET,
    HTTP_MSG_SESSION_ACTION,
    HTTP_MSG_SESSION_CONTENT_SEND,
    HTTP_MSG_HANDLE_EXIT
} mbtk_http_msg_type_enum;

typedef enum {
    MBTK_HTTP_ERR_SUCCESS,
    MBTK_HTTP_ERR_UNKNOWN
} mbtk_http_err_enum;

typedef struct {
    mbtk_http_msg_type_enum type;
    int int_arg1;
    int int_arg2;
    char str_small[30];
    char str_large[HTTP_MSG_LEN_MAX];
} mbtk_http_msg_req_t;

typedef struct {
    mbtk_http_msg_type_enum type;
    mbtk_http_err_enum err;

    mbtk_http_data_type_enum data_type;
    int buff_len;
    char buff[HTTP_MSG_LEN_MAX];
} mbtk_http_msg_rsp_t;

/*************************************************************
    Extern variables
*************************************************************/
#if 0
const mbtk_http_header_field_t mbtk_http_headers[] = {
    {HTTP_HEADER_INDEX_CACHE_CONTROL, HTTP_HEADER_GENERAL,"Cache-Control"},
    {HTTP_HEADER_INDEX_CONNECTION, HTTP_HEADER_GENERAL,"Connection"},
    {HTTP_HEADER_INDEX_DATE, HTTP_HEADER_GENERAL,"Date"},
    {HTTP_HEADER_INDEX_PRAGMA, HTTP_HEADER_GENERAL,"Pragma"},
    {HTTP_HEADER_INDEX_TRAILER, HTTP_HEADER_GENERAL,"Trailer"},
    {HTTP_HEADER_INDEX_TRANSFER_ENCODING, HTTP_HEADER_GENERAL,"Transfer-Encoding"},
    {HTTP_HEADER_INDEX_UPGRADE, HTTP_HEADER_GENERAL,"Upgrade"},
    {HTTP_HEADER_INDEX_VIA, HTTP_HEADER_GENERAL,"Via"},
    {HTTP_HEADER_INDEX_WARNING, HTTP_HEADER_GENERAL,"Warning"},
    {HTTP_HEADER_INDEX_ACCEPT, HTTP_HEADER_REQUEST,"Accept"},
    {HTTP_HEADER_INDEX_ACCEPT_CHARSET, HTTP_HEADER_REQUEST,"Accept-Charset"},
    {HTTP_HEADER_INDEX_ACCEPT_ENCODING, HTTP_HEADER_REQUEST,"Accept-Encoding"},
    {HTTP_HEADER_INDEX_ACCEPT_LANGUAGE, HTTP_HEADER_REQUEST,"Accept-Language"},
    {HTTP_HEADER_INDEX_AUTHORIZATION, HTTP_HEADER_REQUEST,"Authorization"},
    {HTTP_HEADER_INDEX_EXPECT, HTTP_HEADER_REQUEST,"Expect"},
    {HTTP_HEADER_INDEX_FROM, HTTP_HEADER_REQUEST,"From"},
    {HTTP_HEADER_INDEX_HOST, HTTP_HEADER_REQUEST,"Host"},
    {HTTP_HEADER_INDEX_IF_MATCH, HTTP_HEADER_REQUEST,"If-Match"},
    {HTTP_HEADER_INDEX_IF_MODIFIED_SINCE, HTTP_HEADER_REQUEST,"If-Modified-Since"},
    {HTTP_HEADER_INDEX_IF_NONE_MATCH, HTTP_HEADER_REQUEST,"If-None-Match"},
    {HTTP_HEADER_INDEX_IF_RANGE, HTTP_HEADER_REQUEST,"If-Range"},
    {HTTP_HEADER_INDEX_IF_UNMODIFIED_SINCE, HTTP_HEADER_REQUEST,"If-Unmodified-Since"},
    {HTTP_HEADER_INDEX_MAX_FORWARDS, HTTP_HEADER_REQUEST,"Max-Forwards"},
    {HTTP_HEADER_INDEX_PROXY_AUTHORIZATION, HTTP_HEADER_REQUEST,"Proxy-Authorization"},
    {HTTP_HEADER_INDEX_RANGE, HTTP_HEADER_REQUEST,"Range"},
    {HTTP_HEADER_INDEX_REFERER, HTTP_HEADER_REQUEST,"Referer"},
    {HTTP_HEADER_INDEX_TE, HTTP_HEADER_REQUEST,"TE"},
    {HTTP_HEADER_INDEX_USER_AGENT, HTTP_HEADER_REQUEST,"User-Agent"},
    {HTTP_HEADER_INDEX_ACCEPT_RANGES, HTTP_HEADER_RESPONSE,"Accept-Ranges"},
    {HTTP_HEADER_INDEX_AGE, HTTP_HEADER_RESPONSE,"Age"},
    {HTTP_HEADER_INDEX_ETAG, HTTP_HEADER_RESPONSE,"ETag"},
    {HTTP_HEADER_INDEX_LOCATION, HTTP_HEADER_RESPONSE,"Location"},
    {HTTP_HEADER_INDEX_PROXY_AUTHENTICATE, HTTP_HEADER_RESPONSE,"Proxy-Authenticate"},
    {HTTP_HEADER_INDEX_RETRY_AFTER, HTTP_HEADER_RESPONSE,"Retry-After"},
    {HTTP_HEADER_INDEX_SERVER, HTTP_HEADER_RESPONSE,"Server"},
    {HTTP_HEADER_INDEX_VARY, HTTP_HEADER_RESPONSE,"Vary"},
    {HTTP_HEADER_INDEX_WWW_AUTHENTICATE, HTTP_HEADER_RESPONSE,"WWW-Authenticate"},
    {HTTP_HEADER_INDEX_ALLOW, HTTP_HEADER_ENTITY,"Allow"},
    {HTTP_HEADER_INDEX_CONTENT_ENCODING, HTTP_HEADER_ENTITY,"Content-Encoding"},
    {HTTP_HEADER_INDEX_CONTENT_LANGUAGE, HTTP_HEADER_ENTITY,"Content-Language"},
    {HTTP_HEADER_INDEX_CONTENT_LENGTH, HTTP_HEADER_ENTITY,"Content-Length"},
    {HTTP_HEADER_INDEX_CONTENT_LOCATION, HTTP_HEADER_ENTITY,"Content-Location"},
    {HTTP_HEADER_INDEX_CONTENT_MD5, HTTP_HEADER_ENTITY,"Content-MD5"},
    {HTTP_HEADER_INDEX_CONTENT_RANGE, HTTP_HEADER_ENTITY,"Content-Range"},
    {HTTP_HEADER_INDEX_CONTENT_TYPE, HTTP_HEADER_ENTITY,"Content-Type"},
    {HTTP_HEADER_INDEX_EXPIRES, HTTP_HEADER_ENTITY,"Expires"},
    {HTTP_HEADER_INDEX_LAST_MODIFIED, HTTP_HEADER_ENTITY,"Last-Modified"},
    {HTTP_HEADER_INDEX_SET_COOKIE, HTTP_HEADER_OTHERS,"Set-Cookie"},
    {HTTP_HEADER_INDEX_COOKIE, HTTP_HEADER_OTHERS,"Cookie"},
    {HTTP_HEADER_INDEX_X_FRAME_OPTIONS, HTTP_HEADER_OTHERS,"X-Frame-Options"},
    {HTTP_HEADER_INDEX_X_XSS_PROTECTION, HTTP_HEADER_OTHERS,"X-XSS-Protection"},
    {HTTP_HEADER_INDEX_DNT, HTTP_HEADER_OTHERS,"DNT"},
    {HTTP_HEADER_INDEX_P3P, HTTP_HEADER_OTHERS,"P3P"}
};
#endif
/*************************************************************
    Public Function Declaration
*************************************************************/
int mbtk_http_handle_get(bool show_rsp_header,mbtk_http_data_callback_func data_cb);
int mbtk_http_handle_free(int handle_id);
int mbtk_http_session_create(int handle_id, mbtk_http_option_enum option,
        mbtk_http_version_enum version);
int mbtk_http_session_option_reset(int handle_id, int session_id, mbtk_http_option_enum option);
int mbtk_http_session_ingnore_cert_set(int handle_id, int session_id, bool ingnore_cert);
int mbtk_http_session_free(int handle_id,int session_id);
int mbtk_http_session_url_set(int handle_id,int session_id,void *url);
int mbtk_http_session_head_add(int handle_id,int session_id,
        char *name, char *value);
int mbtk_http_session_content_set(int handle_id,int session_id,
        char *content,uint32 content_len);
int mbtk_http_session_start(int handle_id,int session_id);
int mbtk_http_session_content_send(int handle_id,int session_id,
        char *data,int data_len);
const mbtk_http_session_t* mbtk_http_session_get(int handle_id,int session_id);


#ifdef __cplusplus
}
#endif

#endif /* _MBTK_HTTP_2_H */


