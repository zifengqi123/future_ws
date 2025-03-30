/*
* mbtk_gnss.h
*
* MBTK GNSS API header.
*
* Author : lb
* Date   : 2024/7/11 16:14:33
*/
#ifndef __MBTK_GNSS_H
#define __MBTK_GNSS_H
#include <pthread.h>

#include "mbtk_type.h"

// Refor to : mbtk_gnss_ind_set() gnss_ind
#define MBTK_GNSS_IND_LOCATION (1)      // 1
#define MBTK_GNSS_IND_NMEA (1 << 1)     // 2

// Refor to : mbtk_gnss_open() type
#define GNSS_PRINT_PORT_UART1       1           // 1
#define GNSS_PRINT_PORT_USB_NMEA    (1<<1)      // 2
#define GNSS_PRINT_PORT_USB_AT      (1<<2)      // 4
#define GNSS_PRINT_PORT_TTY_AT      (1<<3)      // 8

#define MBTK_IND_START_FLAG     0xFF
#define MBTK_IND_END_FLAG       0xEE
#define MBTK_IND_LOCATION_TAG   "IND_LOC:"
#define MBTK_IND_NMEA_TAG       "IND_NMEA:"

#if 1//MBTK_GNSS_LOCATION_INFO
#define MBTK_GNSS_LOG_ENABLED 0

#define MAX_NMEA_TOKENS  32
#define NMEA_RMC_PARAM_NUM 14
#define NMEA_GGA_PARAM_NUM 15

/** GpsLocation has valid latitude and longitude. */
#define GNSS_LOCATION_HAS_LAT_LONG   0x00000001
/** GpsLocation has valid altitude. */
#define GNSS_LOCATION_HAS_ALTITUDE   0x00000002
/** GpsLocation has valid speed. */
#define GNSS_LOCATION_HAS_SPEED      0x00000004
/** GpsLocation has valid bearing. */
#define GNSS_LOCATION_HAS_BEARING    0x00000008
/** GpsLocation has valid timestamp. */
#define GNSS_LOCATION_HAS_TIMESTAMP  0x00000010
/** GpsLocation has valid all. */
#define GNSS_LOCATION_HAS_ALL  0x0000001F
#endif

typedef uint32 mbtk_gnss_handle;

typedef void (*mbtk_gnss_callback_func)(uint32 ind_type, const void* data, uint32 data_len);

typedef enum {
    GNSS_ERR_OK,
    GNSS_ERR_CLI_FULL,
    GNSS_ERR_UNKNOWN_HANDLE,
    GNSS_ERR_UNSUPPORT,
    GNSS_ERR_TIMEOUT,
    GNSS_ERR_ARG,
    GNSS_ERR_CHECKSUM,
    GNSS_ERR_SET_BUSY,
    GNSS_ERR_DL_FW,
    GNSS_ERR_OPEN_DEV,
    GNSS_ERR_FORMAT,
    GNSS_ERR_BUSY,
    GNSS_ERR_EPH_GET_FAIL,
    GNSS_ERR_EPH_INJECT_FAIL,

    GNSS_ERR_UNKNOWN
} gnss_err_enum;

#if 1//MBTK_GNSS_LOCATION_INFO
typedef struct {
        const char*  head;
        const char*  end;
} mbtk_token;

typedef struct {
        int     count;
        mbtk_token   tokens[ MAX_NMEA_TOKENS ];
} mbtk_nmeatokenizer;

typedef struct {
    uint32      flags;
    double      latitude;               /**<   Latitude in degrees. */
    double      longitude;              /**<   Longitude in degrees. */
    double      altitude;               /**<   Altitude in meters above the WGS 84 reference ellipsoid. */
    float       speed;                  /**<   Speed in meters per second. */
    float       bearing;                /**<   Heading in degrees. */
    int64_t     timestamp;              /**<   Timestamp for the location fix in UTC million-second base.  */
} mbtk_gnss_location_info_t;
#endif

gnss_err_enum mbtk_gnss_init(mbtk_gnss_callback_func cb);

gnss_err_enum mbtk_gnss_deinit();

gnss_err_enum mbtk_gnss_open(int type, int timeout);

gnss_err_enum mbtk_gnss_close(int timeout);

gnss_err_enum mbtk_gnss_setting(const char *setting_cmd, int timeout);

gnss_err_enum mbtk_gnss_dl(const char *fw_path, int timeout);

gnss_err_enum mbtk_gnss_ind_set(uint32 gnss_ind, int timeout);

gnss_err_enum mbtk_gnss_eph_download(int timeout);

gnss_err_enum mbtk_gnss_eph_inject(int timeout);

#endif /* __MBTK_GNSS_H */
