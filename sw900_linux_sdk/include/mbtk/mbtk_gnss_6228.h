/**
 *   \file mbtk_gnss_internal.h
 *   \brief A Documented file.
 *
 *  Detailed description
 *   \Author:  Sniper <js.wang@mobiletek.cn>
 *   \Version: 1.0.0
 *   \Date: 2022-03-17
 */

#ifndef __MBTK_GNSS_H__
#define __MBTK_GNSS_H__

#define MOPEN_GNSS_NMEA_MAX_LENGTH  255                 /**  NMEA string maximum length. */

typedef uint32 mbtk_gnss_client_handle;

typedef void (*mbtk_gnss_handler_func_t)
(
    mbtk_gnss_client_handle  h_loc,
    int   e_msg_id,
    void                    *pv_data,
    void                    *context_ptr
);

struct mbtk_gnss_location_info_t
{
    uint32_t    size;                   /**<   Set to the size of mcm_gps_location_t. */
    int flags; /**<   Contains GPS location flags bits. */
    int position_source;  /**<   Provider indicator for HYBRID or GPS. */
    double      latitude;               /**<   Latitude in degrees. */
    double      longitude;              /**<   Longitude in degrees. */
    double      altitude;               /**<   Altitude in meters above the WGS 84 reference ellipsoid. */
    float       speed;                  /**<   Speed in meters per second. */
    float       bearing;                /**<   Heading in degrees. */
    float       accuracy;               /**<   Expected accuracy in meters. */
    int64_t     timestamp;              /**<   Timestamp for the location fix in UTC million-second base.  */
    int32_t     is_indoor;              /**<   Location is indoors. */
    float       floor_number;           /**<   Indicates the floor number. */
};

typedef struct
{
    int64_t     timestamp;                          /**<   System Timestamp, marked for when got the nmea data */
    int         length;                             /**<   NMEA string length. */
    char        nmea[MOPEN_GNSS_NMEA_MAX_LENGTH + 1];   /**<   NMEA string.*/
} mbtk_gnss_nmea_info_t;  /* Message */

int mbtk_gnss_client_init(uint32 *ph_gnss);
int mbtk_gnss_client_deinit(uint32 h_gnss);
int mbtk_gnss_add_rx_msg_handler(uint32 h_gnss, mbtk_gnss_handler_func_t handler_ptr);
int mbtk_gnss_set_mode(uint32 h_gnss, int mode);
int mbtk_gnss_set_system_config(uint32 h_gnss, int mode);
int mbtk_gnss_set_nema_config(uint32 h_gnss, int mode);
int mbtk_gnss_download_tle(void);
int mbtk_gnss_injects_aidpos(uint32 h_gnss);
int mbtk_gnss_firmware_update(void);
int mbtk_gnss_dev_reset(uint32 h_gnss, int type, int mode);
int mbtk_gnss_print_version(uint32 h_gnss);


int mbtk_at_gnss_start_ttff(int type, int timeout_sec, int count);

#endif
