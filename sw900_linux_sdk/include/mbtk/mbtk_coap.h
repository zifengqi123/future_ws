/*************************************************************
Description:
    L620 Coap support Header file.
Author:
    LuoJian
Date:
    2018/9/16 4:26:36
*************************************************************/
#ifndef FEATURE_MBTK_ECOAP
#define FEATURE_MBTK_ECOAP
#include <ctype.h>
#include <stdbool.h>

#include "mbtk_coap_api.h"

/*************************************************************
    Constants and Macros
*************************************************************/
#define MBTK_COAP_DTLS_SUPPORT

#define MBTK_ECOAP_NEW_MAX 128
#define MBTK_ECOAP_OPTION_LEN 512
#define MBTK_ECOAP_DATA_MAX 1024


/*************************************************************
    Definitions:enum,struct,union,class
*************************************************************/

#define MBTK_ECOAP_ERR_SUCCESS  0                 /* No error */
#define	MBTK_ECOAP_ERR_NO_OPEN 300               /* 300 coap service is not opened. */
#define	MBTK_ECOAP_ERR_IS_OPENED 301                /* 301 coap service is opened.*/
#define	MBTK_ECOAP_ERR_NO_READY 302              /* 302 coap host and port not set */
#define	MBTK_ECOAP_ERR_NET 303                   /* 303 DNS error. */
#define	MBTK_ECOAP_ERR_SOCK_CANNOT_RST 304       /* 304 Socket can not reset. */
#define	MBTK_ECOAP_ERR_MSG_ID 305                /* 305 Message ID error. */
#define	MBTK_ECOAP_ERR_PAYLOAD_LENGTH 306        /* 306 Payload length error. */
#define	MBTK_ECOAP_ERR_OPTION 307                /* 307 Potion set error. */
#define	MBTK_ECOAP_ERR_TASK_IS_RUNING 308        /* 308 Task is runing. */
#define	MBTK_ECOAP_ERR_UNKNOWN 350           /* 350 Unknown coap error */




typedef enum
{
    MBTK_ECOAP_BLOCK_16,
    MBTK_ECOAP_BLOCK_32,
    MBTK_ECOAP_BLOCK_64,
    MBTK_ECOAP_BLOCK_128,
    MBTK_ECOAP_BLOCK_256,
    MBTK_ECOAP_BLOCK_512,
    MBTK_ECOAP_BLOCK_1024
} mbtk_ecoap_option_block_e;

typedef enum {
	MBTK_ECOAP_SIG_START,
	MBTK_ECOAP_SIG_STOP,
    MBTK_ECOAP_SIG_SEND,
    MBTK_ECOAP_SIG_RECV,
    MBTK_ECOAP_SIG_DISCONNING
} mbtk_ecoap_base_sig_enum;

typedef enum
{
    MBTK_ECOAP_STATE_NON,
    MBTK_ECOAP_STATE_CLOSING,
    MBTK_ECOAP_STATE_OPENED,
    MBTK_ECOAP_STATE_READY
} mbtk_ecoap_state_e;

typedef struct
{
    mbtk_ecoap_option_block_e size;
    int number;
    uint8 more_flag;
} mbtk_ecoap_option_block_s;

typedef struct mbtk_ecoap_option
{
    mbtk_coap_option_type type;

    union
    {
        mbtk_ecoap_option_block_s opt_block;
        mbtk_content_format_type opt_content_format;
        uint8 opt_str[128];
        int opt_int;
    } opt;

    struct mbtk_ecoap_option *next;
} mbtk_ecoap_option_s;

typedef struct mbtk_ecoap_option_2{
	int optlen;
	uint8 optVal[512+1];
}mbtk_ecoap_opt_2;

typedef struct mbtk_ecoap_option_3{
	int optlen;
	char optVal[512+1];
}mbtk_ecoap_opt_3;

typedef struct
{
    uint8 version;
    mbtk_coap_type type;
    mbtk_coap_code_type code;
    int message_id;

    uint8 token[8];
    uint8 token_len;

    uint8 send_count;

    mbtk_ecoap_opt_2 option;

    uint8 *payload;
    int payload_len;
}mbtk_ecoap_package_ver;


typedef struct mbtk_ecoap_package
{
    uint8 send_count;

    uint8 version;
    mbtk_coap_type type;
    mbtk_coap_code_type code;

    uint8 token[8];
    uint8 token_len;

    int message_id;

    mbtk_ecoap_option_s *options;

    uint8 *payload;
    int payload_len;

    struct mbtk_ecoap_package *next;
} mbtk_ecoap_package_s;

typedef struct
{
    uint8 host[32];
    int port;
	int socket;
#ifdef MBTK_COAP_DTLS_SUPPORT
    int is_dtls;
#endif
    int message_id;
} mbtk_ecoap_net_s;


/*************************************************************
    Extern variables
*************************************************************/


/*************************************************************
    Public Function Declaration
*************************************************************/
int mbtk_coap_ecoapnew_exec_cmd
(
    char *ip_addr,
    int port,
    bool is_support_ssl, 
    bool ingnore_cert
);

int mbtk_coap_ecoaprxmod_exec_cmd
(
	int mode
);

int mbtk_coap_ecoappr_exec_cmd
(
	int format
);


int mbtk_coap_ecoaprxget_exec_cmd
(
    int len
);

int mbtk_coap_ecoapver_exec_cmd
(
    int version
);

int mbtk_coap_ecoaptype_exec_cmd
(
    mbtk_coap_type type
);

int mbtk_coap_ecoapcode_exec_cmd
(
    mbtk_coap_code_type code
);

int mbtk_coap_ecoaptoken_exec_cmd
(
    char *token_buf, int len
);

int mbtk_coap_ecoapmsgid_exec_cmd
(
    int msg_id
);

int mbtk_coap_ecoapopt_exec_cmd
(
    char *value_buf, int buf_len
);

int mbtk_coap_ecoapsend_exec_cmd
(
    int message_id, int data_len, char *data
);

int mbtk_coap_ecoapdel_exec_cmd( int del_id );

int mbtk_coap_ecoapnmi_exec_cmd( void );


#endif /* _MBTK_COAP_L620_H */
