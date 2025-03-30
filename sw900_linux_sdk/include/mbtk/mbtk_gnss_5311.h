#ifndef __MBTK_GNSS_5311_H__
#define __MBTK_GNSS_5311_H__

#include "mbtk_log.h"

typedef void (*mbtk_gnss_nmea_func_t)(void *nmea_data, int nmea_data_len);

typedef enum {
    ASR_GPS_INITIAL_SUCCESS             = 0,
    ASR_GPS_INITIAL_FAILED              = 1,
    ASR_GPS_INITIALED                   = 2,
    ASR_GPS_DOWNLOAD_SUCCESS            = 3,
    ASR_GPS_DOWNLOAD_FAIL               = 4,
    ASR_GPS_SEND_DATA_SUCCESS           = 5,
    ASR_GPS_SEND_DATA_FAIL              = 6,
    ASR_GPS_DEINIT_SUCCESS,
    ASR_GPS_DEINIT_FAIL,
}USER_CB_STATE_T;

typedef enum
{
    MBTK_GNSS_RESULT_SUCCESS = 0,
    MBTK_GNSS_RESULT_FAIL,
    MBTK_GNSS_RESULT_OPEN_SUCCESS,
    MBTK_GNSS_RESULT_OPEN_FAIL,
    MBTK_GNSS_RESULT_DOWNLOAD_SUCCESS,
    MBTK_GNSS_RESULT_DOWNLOAD_FAIL,
    MBTK_GNSS_RESULT_SEND_SUCCESS,
    MBTK_GNSS_RESULT_SEND_FAIL,
    MBTK_GNSS_RESULT_CLOSE_SUCCESS,
    MBTK_GNSS_RESULT_CLOSE_FAIL,
    MBTK_GNSS_RESULT_TIMEOUT,
    MBTK_GNSS_RESULT_UNKNOWN_ERROR
}MBTK_GNSS_5311_RESULT_TYPE;

typedef enum
{
    MBTK_GNSS_MSG_LOCATION_INFO = 0,
    MBTK_GNSS_MSG_NMEA_INFO
}MBTK_GNSS_MSG_INFO_TYPE;

typedef struct
{
    int                     init;
    int                     fd;
    mbtk_gnss_nmea_func_t   callbacks;
    pthread_t               thread;
    int                     control[2];
    MBTK_GNSS_MSG_INFO_TYPE gnss_msg_state;
}mbtk_gnss_nmea_status;

MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_init(void);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_deinit(void);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_open(void);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_close(void);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_sleep(void);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_wakeup(void);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_param_config(const char *param_buf, int param_buf_len);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_get_status(const char *status_buf, int status_buf_len, int *get_status_len);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_add_nmea_out_func(mbtk_gnss_nmea_func_t cb);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_set_nmea_out_type(MBTK_GNSS_MSG_INFO_TYPE type);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_5311_download_tle(char *host, int alam_flag);
MBTK_GNSS_5311_RESULT_TYPE mbtk_gnss_injectEphemeris(void);
#endif