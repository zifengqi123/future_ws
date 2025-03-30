/*
*
* Data : 2022/10/20 11:24:09
* Author : LiuBin
*/
#ifndef _MBTK_INFO_API_H
#define _MBTK_INFO_API_H
#include <netinet/in.h>

#include "mbtk_type.h"
#include "mbtk_list.h"
#include "mbtk_log.h"
#include "mbtk_utils.h"
#include "mbtk_pdu_sms.h"

#ifndef UNUSED
#define UNUSED(a)    (void)(a)
#endif

// LOG_DEBUG_LEVEL 7
#define LOG LOGI

#define CELL_NUM_MAX 50
#define APN_STR_MAX 128
#if (defined(MBTK_AF_SUPPORT) || defined(MBTK_ALL_CID_SUPPORT))
#define MBTK_APN_CID_MIN 1
#else
// 2 - 7
#define MBTK_APN_CID_MIN 2
#endif
#define MBTK_APN_CID_MAX 7
#define MBTK_DATA_CALL_TIMEOUT_DEFAULT 10  // 10s

#define MBTK_APN_NAME_SIZE 150+1
#define MBTK_APN_USERNAME_SIZE 127+1
#define MBTK_APN_PASSWORD_SIZE 127+1
#define MBTK_APN_TYPE_SIZE 127+1



typedef void (*mbtk_info_callback_func)(const void* data, int data_len);

typedef enum {
    MBTK_TIME_TYPE_CELL = 0,
    MBTK_TIME_TYPE_NTP,
    MBTK_TIME_TYPE_USER
} mbtk_time_type_enum;

typedef enum {
    MBTK_ACTIVE,
    MBTK_HELD,
    MBTK_DIALING,
    MBTK_ALERTING,
    MBTK_INCOMING,
    MBTK_WAITING,
    MBTK_OFFERING,
} mbtk_call_stat_enum;

/*
    0 : GSM only
    1 : UMTS only
    2 : GSM/UMTS(auto)
    3 : GSM/UMTS(GSM preferred)
    4 : GSM/UMTS(UMTS preferred)
    5 : LTE only
    6 : GSM/LTE(auto)
    7 : GSM/LTE(GSM preferred)
    8 : GSM/LTE(LTE preferred)
    9 : UMTS/LTE(auto)
    10 : UMTS/LTE(UMTS preferred)
    11 : UMTS/LTE(LTE preferred)
    12 : GSM/UMTS/LTE(auto)
    13 : GSM/UMTS/LTE(GSM preferred)
    14 : GSM/UMTS/LTE(UMTS preferred)
    15 : GSM/UMTS/LTE(LTE preferred)
*/
typedef enum
{
    MBTK_NET_PREF_GSM_ONLY = 0,
    MBTK_NET_PREF_UMTS_ONLY,
    MBTK_NET_PREF_GSM_UMTS_AUTO,
    MBTK_NET_PREF_GSM_UMTS_GSM_PREF,
    MBTK_NET_PREF_GSM_UMTS_UMTS_PREF,
    MBTK_NET_PREF_LTE_ONLY,
    MBTK_NET_PREF_GSM_LTE_AUTO,
    MBTK_NET_PREF_GSM_LTE_GSM_PREF,
    MBTK_NET_PREF_GSM_LTE_LTE_PREF,
    MBTK_NET_PREF_UMTS_LTE_AUTO,
    MBTK_NET_PREF_UMTS_LTE_UMTS_PREF,
    MBTK_NET_PREF_UMTS_LTE_LTE_PREF,
    MBTK_NET_PREF_GSM_UMTS_LTE_AUTO,
    MBTK_NET_PREF_GSM_UMTS_LTE_GSM_PREF,
    MBTK_NET_PREF_GSM_UMTS_LTE_UMTS_PREF,
    MBTK_NET_PREF_GSM_UMTS_LTE_LTE_PREF
} mbtk_net_pref_enum;

typedef enum
{
    MBTK_CELL_TYPE_GSM = 0,
    MBTK_CELL_TYPE_UMTS,
    MBTK_CELL_TYPE_LTE
} mbtk_cell_type_enum;

typedef struct
{
    int client_fd;
    pthread_t read_thread_id;
    int exit_fd[2];
    bool is_waitting;
    pthread_cond_t cond;
    pthread_mutex_t mutex;

    pthread_mutex_t send_mutex;

    // Temp response data.
    uint16 info_err;
    uint16 data_len;
    void *data;

    //mbtk wyq for server_ready_status add start
    char server_ready_status;
    //mbtk wyq for server_ready_status add end

    mbtk_info_callback_func net_state_cb;
    mbtk_info_callback_func call_state_cb;
    mbtk_info_callback_func sms_state_cb;
    mbtk_info_callback_func radio_state_cb;
    mbtk_info_callback_func sim_state_cb;
    mbtk_info_callback_func pdp_state_cb;
    //add signal by xr
    mbtk_info_callback_func signal_state_cb;
} mbtk_info_handle_t;

typedef struct
{
    /*
    0: automatic
    1: manual
    */
    uint8 net_sel_mode;
    /*
    0: GSM
    1: GSM Compact
    2: UTRAN
    3: GSM w/EGPRS
    4: UTRAN w/HSDPA
    5: UTRAN w/HSUPA
    6: UTRAN w/HSDPA and HSUPA
    7: E-UTRAN
    8: UTRAN HSPA+
    0xFF: Unused
    */
    uint8 net_type;
    //uint8 plmn[10]; // 46000
    /*
    0: unknown
    1: available
    2: current
    3: forbidden
    */
    uint8 net_state;
    uint32 plmn;
} __attribute__((packed)) mbtk_net_info_t;

typedef struct
{
    uint8 net_pref;     // mbtk_net_pref_enum
    uint16 gsm_band;    // mbtk_gsm_band_enum
    uint16 umts_band;   // mbtk_umts_band_enum
    uint32 tdlte_band;  // mbtk_tdlte_band_enum
    uint32 fddlte_band; // mbtk_fddlte_band_enum
    uint32 lte_ext_band; // mbtk_lte_ext_band_enum
} __attribute__((packed)) mbtk_band_info_t;

typedef struct
{
    uint8 type; // mbtk_radio_technology_enum
    uint8 rssi; // 0: 113 dBm or less
                // 1: 111 dBm
                // 2��30: 109��53 dBm
                // 31: 51 dBm or greater
                // 99: not known or not detectable
    uint8 rxlev;// 0:rssi < -110 dBm
                // 1: -110 dBm �� rssi < -109 dBm
                // 2: -109 dBm �� rssi < -108 dBm
                // ......
                // 61: -50 dBm �� rssi < -49 dBm
                // 62: -49 dBm �� rssi < -48 dBm
                // 63: -48 dBm �� rssi
                // 99: not known or not detectable
    uint8 ber;  // 0...7 as RXQUAL values in the table in 3GPP TS 45.008 [20] subclause 8.2.4
                // 99 not known or not detectable
    uint8 rscp; // 0: rscp < -120 dBm
                // 1: -120 dBm �� rscp < -119 dBm
                // 2: -119 dBm �� rscp < -118 dBm
                // ......
                // 94: -27 dBm �� rscp < -26 dBm
                // 95: -26 dBm �� rscp < -25 dBm
                // 96: - 25 dBm �� rscp
                // 255: not known or not detectable
    uint8 ecno; // 0: Ec/Io < -24 dB
                // 1: -24 dB �� Ec/Io < -23.5 dB
                // 2: -23.5 dB �� Ec/Io < -23 dB
                // ......
                // 47: -1 dB �� Ec/Io < -0.5 dB
                // 48: -0.5 dB �� Ec/Io < 0 dB
                // 49: 0 dB �� Ec/Io
                // 255: not known or not detectable
    uint8 rsrq; // 0: rsrq < -19.5 dB
                // 1: -19.5 dB �� rsrq < -19 dB
                // 2: -19 dB �� rsrq < -18.5 dB
                // ......
                // 32: -4 dB �� rsrq < -3.5 dB
                // 33: -3.5 dB �� rsrq < -3 dB
                // 34: -3 dB �� rsrq
                // 255: not known or not detectable
    uint8 rsrp; // 0: rsrp < -140 dBm
                // 1: -140 dBm �� rsrp < -139 dBm
                // 2: -139 dBm �� rsrp < -138 dBm
                // ......
                // 95: -46 dBm �� rsrp < -45 dBm
                // 96: -45 dBm �� rsrp < -44 dBm
                // 97: -44 dBm �� rsrp
                // 255: not known or not detectable
} __attribute__((packed)) mbtk_signal_info_t;

typedef struct
{
    uint8 call_state;// mbtk_net_reg_state_enum
    uint8 data_state;// mbtk_net_reg_state_enum
    uint8 ims_state;// mbtk_net_reg_state_enum
    uint8 type; // mbtk_radio_technology_enum
    uint16 lac;
    uint32 ci;
} __attribute__((packed)) mbtk_net_reg_info_t;

typedef struct
{
    uint8 call_wait;
    uint8 dir1;
    uint8 dir;
    uint8 state;
    uint8 mode;
    uint8 mpty;
    char phone_number[100];
    uint8 type;
    uint8 pas;
    uint8 disconnected_id;
} __attribute__((packed)) mbtk_call_info_t;

typedef struct
{
    char character;
    uint32 duration;
} __attribute__((packed)) mbtk_call_dtmf_info_t;

typedef enum {
    MBTK_SIM_ABSENT = 0,
    MBTK_SIM_NOT_READY = 1,
    MBTK_SIM_READY = 2,
    MBTK_SIM_PIN = 3,
    MBTK_SIM_PUK = 4,
    MBTK_SIM_NETWORK_PERSONALIZATION = 5
} mbtk_sim_state_enum;

typedef enum {
    MBTK_IP_TYPE_IP,
    MBTK_IP_TYPE_IPV6,
    MBTK_IP_TYPE_IPV4V6,
    MBTK_IP_TYPE_PPP
} mbtk_ip_type_enum;

typedef enum
{
    MBTK_NET_TYPE_UNKNOWN,
    MBTK_NET_TYPE_GSM,
    MBTK_NET_TYPE_UMTS,
    MBTK_NET_TYPE_LTE
} mbtk_net_type_enum;

typedef enum {
    MBTK_CLCC = 1,
    MBTK_CPAS,
    MBTK_DISCONNECTED,
} mbtk_call_enum;

typedef struct
{
    // LTE server cell: tac, PCI, dlEuarfcn, ulEuarfcn, band
    // LTE cell: phyCellId,euArfcn,rsrp,rsrq
    // WCDMA server cell: lac, ci, arfcn
    // WCDMA cell: lac, ci, arfcn
    // GSM server cell: lac, ci, arfcn, bsic
    // GSM cell:
    uint32 value1;
    uint32 value2;
    uint32 value3;
    uint32 value4;
    uint32 value5;
	uint32 value6;
    uint32 value7;
    uint32 value8;
    uint32 value9;
    uint32 value10;
} __attribute__((packed)) mbtk_cell_info_t;

typedef enum {
    MBTK_CALL_RADY,                 //MT allows commands from TA/TE
    MBTK_CALL_UNAVAILABLE,          //MT does not allow commands from TA/TE
    MBTK_CALL_UNKNOWN,              //MT is not guaranteed to respond to instructions
    MBTK_CALL_RINGING,              //MT is ready for commands from TA/TE, but the ringer is active
    MBTK_CALL_PROGRESS,             //MT is ready for commands from TA/TE, but a call is in progress
    MBTK_CALL_ASLEEP,               //MT is unable to process commands from TA/TE because it is in a low functionality state
    MBTK_CALL_ACTIVE,
} mbtk_call_pas_enum;

typedef enum {
    MBTK_RADIO_STATE_UNKNOWN,
    MBTK_RADIO_STATE_ON,
    MBTK_RADIO_STATE_OFF
} mbtk_radio_state_enum;

typedef enum {
    MBTK_DATA_CALL_STOP = 0,    // Stop data call.
    MBTK_DATA_CALL_START,       // Start data call.
    MBTK_DATA_CALL_STATE        // Get data call state
} mbtk_data_call_type_enum;
/*
0: not registered, MT is not currently searching a new operator to register to
1: registered, home network
2: not registered, but MT is currently searching a new operator to register to
3: registration denied
4: unknown
5: registered, roaming
6: registered, home network, SMS-only (applicable only when AcT is E-UTRAN)
7: registered, roaming, SMS-only (applicable only when AcT is E-UTRAN)
8: attached for emergency bearer services only (not applicable)
9: registered for ��CSFB not preferred��,home network(applicable only when AcT is E-UTRAN)
10: registered for ��CSFB not preferred��,roaming(applicable only when AcT is E-UTRAN)
11: only emergency services are available
*/
typedef enum
{
    MBTK_NET_REG_STATE_NON = 0,
    MBTK_NET_REG_STATE_HOME,
    MBTK_NET_REG_STATE_SEARCHING,
    MBTK_NET_REG_STATE_DENIED,
    MBTK_NET_REG_STATE_UNKNOWN,
    MBTK_NET_REG_STATE_ROAMING,
    MBTK_NET_REG_STATE_SMS_ONLY,
    MBTK_NET_REG_STATE_ROAMING_SMS,
    MBTK_NET_REG_STATE_ATTACHED_EMERGENCY,
    MBTK_NET_REG_STATE_CSFB_HOME,
    MBTK_NET_REG_STATE_CSFB_ROAMING,
    MBTK_NET_REG_STATE_EMERGENCY_ONLY
} mbtk_net_reg_state_enum;

typedef struct {
    int cid; // 2 - 7
    mbtk_ip_type_enum ip_type; // mbtk_ip_type_enum
    uint8 apn[APN_STR_MAX];
    uint8 user[APN_STR_MAX];
    uint8 pass[APN_STR_MAX];
    uint8 auth[APN_STR_MAX];
} mbtk_apn_info_t;

typedef struct {
    bool valid;
    uint32 IPAddr;
    uint32 PrimaryDNS;
    uint32 SecondaryDNS;
    uint32 GateWay;
    uint32 NetMask;
} __attribute__((packed)) mbtk_ipv4_info_t;

typedef struct {
    bool valid;
    uint32 IPV6Addr[4];
    uint32 PrimaryDNS[4];
    uint32 SecondaryDNS[4];
    uint32 GateWay[4];
    uint32 NetMask[4];
} __attribute__((packed)) mbtk_ipv6_info_t;

typedef enum
{
    MBTK_DEV_MODEM_MIN_FUN,                                   //Modem 最小功能
    MBTK_DEV_MODEM_FULL_FUN,                                  //Modem 全功能
    MBTK_DEV_MODEM_DISABLE_RECEIVE_RF_CIRCUITS = 3,           //Modem 禁用射频接收电路
    MBTK_DEV_MODEM_DISABLE_TRANSMIT_AND_RECEIVE_RF_CIRCUITS,  //Modem禁用射频发射和接收电路
    MBTK_DEV_MODEM_DISABLE_SIM,                               //Modem 禁用(U)SIM 卡
    MBTK_DEV_MODEM_TURN_OFF_FULL_SECONDARY_RECEIVE,           //Modem 完全禁用辅助接收
}MBTK_DEV_MODEM_FUNCTION;

/*
0: GSM
1: GSM Compact
2: UTRAN
3: GSM w/EGPRS
4: UTRAN w/HSDPA
5: UTRAN w/HSUPA
6: UTRAN w/HSDPA and HSUPA
7: E-UTRAN
8: UTRAN HSPA+
*/
typedef enum {
    MBTK_RADIO_TECH_GSM = 0,
    MBTK_RADIO_TECH_GSM_COMPACT,
    MBTK_RADIO_TECH_UTRAN,
    MBTK_RADIO_TECH_GSM_EGPRS,
    MBTK_RADIO_TECH_UTRAN_HSDPA,
    MBTK_RADIO_TECH_UTRAN_HSUPA,
    MBTK_RADIO_TECH_UTRAN_HSDPA_HSUPA,
    MBTK_RADIO_TECH_E_UTRAN,      // LTE
    MBTK_RADIO_TECH_UTRAN_HSPA
} mbtk_radio_technology_enum;

typedef struct
{
    MBTK_DEV_MODEM_FUNCTION fun;
    int rst;
} mbtk_modem_info_t;

typedef enum {
    MBTK_SIM = 0,
    MBTK_USIM = 1,
    MBTK_TEST_SIM = 2,
    MBTK_TEST_USIM = 3,
    MBTK_UNKNOWN = 4,
} mbtk_sim_card_type_enum;

typedef struct
{
    uint8_t p1_retry;
    uint8_t p2_retry;
    uint8_t puk1_retry;
    uint8_t puk2_retry;
} mbtk_pin_puk_last_times;

typedef struct
{
    uint8_t format;
    char plmn_name[16];
}MBTK_SIM_PLMN_INFO_C;  //string type


typedef struct
{
    uint8_t count;
    uint8_t format;
    MBTK_SIM_PLMN_INFO_C mbtk_plmn_name[24];
} mbtk_plmn_info;

typedef struct
{
 uint8_t old_pin_value[16];
 uint8_t new_pin_value[16];
} mbtk_change_pin_info;

typedef struct
{
 uint8_t pin_value[16];
 uint8_t puk_value[16];
} mbtk_unlock_pin_info;

typedef struct
{
 uint8_t pin_value[16];
 uint8_t enable;
} mbtk_enable_pin_info;

typedef struct
{
    int sim;
    int sim_card_type;
    mbtk_pin_puk_last_times ql_last_times;
} mbtk_sim_card_info;


typedef struct{
    uint8_t         mode;
    uint32_t        oosPhase[3];   //单位为秒
} mbtk_oos_info;

typedef struct
{
    int8 ther;
} mbtk_thermal_info_t;

/**************led enum*********/
typedef enum
{
    MBTK_LED_TYPE_NET,       //set net_led
    MBTK_LED_TYPE_STATUS     //set status_led
}mbtk_led_type;

typedef enum
{
    MBTK_LED_STATUS_CLOSE,       //close led
    MBTK_LED_STATUS_OPEN     //open led
}mbtk_led_status;
/**************led enum*********/

/**************qser data_call*****************/
typedef enum {
    MBTK_APN_REQ_TYPE_SET = 0,   // set apn req
    MBTK_APN_REQ_TYPE_ADD        // add apn req
}mbtk_apn_req_type_enum;

typedef enum {
	MBTK_APN_AUTH_PROTO_DEFAULT = 0,
	MBTK_APN_AUTH_PROTO_NONE,
	MBTK_APN_AUTH_PROTO_PAP,
	MBTK_APN_AUTH_PROTO_CHAP,
#if 0
	MBTK_APN_AUTH_PROTO_PAP_CHAP,
    //NOT SUPPORT
#endif
} mbtk_apn_auth_proto_enum;

typedef struct {
	int cid;                /*!< UMTS/CDMA profile ID. range: 0 - 7*/
	mbtk_ip_type_enum ip_type;               /*!< Packet Data Protocol (PDP) type specifies the type of data payload
	                                             exchanged over the airlink when the packet data session is
	                                             established with this profile. */ 
	mbtk_apn_req_type_enum req_type;          /*!< apn req type*/
	mbtk_apn_auth_proto_enum auth_proto;         /*!< Authentication Protocol. */
	uint8 apn_name[MBTK_APN_NAME_SIZE];        /*!< A string parameter that is a logical name used to select the GGSN
	                                             and external packet data network. */ 
	uint8 user_name[MBTK_APN_USERNAME_SIZE];    /*!< Username used during data network authentication. */
	uint8 user_pass[MBTK_APN_PASSWORD_SIZE];    /*!< Password to be used during data network authentication. */
    uint8 apn_type[MBTK_APN_TYPE_SIZE];
} mbtk_qser_apn_info_s;
/**************qser data_call*****************/

mbtk_info_handle_t* mbtk_info_handle_get();

int mbtk_info_handle_free(mbtk_info_handle_t** handle);

/*
* Get platform version.
*/
int mbtk_version_get(mbtk_info_handle_t* handle, void *version);

/*
* Get platform model.
*/
int mbtk_model_get(mbtk_info_handle_t* handle, void *model);

/*
* Get platform IMEI.
*/
int mbtk_imei_get(mbtk_info_handle_t* handle, void *imei);

/*
* Get platform SN.
*/
int mbtk_sn_get(mbtk_info_handle_t* handle, void *sn);

/*
* Get platform MEID.
*/
int mbtk_meid_get(mbtk_info_handle_t* handle, void *meid);

/*
* Return VoLTE state.
*/
int mbtk_volte_state_get(mbtk_info_handle_t* handle, int *volte_state);

/*
* Return modem state.
*/
int mbtk_get_modem_fun(mbtk_info_handle_t* handle, int* fun);

/*
* Set modem state.
*/
int mbtk_set_modem_fun(mbtk_info_handle_t* handle, mbtk_modem_info_t *info);


/*
* Set VoLTE state.
*
* volte_state:
* 0 : Close VoLTE.
* 1 : Open VoLTE.
*
* Restarting takes effect after execution.
*/
int mbtk_volte_state_set(mbtk_info_handle_t* handle, int volte_state);

/*
* Get platform IMSI.
*/
int mbtk_imsi_get(mbtk_info_handle_t* handle, void *imsi);

/*
* Get platform ICCID.
*/
int mbtk_iccid_get(mbtk_info_handle_t* handle, void *iccid);

/*
* Get current phone number.
*/
int mbtk_phone_number_get(mbtk_info_handle_t* handle, void *phone_number);

/*
* Get available network.
*/
int mbtk_available_net_get(mbtk_info_handle_t* handle, list_node_t **net_list);

/*
* Set network select mode. (+COPS=...)
*/
int mbtk_net_sel_mode_set(mbtk_info_handle_t* handle, const mbtk_net_info_t *net);

/*
* Get network select mode. (+COPS?)
*/
int mbtk_net_sel_mode_get(mbtk_info_handle_t* handle, mbtk_net_info_t *net);

/*
* Get platform support bands.
*/
int mbtk_support_band_get(mbtk_info_handle_t* handle, mbtk_band_info_t *band);

/*
* Get platform current bands.
*/
int mbtk_current_band_get(mbtk_info_handle_t* handle, mbtk_band_info_t *band);

/*
* Set platform current bands.
*/
int mbtk_current_band_set(mbtk_info_handle_t* handle, const mbtk_band_info_t *band);

/*
* Get current cell infomation.
*/
int mbtk_cell_get(mbtk_info_handle_t* handle, mbtk_cell_type_enum *type, list_node_t **cell_list);

/*
* Set AT*CELL.
*/
int mbtk_cell_set(mbtk_info_handle_t* handle, char * info, char* response);

/*
* Get current APN informations.
*/
int mbtk_apn_get(mbtk_info_handle_t* handle, int *apn_num, mbtk_apn_info_t apns[]);

/*
* Set current QSER APN informations.
*/
int mbtk_qser_apn_set(mbtk_info_handle_t* handle, mbtk_qser_apn_info_s *apninfo, unsigned char *cid);

/*
* Set current QSER APN informations.
*/
int mbtk_qser_apn_get(mbtk_info_handle_t* handle, int *apn_num, mbtk_qser_apn_info_s apns[]);

/*
* Set current APN informations.
*/
int mbtk_apn_set(mbtk_info_handle_t* handle, int cid, mbtk_ip_type_enum ip_type, const void* apn_name,
                    const void *user_name, const void *user_pass, const void *auth);

/*
* Delete apn.
*/
int mbtk_apn_del(mbtk_info_handle_t* handle, unsigned char profile_idx);

/*
* Start data call.
*/
int mbtk_data_call_start(mbtk_info_handle_t* handle, int cid, int auto_conn_interval, bool boot_conn, int timeout);

/*
* Stop data call.
*/
int mbtk_data_call_stop(mbtk_info_handle_t* handle, int cid, int timeout);

/*
* Get data call state.
*/
int mbtk_data_call_state_get(mbtk_info_handle_t* handle, int cid, mbtk_ipv4_info_t *ipv4, mbtk_ipv6_info_t *ipv6);

/*
* Get current network signal.
*/
int mbtk_net_signal_get(mbtk_info_handle_t* handle, mbtk_signal_info_t *signal);

/*
* Get current network register information.
*/
int mbtk_net_reg_get(mbtk_info_handle_t* handle, mbtk_net_reg_info_t *reg);

/*
* Get current IMS enable or not.
*/
int mbtk_net_ims_get(mbtk_info_handle_t* handle, int* enable);

/*
* Set IMS enable or not. This function takes effect after starting the device.
*/
int mbtk_net_ims_set(mbtk_info_handle_t* handle, int enable);

/*
* Get current network IMS register state.
*/
int mbtk_net_ims_reg_state_get(mbtk_info_handle_t* handle, int* reg);

/*
* Get radio state.
*/
int mbtk_radio_state_get(mbtk_info_handle_t* handle, int *radio_state);

/*
* Set radio state.
*/
int mbtk_radio_state_set(mbtk_info_handle_t* handle, int radio_state);

/*
* Return sms cmgf.
*/
int mbtk_sms_cmgf_get(mbtk_info_handle_t* handle, int *volte_state);

/*
* Set sms cmgf.
*
* volte_state:
* 0 : PDU mode.
* 1 :  text mode.
*
* Restarting takes effect after execution.
*/
int mbtk_sms_cmgf_set(mbtk_info_handle_t* handle, int mode);

/*
* Set sms cmgs.
*
if PDU mode (+CMGF=0):
+CMGS=<length><CR>
PDU is given<ctrl-Z/ESC>

if text mode (+CMGF=1):
+CMGS=<da>[,<toda>]<CR>
text is entered<ctrl-Z/ESC>

* Restarting takes effect after execution.
*/
int mbtk_sms_cmgs_set(mbtk_info_handle_t* handle, char * cmgs, char *resp);

/*
* Set sms cmgw.
*
if text mode (+CMGF=1):
+CMGW=<oa/da>[,<tooa/toda>[,<stat>]]
<CR>
text is entered<ctrl-Z/ESC>
if PDU mode (+CMGF=0):
+CMGW=<length>[,<stat>]<CR>PDU is
given<ctrl-Z/ESC>

*/
int mbtk_sms_cmgw_set(mbtk_info_handle_t* handle, char * cmgw, char *resp);

/*
* Set sms cmgd.
*
* +CMGD=<index>[,<delflag>]
*
* Restarting takes effect after execution.
*/
int mbtk_sms_cmgd_set(mbtk_info_handle_t* handle, char * cmdg);

/*
* Set sms cmgl.
*
* AT+CMGL[=<stat>]
*
* Restarting takes effect after execution.
*/
int mbtk_sms_cmgl_set(mbtk_info_handle_t* handle, char * cmgl, char *resp);

/*
* Return sms csca.
*/
int mbtk_sms_csca_get(mbtk_info_handle_t* handle, char *buf);

/*
* Set sms csca.
*
* AT+CSCA=<number> [,<type>]
*
* Restarting takes effect after execution.
*/
int mbtk_sms_csca_set(mbtk_info_handle_t* handle, char * csca);

/*
* Set sms csmp.
*
* AT+CSMP=[<fo>[,<vp>[,<pid>[,<dcs>]]]]
*
* Restarting takes effect after execution.
*/
int mbtk_sms_csmp_set(mbtk_info_handle_t* handle, char * csmp);

/*
* Set sms cscb.
*
* AT+CSCB=<[<mode>[,<mids>[,<dcss>]]]>
*
* Restarting takes effect after execution.
*/
int mbtk_sms_cscb_set(mbtk_info_handle_t* handle, char * cscb);

/*
* Set sms cnmi.
*
at+cnmi=1,2

OK
if sending fails:
+CMS ERROR: <err>
*/
int mbtk_sms_cnmi_set(mbtk_info_handle_t* handle);

/*
* Set sms cmss.
*
+CMSS=<index>[,<da>[,<toda>]]

if sending successful:
+CMSS: <mr>
OK
if sending fails:
+CMS ERROR: <err>
*/
int mbtk_sms_cmss_set(mbtk_info_handle_t* handle, char * cmss, char *resp);

/*
* Return sms cmgf.
*/
int mbtk_sms_cpms_get(mbtk_info_handle_t* handle, char * mem);

/*
* Set sms cpms.
*
* AT+CPMS=<mem1>[,<mem2>[,<mem3>]]
*
* Restarting takes effect after execution.
*/
int mbtk_sms_cpms_set(mbtk_info_handle_t* handle, char * mem, char* response);

/*
* Set sms cm.
*
* +CMGR=<index>

if PDU mode (+CMGF=0) ��command successful:
+CMGR: <stat>,[<alpha>],<length><CR><LF><pdu>
OK
if text mode (+CMGF=1), command successful and SMS-DELIVER:
+CMGR:<stat>,<oa>,[<alpha>],<scts>[,<tooa>,<fo>,<pid>,<dcs
>, <sca>,<tosca>,<length>]<CR><LF><data>
OK
if text mode (+CMGF=1), command successful and SMS-SUBMIT:
+CMGR:
<stat>,<da>,[<alpha>][,<toda>,<fo>,<pid>,<dcs>,[<vp>],
<sca>,<tosca>,<length>]<CR><LF><data>
OK
otherwise:
+CMS ERROR: <err>
*
* Restarting takes effect after execution.
*/
int mbtk_sms_cmgr_set(mbtk_info_handle_t* handle, int index, char *resp);

/*
* Get sim state.
*/
int mbtk_sim_state_get(mbtk_info_handle_t* handle, mbtk_sim_state_enum *sim_state);

/*
* Get sim card type.
*/
int mbtk_sim_card_type_get(mbtk_info_handle_t* handle, mbtk_sim_card_type_enum *sim_card_type);

/*
* Get PIN’s number of remaining retry
*/
int mbtk_pin_last_num_get(mbtk_info_handle_t* handle, mbtk_pin_puk_last_times *last_times);

/*
* Get plmn list
*/
int mbtk_get_plmn_list(mbtk_info_handle_t* handle, mbtk_plmn_info *pin);

/*
* Get system temperature.
*
* type[IN]:
*   0: Soc temperature.
*   1: RF temperature.
* temp[OUT]:
*   temperature in celsius.
*/
int mbtk_temp_get(mbtk_info_handle_t* handle, int type, mbtk_thermal_info_t* temp);

/*
* Set sim power state.
* power:
* 0: Sim power off.
* 1: Sim power on.
*/
int mbtk_sim_power_set(int power);

/*
*enable PIN
*/
int mbtk_enable_pin(mbtk_info_handle_t* handle, mbtk_enable_pin_info *pin);

/*
*Verify PIN
*/
int mbtk_verify_pin(mbtk_info_handle_t* handle, char *pin);

/*
*change PIN
*/
int mbtk_change_pin(mbtk_info_handle_t* handle, mbtk_change_pin_info *pin);

/*
*unlock PIN
*/
int mbtk_unlock_pin(mbtk_info_handle_t* handle, mbtk_unlock_pin_info *pin);


/*
* System power.
* type:
* 0: Reboot system.
* 1: Poweroff system.
* 2: Halt system.
*/
int mbtk_system_reboot(int type);

/*
* Get time type.
*/
int mbtk_time_get(mbtk_info_handle_t* handle, int *time_type);

/*
* Get net time.
*/
int mbtk_net_time_get(mbtk_info_handle_t* handle, char* time_str);

/*
* Absolute time conversion
*/
int mbtk_get_abs_time(char *time_str, time_t *time_out);

/*
* Set time.
*
* time_type:
* 0: Cell time
* 1: NTP time
* 2: User time
* time_str: "YYYY-MM-DD HH:MM:SS"
*/
int mbtk_time_set(mbtk_info_handle_t* handle, mbtk_time_type_enum time_type, char* time_str);

//call a phone number
int mbtk_call_start(mbtk_info_handle_t* handle, char* phone_number);
//answer a call
int mbtk_call_answer(mbtk_info_handle_t* handle);
//hang up all call
int mbtk_call_hang(mbtk_info_handle_t* handle);
//hang up a call
int mbtk_a_call_hang(mbtk_info_handle_t* handle, int phone_id);
//hang up waiting or background call
int mbtk_waiting_or_background_call_hang(mbtk_info_handle_t* handle);
//hang up foreground resume background call
int mbtk_foreground_resume_background_call_hang(mbtk_info_handle_t* handle);
//get call infomation
int mbtk_call_reg_get(mbtk_info_handle_t* handle, mbtk_call_info_t *reg);
//set call mute
int mbtk_mute_state_set(mbtk_info_handle_t* handle, int mute_state);
//get mute state
int mbtk_mute_state_get(mbtk_info_handle_t* handle, int *mute_state);
//Wake-up control
int mbtk_wakeup_state_set(mbtk_info_handle_t* handle, uint32 wakeup_state);
//Get the searching interval of OOS
int mbtk_oos_get(mbtk_info_handle_t* handle, mbtk_oos_info *oos_info);
//Set the searching interval of OOS
int mbtk_oos_set(mbtk_info_handle_t* handle, mbtk_oos_info *oos_info);


//set dtmf character
int mbtk_dtmf_send(mbtk_info_handle_t* handle, mbtk_call_dtmf_info_t *dtmf_character);

//set led
int mbtk_led_set(mbtk_info_handle_t* handle, mbtk_led_type type, mbtk_led_status status);
/*
* Set pdp state change callback function.
*/
int mbtk_pdp_state_change_cb_reg(mbtk_info_handle_t* handle, mbtk_info_callback_func cb);

/*
* Set network state change callback function.
*/
int mbtk_net_state_change_cb_reg(mbtk_info_handle_t* handle, mbtk_info_callback_func cb);

/*
* Set call state change callback function.
*/
int mbtk_call_state_change_cb_reg(mbtk_info_handle_t* handle, mbtk_info_callback_func cb);

/*
* Set sms state change callback function.
*/
int mbtk_sms_state_change_cb_reg(mbtk_info_handle_t* handle, mbtk_info_callback_func cb);

/*
* Set radio state change callback function.
*/
int mbtk_radio_state_change_cb_reg(mbtk_info_handle_t* handle, mbtk_info_callback_func cb);

/*
* Set sim state change callback function.
*/
int mbtk_sim_state_change_cb_reg(mbtk_info_handle_t* handle, mbtk_info_callback_func cb);

/*
* Set signal state change callback function.
*/
int mbtk_signal_state_change_cb_reg(mbtk_info_handle_t* handle, mbtk_info_callback_func cb);


int mbtk_sms_cnmi_set(mbtk_info_handle_t* handle);

#endif /* _MBTK_INFO_API_H */
