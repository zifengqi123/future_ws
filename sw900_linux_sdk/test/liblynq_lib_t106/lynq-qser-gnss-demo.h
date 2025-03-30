#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
                                                /**<  0  reserve  */
    E_MT_LOC_MSG_ID_LOCATION_INFO = 1,          /**<  pv_data = &QL_LOC_LOCATION_INFO_T  */
                                                /**<  2  reserve  */
    E_MT_LOC_MSG_ID_NMEA_INFO = 3,              /**<  pv_data = &QL_LOC_NMEA_INFO_T  */
}e_msg_id_t;

#define MOPEN_GNSS_NMEA_MAX_LENGTH  255                 /**  NMEA string maximum length. */
typedef struct
{
    int64_t     timestamp;                              /**<   System Timestamp, marked for when got the nmea data */
    int         length;                                 /**<   NMEA string length. */
    char        nmea[MOPEN_GNSS_NMEA_MAX_LENGTH + 1];   /**<   NMEA string.*/
}mopen_gnss_nmea_info_t;  /* Message */

struct mopen_location_info_t
{
    uint32_t    size;                   /**<   Set to the size of mcm_gps_location_t. */
    int flags;                          /**<   Contains GPS location flags bits. */
    int position_source;                /**<   Provider indicator for HYBRID or GPS. */
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

/*Instantiate callback function*/
void cb
(
        uint32_t    h_loc,
        e_msg_id_t  e_msg_id,
        void        *pv_data,
        void        *context_ptr
        )
{
    printf("e_msg_id=%d\n", e_msg_id);
    switch(e_msg_id)
    {
    case E_MT_LOC_MSG_ID_LOCATION_INFO:
    {
        mopen_location_info_t *pt_location = (mopen_location_info_t *)pv_data;
        printf("**** flag=0x%X, Latitude = %f, Longitude=%f, altitude = %f, speed = %f, timestamp = %lld ****\n",
               pt_location->flags,
               pt_location->latitude,
               pt_location->longitude,
               pt_location->altitude,
               pt_location->speed,
               pt_location->timestamp);
        break;
    }
    case E_MT_LOC_MSG_ID_NMEA_INFO:
    {
        mopen_gnss_nmea_info_t  *pt_nmea = (mopen_gnss_nmea_info_t *)pv_data;

        printf("**** NMEA info: timestamp=%lld, length=%d, nmea=%s ****\n",
               pt_nmea->timestamp, pt_nmea->length, pt_nmea->nmea);
        break;
    }
    }
}

typedef void (*gnss_handler_func_t)
(
        uint32_t    h_loc,
        e_msg_id_t  e_msg_id,
        void        *pv_data,
        void        *context_ptr
        );

typedef enum {
    DELETE_NOTHING = 0,        /**< Delete nothing. */
    DELETE_EPHEMERIS = 1,      /**< Delete ephemeris data. */
    DELETE_ALMANAC = 2,        /**< Delete almanac data. */
    DELETE_POSITION_TIME = 3,  /**< Delete position and time data. */
    DELETE_UTC = 4,            /**< Delete UTC data. */
    DELETE_ALL = 5,            /**< Delete all location data. */
}DELETE_AIDING_DATA_TYPE_T;

typedef struct 
{
    uint32_t year; // >1980
    uint32_t month; // 1-12
    uint32_t day; // 1-31
    uint32_t hour; // 0-23
    uint32_t minute; // 0-59
    uint32_t second; // 0-59
    uint32_t millisecond; // 0-999
}LYNQ_INJECT_TIME_INTO_T; /* Message */


void user_help(void)
{
    printf("\t-1 exit\n"
           "\t1 gnss init\n"
           "\t2 gnss deinit \n"
           "\t3 gnss add handle function\n"
           "\t4 gnss set single mode\n"
           "\t5 gnss set get_para_from_nmea mode\n"
           "\t6 gnss start\n"
           "\t7 gnss stop\n"
           "\t8 gnss Delete_Aiding_Data and reset\n"
           "\t9 gnss injecttime\n"
           "\t10 gnss download tle\n"
           "\t11 gnss set frequency\n"
           "\t12 gnss inject ephemeris\n"
           "\t13 gnss server_configuration\n"
           "please input operator: >> ");
}
void delete_type(void)
{
    printf("\t0 DELETE_NOTHING\n"
           "\t1 DELETE_EPHEMERIS\n"
           "\t2 DELETE_ALMANAC\n"
           "\t3 DELETE_POSITION_TIME \n"
           "\t4 DELETE_UTC\n"
           "\t5 DELETE_ALL\n"
           "please input operator: >> ");
}

#ifdef __cplusplus
}
#endif
