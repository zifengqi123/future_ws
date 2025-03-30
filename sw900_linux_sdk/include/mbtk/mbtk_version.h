/*
* mbtk_version.h
*
* mbtk_source compilation informations.
*
* Author : lb
* Date   : 2024/6/7 16:51:20
*/
#ifndef __MBTK_VERSION_H
#define __MBTK_VERSION_H
#include "mbtk_type.h"

typedef enum {
    MBTK_BUILD_DEF_AF_SUPPORT = 0,
    MBTK_BUILD_DEF_YX_SUPPORT,
    MBTK_BUILD_DEF_SG_SUPPORT,
    MBTK_BUILD_DEF_MBTK_ALL_CID_SUPPORT,
    MBTK_BUILD_DEF_MBTK_GNSS_MODE,
    MBTK_BUILD_DEF_MBTK_DUMP_SUPPORT,

    MBTK_BUILD_DEF_NUM
} mbtk_build_define_enum;

typedef struct {
    char name[64];
    char value[64];
} mbtk_build_def_info_t;

void mbtk_build_def_get(char *buff, int buff_len);

#endif /* __MBTK_VERSION_H */
