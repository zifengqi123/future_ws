/*
* mbtk_mtd.h
*
* MBTK mtd partition utils header.
*
* Author : lb
* Date   : 2024/2/26 16:03:50
*/
#ifndef _MBTK_MTD_H
#define _MBTK_MTD_H
#include "mbtk_type.h"

#define MBTK_PARTITION_NUM_MAX 40

typedef struct {
    char name[32];
    char dev[16];
    uint32 partition_start;
    uint32 partition_size;
    uint32 erase_size;

    bool used;
} mbtk_partition_info_t;

mbtk_partition_info_t* mbtk_partition_get();
int mbtk_partition_get_by_name(char *partition_name, mbtk_partition_info_t *info);

#endif /* _MBTK_MTD_H */
