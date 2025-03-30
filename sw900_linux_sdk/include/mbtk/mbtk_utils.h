/*************************************************************
Description:
    MBTK utils head file.
Author:
    LiuBin
Date:
    2019/7/24 20:08:03
*************************************************************/
#ifndef _MBTK_UTILS_H
#define _MBTK_UTILS_H
#include "mbtk_type.h"
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************
    Constants and Macros
*************************************************************/
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif
#define MBTK_CMDLINE_LEN  1024


/*************************************************************
    Definitions:enum,struct,union,class
*************************************************************/
typedef void (*mbtk_timer_alrm_func)(int signo);
typedef void (*mbtk_cmd_cb_func)(char *buf,int buf_size);


/*************************************************************
    Extern variables
*************************************************************/
typedef enum {
    MBTK_BYTEORDER_BIG,     // Gig Endian
    MBTK_BYTEORDER_LITTLE,  // Little Endian
    MBTK_BYTEORDER_UNKNOWN
} mbtk_byteorder_enum;

/*************************************************************
    Public Function Declaration
*************************************************************/
bool mbtk_cmd_line
(
    const char *cmd,
    char *buf,
    int buf_size
);

bool mbtk_cmd_line_ex
(
    const char *cmd,
    mbtk_cmd_cb_func cb
);

int mbtk_send_at(const void *at_req,void* at_rsp,int rsp_size,int timeout);

/*
* Set timer as microseconds.
*/
int mbtk_timer_set(mbtk_timer_alrm_func func,uint32 timeout_ms);

/**
* Clear current timer.
*/
int mbtk_timer_clear();

int mbtk_get_kernel_cmdline(char *buf, int len);

/** returns 1 if line starts with prefix, 0 if it does not */
int strStartsWith(const char *line, const char *prefix);

char* mbtk_time_text_get(char *buff, size_t buff_size);

mbtk_byteorder_enum mbtk_byteorder_get();

uint16 byte_2_uint16(const void *buff, bool big_endian);

int uint16_2_byte(uint16 a, void *buff, bool big_endian);

uint32 byte_2_uint32(const void *buff, bool big_endian);

int uint32_2_byte(uint32 a, void *buff, bool big_endian);

uint64 byte_2_uint64(const void *buff, bool big_endian);

int uint64_2_byte(uint64 a, void *buff, bool big_endian);

void* memdup(const void* data, int data_len);

int app_already_running(const char *pid_file);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _MBTK_UTILS_H */
