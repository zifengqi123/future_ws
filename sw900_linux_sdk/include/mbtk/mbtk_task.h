/*************************************************************
Description:
    mbtk_task.h
    Used to define mobiletek standard task or thread interfaces
Author:
    YangDagang
Date:
    2019-7-13
*************************************************************/
#ifndef __MBTK_TASK_H__
#define __MBTK_TASK_H__

#include "mbtk_queue.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************
    Constants and Macros
*************************************************************/
typedef void* (*mbtk_task_cb_handle)(void *payload);
typedef void (*mbtk_uart_printf_cb)(char* data,int data_len);


/*************************************************************
    Definitions:enum,struct,union
*************************************************************/
typedef struct
{
    unsigned int sig;
    void* payload;
} mbtk_signal_info;

typedef struct
{
    void* task_id;
    mbtk_task_cb_handle thread_run;
    void* args;
} mbtk_task_info;

typedef struct
{
    mbtk_uart_printf_cb urc_prt;
    mbtk_uart_printf_cb data_prt;
} mbtk_reg_callback;

typedef struct {
    pthread_t thread_id;
    pthread_cond_t cond;
    mbtk_mutex mutex;
    mbtk_queue_node_t queue;
} mbtk_task_queue_info;


/*************************************************************
    Variables:local,extern
*************************************************************/

/*************************************************************
    Local Function Declaration
*************************************************************/

/*************************************************************
    Extern Function Declaration
*************************************************************/
extern int mbtk_task_start(mbtk_task_info *task);
extern int mbtk_task_queue_start(void *param,mbtk_task_cb_handle cb);
extern void mbtk_task_queue_stop(void *param);
extern int mbtk_signal_send(void *param,mbtk_signal_info* info);
extern mbtk_signal_info *mbtk_signal_get(void *param);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __MBTK_TASK_H__ */

