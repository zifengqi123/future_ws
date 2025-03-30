/*************************************************************
Description:
    mbtk_queue.h
    Used to define mobiletek standard queue interfaces
Author:
    YangDagang
Date:
    2019-7-13
*************************************************************/
#ifndef __MBTK_QUEUE_H__
#define __MBTK_QUEUE_H__

#include "mbtk_type.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************
    Constants and Macros
*************************************************************/
#define MBTK_QUEUE_MAX_NUM 500
#define task_mutex pthread_mutex_t

/*************************************************************
    Definitions:enum,struct,union
*************************************************************/

typedef struct{
    task_mutex crit_sect; //User-defined data type
} mbtk_mutex;

typedef struct node
{
    void *payload;
    int count;          //only used to count for internal
    mbtk_mutex mutex;   //only used to count for internal
    struct node *front;
    struct node *rear;
} mbtk_queue_node_t;

/*************************************************************
    Variables:local,extern
*************************************************************/

/*************************************************************
    Local Function Declaration
*************************************************************/

/*************************************************************
    Extern Function Declaration
*************************************************************/
extern void mbtk_mutex_init(mbtk_mutex *mutex);
extern void mbtk_mutex_deinit(mbtk_mutex *mutex);
extern void mbtk_mutex_lock(mbtk_mutex *mutex);
extern void mbtk_mutex_unlock(mbtk_mutex *mutex);

extern void mbtk_queue_init(mbtk_queue_node_t *queue);
extern int mbtk_queue_put(mbtk_queue_node_t *queue,void *payload);
extern void* mbtk_queue_get(mbtk_queue_node_t *queue);
extern int mbtk_queue_insert(mbtk_queue_node_t *queue,void *payload);
extern bool mbtk_queue_empty(mbtk_queue_node_t *queue);
extern bool mbtk_queue_is_full(mbtk_queue_node_t *queue);
extern void mbtk_queue_deinit(mbtk_queue_node_t *queue);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __MBTK_QUEUE_H__ */
