/*
* qser_gnss.h
*
* QSER GNSS header file.
*
* Author : lb
* Date   : 2023/11/23 11:13:18
*/
#ifndef _QSER_GNSS_H
#define _QSER_GNSS_H
#include "mbtk_type.h"
#include "mbtk_gnss.h"
#include "mbtk_log.h"

typedef unsigned int Uint_t;
#define MOPEN_GNSS_NMEA_MAX_LENGTH  255                 /* NMEA string maximum length. */

#define QSER_LEN_MAX 128

typedef enum
{
    E_MT_LOC_MSG_ID_LOCATION_INFO = 1, /**< pv_data = & mopen_location_info_t */
    E_MT_LOC_MSG_ID_NMEA_INFO = 3, /**< pv_data = & mopen_gnss_nmea_info_t */
} e_msg_id_t;

typedef enum {
	QSER_GNSS_ERROR_SUCCESS = 0,
    QSER_GNSS_ERROR_FAIL,
} qser_gnss_error_e;

typedef void (*gnss_handler_func_t)
(
    Uint_t *h_loc,
    e_msg_id_t e_msg_id, //消息 ID
    void *pv_data, //消息内容，取决于 ID，如下说明
    void *context_ptr //用于区分哪个返回的消息
)/* lynq_AddRxIndMsgHandler_t*/;

typedef void (*gnss_async_func_t)(qser_gnss_error_e state);

typedef struct
{
    char host[QSER_LEN_MAX];
    char id[QSER_LEN_MAX];
    char passwd[QSER_LEN_MAX];
}qser_agps_info;

typedef struct
{
    int64_t     timestamp;                              /**<   System Timestamp, marked for when got the nmea data */
    int         length;                                 /**<   NMEA string length. */
    char        nmea[MOPEN_GNSS_NMEA_MAX_LENGTH + 1];   /**<   NMEA string.*/
}mopen_gnss_nmea_info_t;  /* Message */

typedef struct
{
    uint32_t    size;                   /**<   Set to the size of mcm_gps_location_t. */
    int flags;                          /**<   Contains GPS location flags bits. */
    int position_source;                /**<   Provider indicator for HYBRID or GPS. */ //功能暂未实现，可不用添加进结构体
    double      latitude;               /**<   Latitude in degrees. */
    double      longitude;              /**<   Longitude in degrees. */
    double      altitude;               /**<   Altitude in meters above the WGS 84 reference ellipsoid. */
    float       speed;                  /**<   Speed in meters per second. */
    float       bearing;                /**<   Heading in degrees. */ //功能暂未实现，可不用添加进结构体
    float       accuracy;               /**<   Expected accuracy in meters. */ //功能暂未实现，可不用添加进结构体
    int64_t     timestamp;              /**<   Timestamp for the location fix in UTC million-second base.  */
    int32_t     is_indoor;              /**<   Location is indoors. */ //功能暂未实现，可不用添加进结构体
    float       floor_number;           /**<   Indicates the floor number. */
}mopen_location_info_t;//功能暂未实现，可不用添加进结构体

typedef struct
{
    uint32_t year; // 大于 1980
    uint32_t month; // 1-12
    uint32_t day; // 1-31
    uint32_t hour; // 0-23
    uint32_t minute; // 0-59
    uint32_t second; // 0-59
    uint32_t millisecond; // 0-999
} LYNQ_INJECT_TIME_INTO_T; /* Message */

typedef enum {
    DELETE_NOTHING = 0, /*不删除数据*/
    DELETE_EPHEMERIS = 1, /*删除星历*/
    DELETE_ALMANAC = 2, /*删除历书*/
    DELETE_POSITION_TIME = 3, /*删除时间和位置信息. */
    DELETE_UTC = 4, /*删除 UTC 时间*/
    DELETE_ALL = 5 /*删除所有*/
} DELETE_AIDING_DATA_TYPE_T;

int qser_Gnss_Init (uint32_t *h_gnss);

int qser_Gnss_Deinit (uint32_t h_gnss);

int qser_AddRxIndMsgHandler (gnss_handler_func_t handler_ptr,uint32_t h_gnss);

int qser_Set_Indications (uint32_t h_gnss,e_msg_id_t type);

int qser_Gnss_Set_Async_Callback(gnss_async_func_t cb);

int qser_Gnss_Start (uint32_t h_gnss);

int qser_Gnss_Start_Async(uint32_t h_gnss);

int qser_Gnss_Stop (uint32_t h_gnss);

int qser_Gnss_Set_Frequency(uint32_t h_gnss, int frequency);

int qser_Gnss_InjectTime (uint32_t h_gnss,LYNQ_INJECT_TIME_INTO_T *time_info);

int qser_Gnss_Delete_Aiding_Data (uint32_t h_gnss,DELETE_AIDING_DATA_TYPE_T flags);

int qser_Gnss_Server_Configuration(char *host, char *id, char *password);

int qser_Gnss_download_tle();

int qser_Gnss_injectEphemeris(uint32_t h_gnss);


#endif /* _QSER_GNSS_H */
