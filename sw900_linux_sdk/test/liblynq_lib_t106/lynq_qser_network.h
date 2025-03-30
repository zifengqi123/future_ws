/**
 *@file     QSER_nw.h
 *@date     2018-02-22
 *@author   
 *@brief    
 */   
#ifndef __LYNQ_QSER_NETWORK_H__
#define __LYNQ_QSER_NETWORK_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "mbtk_type.h"

typedef uint32_t nw_client_handle_type;


#define QSER_NW_MODE_NONE     0x00    /**<  No network. */
#define QSER_NW_MODE_GSM      0x01    /**<  Include GSM networks. */
#define QSER_NW_MODE_WCDMA    0x02    /**<  Include WCDMA networks. */
#define QSER_NW_MODE_CDMA     0x04    /**<  Include CDMA networks. */
#define QSER_NW_MODE_EVDO     0x08    /**<  Include EVDO networks. */
#define QSER_NW_MODE_LTE      0x10    /**<  Include LTE networks. */
#define QSER_NW_MODE_TDSCDMA  0x20    /**<  Include TDSCDMA networks. */

typedef enum 
{
    E_QSER_NW_ROAM_STATE_OFF  = 0,    /**<  None, or roaming indicator off. */
    E_QSER_NW_ROAM_STATE_ON   = 1     /**<  Roaming indicator on. */
}E_QSER_NW_ROAM_STATE_TYPE_T;

/** Configures the settings that define the MCM network interface. */
typedef struct 
{
    /*  Configuration parameters for MCM network registration Network registration details Technology dependent network registration details */
    uint64_t                        preferred_nw_mode;  /**<   Preferred network mode for connections; a bitmask of QSER_NW_MODE_xxxx.*/
    E_QSER_NW_ROAM_STATE_TYPE_T   roaming_pref;       /**<   Roaming preference.*/
}QSER_NW_CONFIG_INFO_T;

typedef enum 
{
    E_QSER_NW_IMS_MODE_OFF    = 0,     /**<  close ims. */
    E_QSER_NW_IMS_MODE_VOLTE_ENABLE    = 1,  /**<  support volte. */
}E_QSER_NW_IMS_MODE_TYPE_T;

/** Configures the OOS (out of service)  settings that define the MCM network interface. */
#define QSER_NW_OOS_CFG_TYPE_FAST_SCAN          0x00    /**<  fast net scan */
#define QSER_NW_OOS_CFG_TYPE_FULL_BAND_SCAN     0x01    /**<  full band scan */

typedef struct 
{
    /*    Configuration parameters for MCM network fast network scan when OOS (out of service)*/
    char                        enable;
    uint16_t                    time_interval;
}QSER_NW_OOS_CONFIG_FAST_SCAN_INFO_T;

typedef struct 
{
    /*    Configuration parameters for MCM network full band network scan when OOS (out of service)*/
    int t_min;
    int t_step;
    int t_num;
    int t_max;
}QSER_NW_OOS_CONFIG_FULL_BAND_SCAN_INFO_T;


typedef struct 
{    
    char                        type;  /**<   QSER_NW_OOS_CFG_TYPE_xxxx.*/
    union {
    QSER_NW_OOS_CONFIG_FAST_SCAN_INFO_T  fast_can_info;       // 00
    QSER_NW_OOS_CONFIG_FULL_BAND_SCAN_INFO_T full_band_scan_info;     // 01   
  } u;
}QSER_NW_OOS_CONFIG_INFO_T;

//defined for QSER_NW_EventRegister
#define     NW_IND_VOICE_REG_EVENT_IND_FLAG               (1 << 0)    /**< msg format : QSER_NW_VOICE_REG_EVENT_IND_T */
#define     NW_IND_DATA_REG_EVENT_IND_FLAG                (1 << 1)    /**< msg format : QSER_NW_DATA_REG_EVENT_IND_T */ 
#define     NW_IND_SIGNAL_STRENGTH_EVENT_IND_FLAG         (1 << 2)    /**< msg format : QSER_NW_SINGNAL_EVENT_IND_T */ 
//#define     NW_IND_CELL_ACCESS_STATE_CHG_EVENT_IND_FLAG   (1 << 3)    /**< msg format : QL_MCM_NW_CELL_ACCESS_STATE_EVENT_IND_T */ 
//#define     NW_IND_NITZ_TIME_UPDATE_EVENT_IND_FLAG        (1 << 4)    /**< msg format : QL_MCM_NW_NITZ_TIME_EVENT_IND_T */ 
#define     NW_IND_IMS_REG_EVENT_IND_FLAG                 (1 << 5)    /**< msg format : NULL */ 

typedef struct 
{
    char long_eons[512 + 1];    /**<   Long EONS.*/
    char short_eons[512 + 1];   /**<   Short EONS.*/
    char mcc[3 + 1];            /**<   Mobile country code.*/
    char mnc[3 + 1];            /**<   Mobile network code.*/
}QSER_NW_OPERATOR_NAME_INFO_T;

typedef enum 
{
    E_QSER_NW_RADIO_TECH_TD_SCDMA = 1,
    E_QSER_NW_RADIO_TECH_GSM      = 2,    /**<  GSM; only supports voice. */
    E_QSER_NW_RADIO_TECH_HSPAP    = 3,    /**<  HSPA+. */
    E_QSER_NW_RADIO_TECH_LTE      = 4,    /**<  LTE. */
    E_QSER_NW_RADIO_TECH_EHRPD    = 5,    /**<  EHRPD. */
    E_QSER_NW_RADIO_TECH_EVDO_B   = 6,    /**<  EVDO B. */
    E_QSER_NW_RADIO_TECH_HSPA     = 7,    /**<  HSPA. */
    E_QSER_NW_RADIO_TECH_HSUPA    = 8,    /**<  HSUPA. */
    E_QSER_NW_RADIO_TECH_HSDPA    = 9,    /**<  HSDPA. */
    E_QSER_NW_RADIO_TECH_EVDO_A   = 10,   /**<  EVDO A. */
    E_QSER_NW_RADIO_TECH_EVDO_0   = 11,   /**<  EVDO 0. */
    E_QSER_NW_RADIO_TECH_1xRTT    = 12,   /**<  1xRTT. */
    E_QSER_NW_RADIO_TECH_IS95B    = 13,   /**<  IS95B. */
    E_QSER_NW_RADIO_TECH_IS95A    = 14,   /**<  IS95A. */
    E_QSER_NW_RADIO_TECH_UMTS     = 15,   /**<  UMTS. */
    E_QSER_NW_RADIO_TECH_EDGE     = 16,   /**<  EDGE. */
    E_QSER_NW_RADIO_TECH_GPRS     = 17,   /**<  GPRS. */
    E_QSER_NW_RADIO_TECH_NONE     = 18    /**<  No technology selected. */
}E_QSER_NW_RADIO_TECH_TYPE_T;


typedef enum 
{
    E_QSER_NW_TECH_DOMAIN_NONE    = 0,    /**<  None. */
    E_QSER_NW_TECH_DOMAIN_3GPP    = 1,    /**<  3GPP. */
    E_QSER_NW_TECH_DOMAIN_3GPP2   = 2,    /**<  3GPP2. */
}E_QSER_NW_TECH_DOMAIN_TYPE_T;

typedef enum 
{
    E_QSER_NW_IMSI_UNKNOWN_HLR_DENY_REASON                    = 1, /**<  IMSI unknown in HLR. */
    E_QSER_NW_ILLEGAL_MS_DENY_REASON                          = 2, /**<  Illegal MS. */
    E_QSER_NW_IMSI_UNKNOWN_VLR_DENY_REASON                    = 3, /**<  IMSI unknown in VLR. */
    E_QSER_NW_IMEI_NOT_ACCEPTED_DENY_REASON                   = 4, /**<  IMEI not accepted. */
    E_QSER_NW_ILLEGAL_ME_DENY_REASON                          = 5, /**<  Illegal ME. */
    E_QSER_NW_PLMN_NOT_ALLOWED_DENY_REASON                    = 6, /**<  PLMN not allowed. */
    E_QSER_NW_LA_NOT_ALLOWED_DENY_REASON                      = 7, /**<  Location area not allowed. */
    E_QSER_NW_ROAMING_NOT_ALLOWED_LA_DENY_REASON              = 8, /**<  Roaming not allowed in this location area. */
    E_QSER_NW_NO_SUITABLE_CELLS_LA_DENY_REASON                = 9, /**<  No suitable cells in location area. */
    E_QSER_NW_NETWORK_FAILURE_DENY_REASON                     = 10, /**<  Network failure. */
    E_QSER_NW_MAC_FAILURE_DENY_REASON                         = 11, /**<  MAC failure. */
    E_QSER_NW_SYNCH_FAILURE_DENY_REASON                       = 12, /**<  Sync failure. */
    E_QSER_NW_CONGESTION_DENY_REASON                          = 13, /**<  Congestion. */
    E_QSER_NW_GSM_AUTHENTICATION_UNACCEPTABLE_DENY_REASON     = 14, /**<  GSM authentication unacceptable. */
    E_QSER_NW_NOT_AUTHORIZED_CSG_DENY_REASON                  = 15, /**<  Not authorized in this CSG. */
    E_QSER_NW_SERVICE_OPTION_NOT_SUPPORTED_DENY_REASON        = 16, /**<  Service option not supported. */
    E_QSER_NW_REQ_SERVICE_OPTION_NOT_SUBSCRIBED_DENY_REASON   = 17, /**<  Requested service option not subscribed. */
    E_QSER_NW_CALL_CANNOT_BE_IDENTIFIED_DENY_REASON           = 18, /**<  Call cannot be identified. */
    E_QSER_NW_SEMANTICALLY_INCORRECT_MSG_DENY_REASON          = 19, /**<  Semantically incorrect message. */
    E_QSER_NW_INVALID_MANDATORY_INFO_DENY_REASON              = 20, /**<  Invalid mandatory information. */
    E_QSER_NW_MSG_TYPE_NON_EXISTENT_DENY_REASON               = 21, /**<  Message type non-existent or not implemented. */
    E_QSER_NW_INFO_ELEMENT_NON_EXISTENT_DENY_REASON           = 22, /**<  Message type not compatible with the protocol state. */
    E_QSER_NW_CONDITIONAL_IE_ERR_DENY_REASON                  = 23, /**<  Conditional IE error. */
    E_QSER_NW_MSG_INCOMPATIBLE_PROTOCOL_STATE_DENY_REASON     = 24, /**<  Message not compatible with the protocol state. */
    E_QSER_NW_PROTOCOL_ERROR_DENY_REASON                      = 25, /**<  Unspecified protocol error. */
}E_QSER_NW_DENY_REASON_TYPE_T;


typedef enum 
{
    E_QSER_NW_SERVICE_NONE     = 0x0000,  /**<  Not registered or no data. */
    E_QSER_NW_SERVICE_LIMITED  = 0x0001,  /**<  Registered; emergency service only. */
    E_QSER_NW_SERVICE_FULL     = 0x0002,  /**<  Registered, full service. */
}E_QSER_NW_SERVICE_TYPE_T;

typedef struct 
{
    E_QSER_NW_TECH_DOMAIN_TYPE_T  tech_domain;        /**<   Technology, used to determine the structure type  tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/
    E_QSER_NW_RADIO_TECH_TYPE_T   radio_tech;         /**<   Radio technology; see #nw_radio_tech_t_v01.*/
    E_QSER_NW_ROAM_STATE_TYPE_T   roaming;            /**<   0 -- Off, 1 -- Roaming (3GPP2 has extended values).*/
    E_QSER_NW_DENY_REASON_TYPE_T  deny_reason;        /**<   Set when registration state is #nw_deny_reason_t_v01.*/
    E_QSER_NW_SERVICE_TYPE_T      registration_state; /**<   Registration state.*/
}QSER_NW_COMMON_REG_INFO_T;


typedef struct 
{
    E_QSER_NW_TECH_DOMAIN_TYPE_T  tech_domain;        /**<   Technology, used to determine the structure type  tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/
    E_QSER_NW_RADIO_TECH_TYPE_T   radio_tech;         /**<   Radio technology; see #nw_radio_tech_t_v01.*/
    char                            mcc[3+1];           /**<   Mobile country code.*/
    char                            mnc[3+1];           /**<   Mobile network code.*/
    E_QSER_NW_ROAM_STATE_TYPE_T   roaming;            /**<   0 -- Off, 1 -- Roaming (3GPP2 has extended values).*/
    uint8_t                         forbidden;          /**<   Forbidden: 0 -- No, 1 -- Yes.*/
    uint32_t                        cid;                /**<   Cell ID for the registered 3GPP system.*/
    uint16_t                        lac;                /**<   Locatin area code for the registered 3GPP system.*/
    uint16_t                        psc;                /**<   Primary scrambling code (WCDMA only); 0 -- None.*/
    uint16_t                        tac;                /**<   Tracking area code information for LTE.*/
}QSER_NW_3GPP_REG_INFO_T;


typedef struct 
{
    E_QSER_NW_TECH_DOMAIN_TYPE_T  tech_domain;        /**<   Technology, used to determine structure type  tech: 0 -- None, 1 -- 3GPP, 2 -- 3GPP2.*/
    E_QSER_NW_RADIO_TECH_TYPE_T   radio_tech;         /**<   Radio technology; see #nw_radio_tech_t_v01.*/
    char                            mcc[3+1];           /**<   Mobile country code.*/
    char                            mnc[3+1];           /**<   Mobile network code.*/
    E_QSER_NW_ROAM_STATE_TYPE_T   roaming;            /**<   Roaming status; see #nw_roam_state_t_v01.*/
    uint8_t                         forbidden;          /**<   Forbidden: 0 -- No, 1 -- Yes.*/
    uint8_t                         inPRL;              /**<   0 -- Not in PRL, 1 -- In PRL.*/
    uint8_t                         css;                /**<   Concurrent services supported: 0 -- No, 1 -- Yes.*/
    uint16_t                        sid;                /**<   CDMA system ID.*/
    uint16_t                        nid;                /**<   CDMA network ID.*/
    uint16_t                        bsid;               /**<   Base station ID. @newpagetable */
}QSER_NW_3GPP2_REG_INFO_T;

/** Gets the status associated with the connection of \<id\>. */
typedef struct 
{
    uint8_t                         voice_registration_valid;               /**< Must be set to TRUE if voice_registration is being passed. */
    QSER_NW_COMMON_REG_INFO_T     voice_registration;                     /**<   Voice registration. */

    uint8_t                         data_registration_valid;                /**< Must be set to TRUE if data_registration is being passed. */
    QSER_NW_COMMON_REG_INFO_T     data_registration;                      /**<   Data registration. */

    uint8_t                         voice_registration_details_3gpp_valid;  /**< Must be set to TRUE if voice_registration_details_3gpp is being passed. */
    QSER_NW_3GPP_REG_INFO_T       voice_registration_details_3gpp;        /**<   Voice registration details for 3GPP. */

    uint8_t                         data_registration_details_3gpp_valid;   /**< Must be set to TRUE if data_registration_details_3gpp is being passed. */
    QSER_NW_3GPP_REG_INFO_T       data_registration_details_3gpp;         /**<   Data registration details for 3GPP. */

    uint8_t                         voice_registration_details_3gpp2_valid; /**< Must be set to TRUE if voice_registration_details_3gpp2 is being passed. */
    QSER_NW_3GPP2_REG_INFO_T      voice_registration_details_3gpp2;       /**<   Voice registration details for 3GPP2. */

    uint8_t                         data_registration_details_3gpp2_valid;  /**< Must be set to TRUE if data_registration_details_3gpp2 is being passed. */
    QSER_NW_3GPP2_REG_INFO_T      data_registration_details_3gpp2;        /**<   Data registration details for 3GPP2. */
}QSER_NW_REG_STATUS_INFO_T;

typedef enum 
{
    E_QSER_NW_IMS_SERVICE_NONE     = 0x0000,  /**<  Not registered */
    E_QSER_NW_IMS_SERVICE_REGISTERED     = 0x0001,  /**<  Registered*/
}E_QSER_NW_IMS_SERVICE_TYPE_T;

typedef struct 
{   
    E_QSER_NW_IMS_SERVICE_TYPE_T      registration_state; /**<   Registration state.*/
}QSER_NW_IMS_REG_STATUS_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates received signal strength. A signed value; -125 or lower indicates no signal.*/
}QSER_NW_GSM_SIGNAL_INFO_T;


typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}QSER_NW_WCDMA_SIGNAL_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec.    a signed value; -125 or lower indicates no signal.*/
    int8_t      rscp;       /**<   RSCP in dBm.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
    int8_t      sinr;       /**<   Measured SINR in dB. @newpagetable */
}QSER_NW_TDSCDMA_SIGNAL_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Ec. A signed value; -125 or lower indicates no signal.*/
    int8_t      rsrq;       /**<   RSRQ value in dB (signed integer value), as measured by L1. Range: -3 to -20 (-3 equals -3 dB, -20 equals -20 dB).*/
    int16_t      rsrp;       /**<   Current RSRP in dBm, as measured by L1. Range: -44 to -140 (-44 equals -44 dBm, -140 equals -140 dBm).*/
    int16_t      snr;        /**<   SNR level as a scaled integer in units of 0.1 dB; e.g., -16 dB has a value of -160 and 24.6 dB has a value of 246.*/
}QSER_NW_LTE_SIGNAL_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Power (AGC) + Ec/Io. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
}QSER_NW_CDMA_SIGNAL_INFO_T;

typedef struct 
{
    int8_t      rssi;       /**<   RSSI in dBm. Indicates forward link pilot Power (AGC) + Ec/Io. A signed value; -125 or lower indicates no signal.*/
    int16_t     ecio;       /**<   Ec/Io value representing negative 0.5 dB increments, e.g., 2 equals -1 dbm.*/
    int8_t      sinr;       /**<   SINR level.*/
    int32_t     io;         /**<   Received IO in dBm. */
}QSER_NW_HDR_SIGNAL_INFO_T;

typedef struct
{
    int16_t     ssRsrp;   /* SS(Synchronization Signal) reference signal received power, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [44, 140], INT_MAX means invalid/unreported.*/
    int16_t     ssRsrq;   /* SS reference signal received quality, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [3, 20], INT_MAX means invalid/unreported.*/
    int16_t     ssSinr;   /* SS signal-to-noise and interference ratio.
                      * Reference: 3GPP TS 38.215 section 5.1.*, 3GPP TS 38.133 section 10.1.16.1.
                      * Range [-23, 40], INT_MAX means invalid/unreported.*/
    int16_t     csiRsrp;  /* CSI reference signal received power, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [44, 140], INT_MAX means invalid/unreported.*/
    int16_t     csiRsrq;  /* CSI reference signal received quality, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [3, 20], INT_MAX means invalid/unreported.*/
    int16_t     csiSinr;  /* CSI signal-to-noise and interference ratio.
                      * Reference: 3GPP TS 138.215 section 5.1.*, 3GPP TS 38.133 section 10.1.16.1.
                      * Range [-23, 40], INT_MAX means invalid/unreported.*/
}QSER_NW_NR_SIGNAL_INFO_T;


/** Gets signal strength information. */
typedef struct 
{
    uint8_t                         gsm_sig_info_valid;         /**< Must be set to TRUE if gsm_sig_info is being passed. */
    QSER_NW_GSM_SIGNAL_INFO_T     gsm_sig_info;               /**<   GSM signal information. */
    uint8_t                         wcdma_sig_info_valid;       /**< Must be set to TRUE if wcdma_sig_info is being passed. */
    QSER_NW_WCDMA_SIGNAL_INFO_T   wcdma_sig_info;             /**<   WCDMA signal information. */
    uint8_t                         tdscdma_sig_info_valid;     /**< Must be set to TRUE if tdscdma_sig_info is being passed. */
    QSER_NW_TDSCDMA_SIGNAL_INFO_T tdscdma_sig_info;           /**<   TDSCDMA signal information. */
    uint8_t                         lte_sig_info_valid;         /**< Must be set to TRUE if lte_sig_info is being passed. */
    QSER_NW_LTE_SIGNAL_INFO_T     lte_sig_info;               /**<   LTE signal information. */
    uint8_t                         cdma_sig_info_valid;        /**< Must be set to TRUE if cdma_sig_info is being passed. */
    QSER_NW_CDMA_SIGNAL_INFO_T    cdma_sig_info;              /**<   CDMA signal information. */
    uint8_t                         hdr_sig_info_valid;         /**< Must be set to TRUE if hdr_sig_info is being passed. */
    QSER_NW_HDR_SIGNAL_INFO_T     hdr_sig_info;               /**<   HDR signal information. */
    uint8_t                         nr_sig_info_valid;
    QSER_NW_NR_SIGNAL_INFO_T      nr_sig_info;
}QSER_NW_SIGNAL_STRENGTH_INFO_T;





/* @bridef Callback function registered to QSER_NW_AddRxMsgHandler 
 * map of ind_flag and ind_msg_buf as bellow :
 *  NW_IND_VOICE_REG_EVENT_IND_FLAG : QSER_NW_VOICE_REG_EVENT_IND_T
 *  NW_IND_DATA_REG_EVENT_IND_FLAG : QSER_NW_DATA_REG_EVENT_IND_T
 *  NW_IND_SIGNAL_STRENGTH_EVENT_IND_FLAG : QSER_NW_SINGNAL_EVENT_IND_T
 *  NW_IND_CELL_ACCESS_STATE_CHG_EVENT_IND_FLAG : QSER_NW_CELL_ACCESS_STATE_EVENT_IND_T
 *  NW_IND_NITZ_TIME_UPDATE_EVENT_IND_FLAG : QSER_NW_NITZ_TIME_EVENT_IND_T 
 *  NW_IND_IMS_REG_EVENT_IND_FLAG : NULL 
 * */
typedef void (*QSER_NW_RxMsgHandlerFunc_t)(
    nw_client_handle_type h_nw, 
    uint32_t ind_flag, 
    void                  *ind_msg_buf, 
    uint32_t              ind_msg_len, 
    void                  *contextPtr
);


/** Indication message; Indication for the corresponding registered event flag NW_IND_VOICE_REG_EVENT_IND_FLAG */
typedef struct {

  uint8_t                         registration_valid;               /**< Must be set to TRUE if voice_registration is being passed. */
  QSER_NW_COMMON_REG_INFO_T     registration;                     /**< Voice registration. */

  uint8_t                         registration_details_3gpp_valid;  /**< Must be set to TRUE if voice_registration_details_3gpp is being passed. */
  QSER_NW_3GPP_REG_INFO_T       registration_details_3gpp;        /**< Voice registration details for 3GPP. */

  uint8_t                         registration_details_3gpp2_valid; /**< Must be set to TRUE if voice_registration_details_3gpp2 is being passed. */
  QSER_NW_3GPP2_REG_INFO_T      registration_details_3gpp2;       /**< Voice registration details for 3GPP2. */
}QSER_NW_VOICE_REG_EVENT_IND_T; 

/** Indication message; Indication for the corresponding registered event flag NW_IND_DATA_REG_EVENT_IND_FLAG */
typedef struct {

  uint8_t                         registration_valid;               /**< Must be set to TRUE if data_registration is being passed. */
  QSER_NW_COMMON_REG_INFO_T     registration;                     /**< Data registration. */
  
  uint8_t                         registration_details_3gpp_valid;  /**< Must be set to TRUE if data_registration_details_3gpp is being passed. */
  QSER_NW_3GPP_REG_INFO_T       registration_details_3gpp;        /**< Data registration details for 3GPP. */

  uint8_t                         registration_details_3gpp2_valid; /**< Must be set to TRUE if data_registration_details_3gpp2 is being passed. */
  QSER_NW_3GPP2_REG_INFO_T      registration_details_3gpp2;       /**< Data registration details for 3GPP2. */
}QSER_NW_DATA_REG_EVENT_IND_T;


/** Indication message; Indication for the corresponding registered event flag NW_IND_SIGNAL_STRENGTH_EVENT_IND_FLAG */
typedef struct {
    uint8_t gsm_sig_info_valid;                       /**< Must be set to TRUE if gsm_sig_info is being passed. */
    QSER_NW_GSM_SIGNAL_INFO_T       gsm_sig_info;   /**< GSM singal information. */

    uint8_t wcdma_sig_info_valid;                     /**< Must be set to TRUE if wcdma_sig_info is being passed. */
    QSER_NW_WCDMA_SIGNAL_INFO_T wcdma_sig_info;     /**< WCDMA singal information. */

    uint8_t tdscdma_sig_info_valid;                   /**< Must be set to TRUE if tdscdma_sig_info is being passed. */
    QSER_NW_TDSCDMA_SIGNAL_INFO_T tdscdma_sig_info; /**< TDSCDMA singal information. */

    uint8_t lte_sig_info_valid;                       /**< Must be set to TRUE if lte_sig_info is being passed. */
    QSER_NW_LTE_SIGNAL_INFO_T lte_sig_info;         /**< LTE singal information. */

    uint8_t cdma_sig_info_valid;                      /**< Must be set to TRUE if cdma_sig_info is being passed. */
    QSER_NW_CDMA_SIGNAL_INFO_T cdma_sig_info;       /**< CDMA singal information. */

    uint8_t hdr_sig_info_valid;                       /**< Must be set to TRUE if hdr_sig_info is being passed. */
    QSER_NW_HDR_SIGNAL_INFO_T hdr_sig_info;         /**< HDR singal information. */
    
    uint8_t                         nr_sig_info_valid;
    QSER_NW_NR_SIGNAL_INFO_T      nr_sig_info;
}QSER_NW_SINGNAL_EVENT_IND_T;

typedef enum 
{
    E_QSER_NW_RF_MODE_CFUN_0 = 0,    /**<  CFUN 0. */
    E_QSER_NW_RF_MODE_CFUN_1 = 1,    /**<  CFUN 1. */
    E_QSER_NW_RF_MODE_FLIGHT = 4,    /**<  Flight Mode, CFUN 4. */
}E_QSER_NW_RF_MODE_TYPE_T;

int qser_nw_client_init(nw_client_handle_type  *ph_nw);

int qser_nw_client_deinit(nw_client_handle_type  h_nw);

int qser_nw_set_config
(
    nw_client_handle_type       h_nw,
    QSER_NW_CONFIG_INFO_T     *pt_info 
);

int qser_nw_set_ims_enable
(
    nw_client_handle_type h_nw,
    E_QSER_NW_IMS_MODE_TYPE_T ims_mode
);

int qser_nw_set_oos_config
(
    nw_client_handle_type       h_nw,
    QSER_NW_OOS_CONFIG_INFO_T     *pt_info 
);

int qser_nw_get_oos_config
(
    nw_client_handle_type       h_nw,
    QSER_NW_OOS_CONFIG_INFO_T     *pt_info 
);

int qser_nw_event_register
(
    nw_client_handle_type       h_nw,
    uint32_t                    bitmask // bit OR of NW_IND_xxxx_EVENT_ON
);

int qser_nw_get_operator_name
(
    nw_client_handle_type           h_nw,
    QSER_NW_OPERATOR_NAME_INFO_T  *pt_info //You should malloc this or may cause stack overflow
);

int qser_nw_get_reg_status
(
    nw_client_handle_type               h_nw,
    QSER_NW_REG_STATUS_INFO_T         *pt_info 
);

int qser_nw_get_ims_reg_status
(
    nw_client_handle_type h_nw,
    QSER_NW_IMS_REG_STATUS_INFO_T *pt_info
);

int qser_nw_get_signal_strength
(
    nw_client_handle_type               h_nw,
    QSER_NW_SIGNAL_STRENGTH_INFO_T    *pt_info
);

int qser_nw_add_rx_msg_handler
(
    nw_client_handle_type        h_nw,
    QSER_NW_RxMsgHandlerFunc_t handlerPtr, 
    void* contextPtr
);

int qser_nw_set_rf_mode
(
    nw_client_handle_type       h_nw,
    E_QSER_NW_RF_MODE_TYPE_T    rf_mode
);

int qser_nw_get_rf_mode
(
    nw_client_handle_type       h_nw,
    E_QSER_NW_RF_MODE_TYPE_T    *rf_mode
);


#ifdef __cplusplus
}
#endif
#endif//__QSER_NW_H__
