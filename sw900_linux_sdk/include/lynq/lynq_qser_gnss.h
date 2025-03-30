#ifndef __LYNQ_GNSS_H__
#define __LYNQ_GNSS_H__
#ifdef __cplusplus
extern "C" {
#endif
struct mbtk_gnss_handle_t
{
    int dev_fd;
    pthread_t  uart_pthread;
    pthread_t  gnss_pthread;
    gnss_handler_func_t gnss_handler_func;
    int mode; // 0 - stop, 1 - single, 2 - periodic, 3 - start
    pthread_mutex_t _cond_mutex;
    int reset_state;
    int inited;
    ring_buffer_t ring_buffer;
    int getap_status;
    char *rb;

#if TTFF_TEST
    pthread_t ttff_pid;
    int location_state;
#endif
    /********************
    save handle's adr
    phandle = &handle
    handle  = mbtk_gnss_handle
    *********************/
    uint32_t *phandle; // handle's adr
};

typedef struct 
{
    uint32_t    size;                   /**<   Set to the size of mcm_gps_sv_info_t. */
    int         prn;                    /**<   Pseudo-random number for the SV. */ 
    float       snr;                    /**<   Signal-to-noise ratio. */ 
    float       elevation;              /**<   Elevation of the SV in degrees. */ 
    float       azimuth;                /**<   Azimuth of the SV in degrees. */ 
}QL_LOC_SV_INFO_T;                      /* Type */

struct login_information  /*Used To Stored User Information*/
{
    char*   host;
    char*   id;
    char*   password;
};

static struct login_information *login_information_t = NULL;

#define     QL_LOC_GPS_SUPPORT_SVS_MAX   32  /**  Maximum number of satellites in view. */
typedef struct 
{
    uint32_t            size;                                   /**<   Set to the size of mcm_gps_sv_status_t. */
    int                 num_svs;                                /**<   Number of SVs currently visible. */
    QL_LOC_SV_INFO_T    sv_list[QL_LOC_GPS_SUPPORT_SVS_MAX];    /**<   Contains an array of SV information. */
    uint32_t            ephemeris_mask;                         /**<   Bitmask indicating which SVs have ephemeris data.  */
    uint32_t            almanac_mask;                           /**<   Bitmask indicating which SVs have almanac data.   */
    uint32_t            used_in_fix_mask;                       /**<   Bitmask indicating which SVs were used for computing the most recent position fix. */
}QL_LOC_SV_STATUS_T;  /* Type */

typedef enum 
{
    LYNQ_POS_MODE_STANDALONE        = 0, /**<  Mode for running GPS standalone (no assistance).  */
    LYNQ_POS_MODE_MS_BASED          = 1, /**<  AGPS MS-Based mode.  */
    LYNQ_POS_MODE_MS_ASSISTED       = 2, /**<  AGPS MS-Assisted mode.  */
}LYNQ_POS_MODE_T;

typedef enum 
{
    LYNQ_POS_RECURRENCE_PERIODIC    = 0, /**<  Receive GPS fixes on a recurring basis at a specified period.  */
    LYNQ_POS_RECURRENCE_SINGLE      = 1, /**<  Request a single-shot GPS fix.  */
}LYNQ_POS_RECURRENCE_T;

struct LYNQ_POS_MODE_INFO_T
{
    LYNQ_POS_MODE_T       mode;               /*  Position mode.      */
    LYNQ_POS_RECURRENCE_T recurrence;         /*  Recurrence          */
};


#endif
typedef struct 
{
    uint32_t year; // 大于1980
    uint32_t month; // 1-12
    uint32_t day; // 1-31
    uint32_t hour; // 0-23
    uint32_t minute; // 0-59
    uint32_t second; // 0-59
    uint32_t millisecond; // 0-999
}LYNQ_INJECT_TIME_INTO_T; /* Message */


typedef enum  {
    LOC_IND_STATUS_INFO_ON,//E_MT_LOC_MSG_ID_STATUS_INFO = 0,
    LOC_IND_LOCATION_INFO_ON,//E_MT_LOC_MSG_ID_LOCATION_INFO,
    LOC_IND_SV_INFO_ON,//E_MT_LOC_MSG_ID_SV_INFO,
    LOC_IND_NMEA_INFO_ON,//E_MT_LOC_MSG_ID_NMEA_INFO,
    LOC_IND_CAP_INFO_ON,//E_MT_LOC_MSG_ID_CAPABILITIES_INFO,
    LOC_IND_AGPS_DATA_CONN_CMD_REQ_ON,//E_MT_LOC_MSG_ID_AGPS_STATUS,
    LOC_IND_NI_NFY_USER_RESP_REQ_ON,//E_MT_LOC_MSG_ID_NI_NOTIFICATION,
    LOC_IND_XTRA_DATA_REQ_ON,//E_MT_LOC_MSG_ID_XTRA_REPORT_SERVER,
}msg_t;

typedef enum {
    DELETE_NOTHING = 0, /**< Delete nothing. */
    DELETE_EPHEMERIS = 1, /**< Delete ephemeris data. */
    DELETE_ALMANAC = 2, /**< Delete almanac data. */
    DELETE_POSITION_TIME = 3, /**< Delete position and time data. */
    DELETE_UTC = 4, /**< Delete UTC data. */
    DELETE_ALL = 5, /**< Delete all location data. */
}DELETE_AIDING_DATA_TYPE_T;

typedef void ( *lynq_atsvc_incb )( const char *input,const int length);
typedef void ( *lynq_atsvc_outcb )(char *output, int out_max_size, int mode);
lynq_atsvc_incb lynq_register_gnss(lynq_atsvc_outcb out_cb);

int qser_Gnss_Init(uint32_t *h_gnss);
int qser_Gnss_Deinit(uint32_t h_gnss);
int qser_AddRxIndMsgHandler(gnss_handler_func_t handler_ptr,uint32_t h_gnss);
int qser_Set_Indications(uint32_t h_gnss,e_msg_id_t type);
int qser_Gnss_Start(uint32_t h_gnss);
int qser_Gnss_Stop(uint32_t h_gnss);
int qser_Gnss_Set_Frequency(uint32_t h_gnss, int frequency);
int qser_Gnss_Delete_Aiding_Data(uint32_t h_gnss,DELETE_AIDING_DATA_TYPE_T flags);
int qser_Gnss_Server_Configuration(char *host, char *id, char *password);
int qser_Gnss_InjectTime(uint32_t h_gnss,LYNQ_INJECT_TIME_INTO_T *time_info);
int qser_firmware_update(uint32_t h_gnss);
int qser_Gnss_injectEphemeris(uint32_t h_gnss);
int qser_Gnss_download_tle();
#ifdef __cplusplus
}
#endif

