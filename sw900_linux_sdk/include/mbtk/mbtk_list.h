/*
 * mbtk_list.h
 *
 *  Created on: Aug 18, 2020
 *      Author: lb
 */

#ifndef MBTK_LIST_H_
#define MBTK_LIST_H_
#include "mbtk_type.h"

typedef struct list_arraynode
{
    void *data;
    struct list_arraynode *next;
} list_arraynode_t;

typedef struct list_treenode
{
    list_arraynode_t *data;
    int count;
    struct list_treenode *left;
    struct list_treenode *right;
} list_treenode_t;

typedef int (*list_sort_func)(void *data1, void *data2);
typedef void (*list_free_func)(void *data);

typedef struct list_node
{
    uint32 size;
    list_sort_func sort_func;
    list_free_func free_func;

    uint32 cur_index;
    list_arraynode_t *cur_array_data;

    list_arraynode_t array_data;
    list_treenode_t tree_data;
} list_node_t;

#ifdef __cplusplus
extern "C" {
#endif

list_node_t* list_create(list_free_func free_func);
uint32 list_size(list_node_t *list);
void list_add(list_node_t *list, void *data);
void list_add_unique(list_node_t *list, void *data, uint32 len);
void* list_remove(list_node_t *list, void *data);
void* list_get(list_node_t *list, uint32 index);
void* list_remove_by_content(list_node_t *list, void *data, uint32 data_len);
void list_first(list_node_t *list);
void* list_next(list_node_t *list);
void list_sort(list_node_t *list, list_sort_func sort_func);
void list_free(list_node_t *list);
void list_clear(list_node_t *list);
#ifdef __cplusplus
}
#endif

#endif /* MBTK_LIST_H_ */

