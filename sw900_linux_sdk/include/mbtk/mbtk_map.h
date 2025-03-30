/*
 * mbtk_map.h
 *
 *  Created on: Aug 18, 2020
 *      Author: lb
 */

#ifndef MBTK_MAP_H_
#define MBTK_MAP_H_
#include "mbtk_type.h"

typedef void (*map_free_func)(void *data);

typedef struct map_data
{
    char *key;
    void *value;
    struct map_data *next;
} map_data_t;

typedef struct
{
    uint32 size;
    uint32 capacity;
    uint32 cur_index;
    map_data_t *cur_data;
    map_free_func free_func;

    map_data_t **map_array;
} map_node_t;

map_node_t* map_create(uint32 capacity, map_free_func free_func);
uint32 map_size(map_node_t* map);
uint32 map_hash(const char* key, uint32 capacity);
void map_put(map_node_t* map, const char* key, void* value);
void* map_get(map_node_t* map, char* key);
void* map_remove(map_node_t* map, char* key);
void map_first(map_node_t *map);
void* map_next(map_node_t *map);
void map_clear(map_node_t* map);
void map_free(map_node_t* map);

#endif /* MBTK_MAP_H_ */

