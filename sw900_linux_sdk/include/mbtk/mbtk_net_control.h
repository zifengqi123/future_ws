/*************************************************************
Description:
    Header file for network control.
Author:
    LiuBin
Date:
    2019/7/24 17:18:43
*************************************************************/
#ifndef _MBTK_NET_CONTROL_H
#define _MBTK_NET_CONTROL_H
#include "mbtk_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************
    Constants and Macros
*************************************************************/


/*************************************************************
    Definitions:enum,struct,union,class
*************************************************************/
typedef enum {
    MBTK_NET_STATE_OFF,     /* Network off */
    MBTK_NET_STATE_CONN,     /* Network connected */
    MBTK_NET_STATE_CONN_2G,  // 2
    MBTK_NET_STATE_CONN_3G,  // 3
    MBTK_NET_STATE_CONN_4G,  // 4
    MBTK_NET_STATE_CONN_UNKNOWN
} mbtk_net_state_t;

typedef enum {
    MBTK_NET_CHANGE_IF,         // Interface changed.
    MBTK_NET_CHANGE_ADDR        // Address changed
} mbtk_net_change_type_t;

typedef enum {
    MBTK_NET_IF_CHANGE_TYPE_ADD,       // Interface add.
    MBTK_NET_IF_CHANGE_TYPE_DEL        // Interface delete.
} mbtk_net_if_change_type_t;

typedef enum {
    MBTK_NET_IF_CHANGE_STATE_UP,       // Interface up.
    MBTK_NET_IF_CHANGE_STATE_DOWN        // Interface down.
} mbtk_net_if_change_state_t;

typedef enum {
    MBTK_NET_ADDR_CHANGE_TYPE_ADD,       // Address add.
    MBTK_NET_ADDR_CHANGE_TYPE_DEL        // Address delete.
} mbtk_net_addr_change_type_t;

typedef struct {
    int if_index;
    char if_name[100];
    mbtk_net_if_change_type_t type;
    mbtk_net_if_change_state_t state;
} mbtk_net_if_change_info_t;

typedef struct {
    int if_index;
    char if_name[100];
    mbtk_net_addr_change_type_t type;
    char addr[100];
} mbtk_net_addr_change_info_t;

typedef void (*mbtk_net_state_callback_func)(mbtk_net_change_type_t type, const void *data);

/*************************************************************
    Extern variables
*************************************************************/


/*************************************************************
    Public Function Declaration
*************************************************************/
mbtk_net_state_t mbtk_net_state_get();
int mbtk_net_enable(bool enable);
int mbtk_net_monitor_reg(const char* if_name, mbtk_net_state_callback_func state_cb);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _MBTK_NET_CONTROL_H */
