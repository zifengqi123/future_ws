/*
*
* Data : 2024/07/19 11:24:09
* Author : LiuBin
*/
#ifndef _MBTK_INFO_API_2_H
#define _MBTK_INFO_API_2_H
#include <netinet/in.h>

#include "mbtk_type.h"
#include "mbtk_list.h"
#include "mbtk_log.h"
#include "mbtk_utils.h"
#include "mbtk_pdu_sms.h"

// LOG_DEBUG_LEVEL 7
#define LOG LOGI

#define MBTK_APN_CID_MIN 1
#define MBTK_APN_CID_MAX 7
#define CELL_NUM_MAX 15
#if 1
#define APN_STR_MAX 128
#define APN_NAME_STR_MAX (150+1)
#else
#define APN_STR_MAX 64
#define APN_NAME_STR_MAX (64+1)
#endif
#define RIL_PIN_STR_MAX 16

typedef void (*mbtk_ril_callback_func)(const void* data, int data_len);

typedef enum {
    MBTK_RIL_CID_NUL = 0,   // Auto set cid.
    MBTK_RIL_CID_DEF = MBTK_APN_CID_MIN,
    MBTK_RIL_CID_1 = MBTK_RIL_CID_DEF,
    MBTK_RIL_CID_2,
    MBTK_RIL_CID_3,
    MBTK_RIL_CID_4,
    MBTK_RIL_CID_5,
    MBTK_RIL_CID_6,
    MBTK_RIL_CID_7 = MBTK_APN_CID_MAX
} mbtk_ril_cid_enum;

typedef enum {
    MBTK_RIL_ERR_SUCCESS = 0,
    MBTK_RIL_ERR_INITED,
    MBTK_RIL_ERR_NOT_INIT,
    MBTK_RIL_ERR_PARAMETER,     // Parameter error.
    MBTK_RIL_ERR_FORMAT,       // Packet format error.
    MBTK_RIL_ERR_SEND_PACK,    // Packet send fail.
    MBTK_RIL_ERR_REQ_UNKNOWN,  // Unknown request.
    MBTK_RIL_ERR_REQ_PARAMETER,  // Request parameter error.
    MBTK_RIL_ERR_UNSUPPORTED,  // Unsupported operation.
    MBTK_RIL_ERR_MEMORY,       // Insufficient memory.
    MBTK_RIL_ERR_IND_FULL,     // Register IND fail(FULL).
    MBTK_RIL_ERR_IND_UNKNOWN,  // Register IND fail(Unknown IND).
    MBTK_RIL_ERR_CID,  // CID error.
    MBTK_RIL_ERR_NET_CONF,
    MBTK_RIL_ERR_TIMEOUT,  // Timeout.
    MBTK_RIL_ERR_TIME_FORMAT,  // Time format error.

    MBTK_RIL_ERR_UNKNOWN = 400,  // Unknown error.
    // CME error start index.
    MBTK_RIL_ERR_CME = 500,
    MBTK_RIL_ERR_CME_NON = 1000
} mbtk_ril_err_enum;

typedef enum {
    MBTK_IP_TYPE_IP,
    MBTK_IP_TYPE_IPV6,
    MBTK_IP_TYPE_IPV4V6,
    MBTK_IP_TYPE_PPP
} mbtk_ip_type_enum;

typedef enum {
    MBTK_TEMP_TYPE_SOC,
    MBTK_TEMP_TYPE_RF
} mbtk_temp_type_enum;

typedef enum {
    MBTK_TIME_TYPE_CELL = 0,  //NITZ
    MBTK_TIME_TYPE_NTP,
    MBTK_TIME_TYPE_GNSS,
    MBTK_TIME_TYPE_USER
} mbtk_time_type_enum;

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

typedef enum {
    MBTK_RADIO_STATE_UNKNOWN = -1,
    MBTK_RADIO_STATE_MINI_FUNC = 0,
    MBTK_RADIO_STATE_FULL_FUNC,
    MBTK_RADIO_STATE_DIS_RECV_RF = 3,
    MBTK_RADIO_STATE_DIS_RF,
    MBTK_RADIO_STATE_DIS_SIM,
    MBTK_RADIO_STATE_SEC_RX_OFF
} mbtk_radio_state_enum;

typedef enum {
    MBTK_SIM_STATE_UNKNOWN = -1,
    MBTK_SIM_STATE_ABSENT = 0,
    MBTK_SIM_STATE_NOT_READY,
    MBTK_SIM_STATE_READY,
    MBTK_SIM_STATE_SIM_PIN,
    MBTK_SIM_STATE_SIM_PUK,
    MBTK_SIM_STATE_PH_SIMLOCK_PIN,
    MBTK_SIM_STATE_PH_SIMLOCK_PUK,
    MBTK_SIM_STATE_PH_FSIM_PIN,
    MBTK_SIM_STATE_PH_FSIM_PUK,
    MBTK_SIM_STATE_SIM_PIN2,
    MBTK_SIM_STATE_SIM_PUK2,
    MBTK_SIM_STATE_PH_NET_PIN,
    MBTK_SIM_STATE_PH_NET_PUK,
    MBTK_SIM_STATE_PH_NETSUB_PIN,
    MBTK_SIM_STATE_PH_NETSUB_PUK,
    MBTK_SIM_STATE_PH_SP_PIN,
    MBTK_SIM_STATE_PH_SP_PUK,
    MBTK_SIM_STATE_PH_CORP_PIN,
    MBTK_SIM_STATE_PH_CORP_PUK
} mbtk_sim_state_enum;


typedef enum
{
    MBTK_NET_TYPE_UNKNOWN,
    MBTK_NET_TYPE_GSM,
    MBTK_NET_TYPE_UMTS,
    MBTK_NET_TYPE_LTE
} mbtk_net_type_enum;

typedef enum
{
    MBTK_DEV_MODEM_MIN_FUN,                                   //Modem 最小功能
    MBTK_DEV_MODEM_FULL_FUN,                                  //Modem 全功能
    MBTK_DEV_MODEM_DISABLE_RECEIVE_RF_CIRCUITS = 3,           //Modem 禁用射频接收电路
    MBTK_DEV_MODEM_DISABLE_TRANSMIT_AND_RECEIVE_RF_CIRCUITS,  //Modem禁用射频发射和接收电路
    MBTK_DEV_MODEM_DISABLE_SIM,                               //Modem 禁用(U)SIM 卡
    MBTK_DEV_MODEM_TURN_OFF_FULL_SECONDARY_RECEIVE,           //Modem 完全禁用辅助接收
} MBTK_DEV_MODEM_FUNCTION;

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
    MBTK_NET_PREF_UNUSE = -1,
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

typedef struct {
    int num;
    mbtk_net_info_t net_info[20];
} mbtk_net_info_array_t;

typedef struct
{
    mbtk_net_pref_enum net_pref;     // mbtk_net_pref_enum
    uint32 gsm_band;    // mbtk_gsm_band_enum
    uint32 umts_band;   // mbtk_umts_band_enum
    uint32 tdlte_band;  // mbtk_tdlte_band_enum
    uint32 fddlte_band; // mbtk_fddlte_band_enum
    uint32 lte_ext_band; // mbtk_lte_ext_band_enum
} __attribute__((packed)) mbtk_band_info_t;

typedef enum
{
    MBTK_CELL_TYPE_GSM = 0,
    MBTK_CELL_TYPE_UMTS,
    MBTK_CELL_TYPE_LTE
} mbtk_cell_type_enum;

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

typedef struct {
    mbtk_ipv4_info_t ipv4;
    mbtk_ipv6_info_t ipv6;
} __attribute__((packed)) mbtk_ip_info_t;

typedef struct
{
    mbtk_radio_technology_enum type : 8; // mbtk_radio_technology_enum
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

typedef struct
{
    mbtk_net_reg_state_enum call_state : 8;// mbtk_net_reg_state_enum
    mbtk_net_reg_state_enum data_state : 8;// mbtk_net_reg_state_enum
    mbtk_net_reg_state_enum ims_state : 8;// mbtk_net_reg_state_enum
    mbtk_radio_technology_enum type : 8; // mbtk_radio_technology_enum
    uint16 lac;
    uint32 ci;
} __attribute__((packed)) mbtk_net_reg_info_t;

typedef enum {
	MBTK_APN_AUTH_PROTO_DEFAULT = 0,
	MBTK_APN_AUTH_PROTO_NONE,
	MBTK_APN_AUTH_PROTO_PAP,
	MBTK_APN_AUTH_PROTO_CHAP,
#if 0
	MBTK_APN_AUTH_PROTO_PAP_CHAP,
    //NOT SUPPORT
#endif
} mbtk_apn_auth_type_enum;

typedef struct {
    mbtk_ril_cid_enum cid : 8; // 1 - 7
    mbtk_ip_type_enum ip_type : 8; // mbtk_ip_type_enum
    mbtk_apn_auth_type_enum auth : 8;
    uint8 auto_save;
    uint8 auto_boot_call;
    uint8 def_route;
    uint8 as_dns;
    uint8 apn[APN_NAME_STR_MAX];
    uint8 user[APN_STR_MAX];
    uint8 pass[APN_STR_MAX];
    uint8 type[APN_NAME_STR_MAX];
} mbtk_apn_info_t;

#if 0
typedef struct {
    mbtk_ril_cid_enum cid : 8; // 1 - 7
    mbtk_ip_type_enum ip_type : 8; // mbtk_ip_type_enum
    mbtk_apn_auth_type_enum auth : 8;
    uint8 auto_save;
    uint8 auto_boot_call;
    uint8 apn[APN_STR_MAX];
} mbtk_apn_get_info_t;
#endif

typedef struct {
    mbtk_ril_cid_enum cid_for_def_route;
    mbtk_ril_cid_enum cid_for_def_dns;

    int num;
    mbtk_apn_info_t apns[MBTK_APN_CID_MAX];
} mbtk_apn_info_array_t;

typedef enum {
    MBTK_SIM = 0,
    MBTK_USIM = 1,
    MBTK_TEST_SIM = 2,
    MBTK_TEST_USIM = 3,
    MBTK_UNKNOWN = 4,
} mbtk_sim_card_type_enum;

typedef struct
{
    uint8 p1_retry;
    uint8 p2_retry;
    uint8 puk1_retry;
    uint8 puk2_retry;
} mbtk_pin_puk_last_times_t;

typedef enum {
    MBTK_SIM_LOCK_TYPE_DISABLE = 0,     // Disable sim lock: [PIN]
    MBTK_SIM_LOCK_TYPE_ENABLE,      // Enable sim lock: [PIN]
    // MBTK_SIM_LOCK_TYPE_QUERY,       // Query sim lock state.
    MBTK_SIM_LOCK_TYPE_CHANGE = 3,      // Change pin : [old PIN][new PIN]
    MBTK_SIM_LOCK_TYPE_VERIFY_PIN,  // Verify pin : [PIN]
    MBTK_SIM_LOCK_TYPE_VERIFY_PUK   // Verify puk : [PUK][new PIN]
} mbtk_sim_lock_type_enum;

typedef struct {
    mbtk_sim_lock_type_enum type : 8;
    uint8 pin1[RIL_PIN_STR_MAX];
    uint8 pin2[RIL_PIN_STR_MAX];
    uint8 puk[RIL_PIN_STR_MAX];
} mbtk_sim_lock_info_t;

typedef struct
{
    uint8 format;
    char name[16];
}MBTK_SIM_PLMN_INFO_C;  //string type

typedef struct
{
    uint8 count;
    MBTK_SIM_PLMN_INFO_C plmn[24];
} mbtk_plmn_info;

typedef struct
{
    // LTE server cell: tac, PCI, dlEuarfcn, ulEuarfcn, band
    // LTE cell: phyCellId,euArfcn,rsrp,rsrq
    // WCDMA server cell: lac, ci, arfcn
    // WCDMA cell: lac, ci, arfcn
    // GSM server cell: lac, ci, arfcn, bsic
    // GSM cell:
    mbtk_cell_type_enum type : 8;
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

typedef struct {
    mbtk_cell_type_enum type;
    int num;
    mbtk_cell_info_t cell[CELL_NUM_MAX];
} mbtk_cell_info_array_t;

typedef struct {
    mbtk_data_call_type_enum type : 8;
    mbtk_ril_cid_enum cid : 8;  // 1 - 7
    uint8 auto_boot_call;       // Will reset proc if necessary.
    uint8 def_route;            // Will reset proc if necessary.
    uint8 as_dns;               // Will reset proc if necessary.
    uint16 retry_interval;      // second
    uint16 timeout;             // second
} mbtk_data_call_info_t;

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

typedef struct
{
    bool running;

    mbtk_cell_info_array_t cell_list;
} mbtk_cell_pack_info_t;

mbtk_ril_err_enum mbtk_ril_init();

mbtk_ril_err_enum mbtk_ril_deinit();

/*
* Get platform version.
*/
mbtk_ril_err_enum mbtk_version_get(void *version);

/*
* Get platform model.
*/
mbtk_ril_err_enum mbtk_model_get(void *model);

/*
* Get platform IMEI.
*/
mbtk_ril_err_enum mbtk_imei_get(void *imei);

/*
* Get platform SN.
*/
mbtk_ril_err_enum mbtk_sn_get(void *sn);

/*
* Get platform MEID.
*/
mbtk_ril_err_enum mbtk_meid_get(void *meid);

/*
* Return VoLTE state.
*/
mbtk_ril_err_enum mbtk_volte_state_get(int *volte_state);

/*
* Set VoLTE state.
*
* volte_state:
* 0 : Close VoLTE.
* 1 : Open VoLTE.
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_volte_state_set(int volte_state);

/*
* Return radio state.
*/
mbtk_ril_err_enum mbtk_radio_state_get(mbtk_radio_state_enum *radio_state);

/*
* Set radio state.
*
* Refor to : mbtk_radio_state_enum
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_radio_state_set(mbtk_radio_state_enum radio_state, bool reset_modem);

/*
* Get system temperature.
*
* type[IN]:
*   0: Soc temperature.
*   1: RF temperature.
* temp[OUT]:
*   temperature in celsius.
*/
mbtk_ril_err_enum mbtk_temp_get(mbtk_temp_type_enum type, int* temp);

/*
* Get time type.
* "23/05/24,06:09:32+32" -> "23/05/24 06:09:32 +32 00"
*/
mbtk_ril_err_enum mbtk_cell_time_get(char* time_str);

/*
* Get platform IMSI.
*/
mbtk_ril_err_enum mbtk_imsi_get(void *imsi);

/*
* Get platform ICCID.
*/
mbtk_ril_err_enum mbtk_iccid_get(void *iccid);

/*
* Get current phone number.
*/
mbtk_ril_err_enum mbtk_phone_number_get(void *phone_number);

/*
* Get platform ICCID.
*/
mbtk_ril_err_enum mbtk_sim_lock_get(int *lock_state);

/*
* Get platform ICCID.
*/
mbtk_ril_err_enum mbtk_sim_lock_set(mbtk_sim_lock_info_t *lock_info);

/*
* Get platform ICCID.
*/
mbtk_ril_err_enum mbtk_sim_lock_retry_times_get(mbtk_pin_puk_last_times_t *retry_times);

/*
* Get available network.
*/
mbtk_ril_err_enum mbtk_available_net_get(mbtk_net_info_array_t *net_array);

/*
* Set network select mode. (+COPS=...)
*/
mbtk_ril_err_enum mbtk_net_sel_mode_set(const mbtk_net_info_t *net);

/*
* Get network select mode. (+COPS?)
*/
mbtk_ril_err_enum mbtk_net_sel_mode_get(mbtk_net_info_t *net);

/*
* Get platform support bands.
*/
mbtk_ril_err_enum mbtk_support_band_get(mbtk_band_info_t *band);

/*
* Get platform current bands.
*/
mbtk_ril_err_enum mbtk_current_band_get(mbtk_band_info_t *band);

/*
* Set platform current bands.
*/
mbtk_ril_err_enum mbtk_current_band_set(const mbtk_band_info_t *band);

/*
* Get current cell infomation.
*/
mbtk_ril_err_enum mbtk_cell_get(mbtk_cell_info_array_t *cell_list);

/*
* Set cell info.
*
* at*CELL=<mode>,<act>,< band>,<freq>,<cellId>
* at*cell=2,3,,40936,429   //
* at*cell=0  //
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_cell_set(char *info, char *response);

/*
* Get current APN informations.
*/
mbtk_ril_err_enum mbtk_apn_get(mbtk_apn_info_array_t *apns);

/*
* Set current APN informations.
*/
mbtk_ril_err_enum mbtk_apn_set(const mbtk_apn_info_t *apn);

/*
* Start data call.
*/
mbtk_ril_err_enum mbtk_data_call_start(mbtk_ril_cid_enum cid, bool auto_boot_call,
            bool def_route, bool as_dns, int retry_interval, int timeout, mbtk_ip_info_t *rsp_info);

/*
* Stop data call.
*/
mbtk_ril_err_enum mbtk_data_call_stop(mbtk_ril_cid_enum cid, int timeout);

/*
* Get data call state.
*/
mbtk_ril_err_enum mbtk_data_call_state_get(mbtk_ril_cid_enum cid, mbtk_ip_info_t *ip);

/*
* Get current network signal.
*/
mbtk_ril_err_enum mbtk_net_signal_get(mbtk_signal_info_t *signal);

/*
* Get current network register information.
*/
mbtk_ril_err_enum mbtk_net_reg_get(mbtk_net_reg_info_t *reg);


/*
* Return sms cmgf.
*/
mbtk_ril_err_enum mbtk_sms_cmgf_get(int *volte_state);

/*
* Set sms cmgf.
*
* volte_state:
* 0 : PDU mode.
* 1 :  text mode.
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_sms_cmgf_set(int mode);

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
mbtk_ril_err_enum mbtk_sms_cmgs_set(char * cmgs, char *resp);

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
mbtk_ril_err_enum mbtk_sms_cmgw_set(char * cmgw, char *resp);

/*
* Set sms cmgd.
*
* +CMGD=<index>[,<delflag>]
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_sms_cmgd_set(char * cmdg);

/*
* Set sms cmgl.
*
* AT+CMGL[=<stat>]
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_sms_cmgl_set(char * cmgl, char *resp);

/*
* Return sms csca.
*/
mbtk_ril_err_enum mbtk_sms_csca_get(char *buf);

/*
* Set sms csca.
*
* AT+CSCA=<number> [,<type>]
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_sms_csca_set(char * csca);

/*
* Set sms csmp.
*
* AT+CSMP=[<fo>[,<vp>[,<pid>[,<dcs>]]]]
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_sms_csmp_set(char * csmp);

/*
* Set sms cscb.
*
* AT+CSCB=<[<mode>[,<mids>[,<dcss>]]]>
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_sms_cscb_set(char * cscb);

/*
* Set sms cnmi.
*
at+cnmi=1,2

OK
if sending fails:
+CMS ERROR: <err>
*/
mbtk_ril_err_enum mbtk_sms_cnmi_set();

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
mbtk_ril_err_enum mbtk_sms_cmss_set(char * cmss, char *resp);

/*
* Return sms cmgf.
*/
mbtk_ril_err_enum mbtk_sms_cpms_get(char * mem);

/*
* Set sms cpms.
*
* AT+CPMS=<mem1>[,<mem2>[,<mem3>]]
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_sms_cpms_set(char * mem, char* response);

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
mbtk_ril_err_enum mbtk_sms_cmgr_set(int index, char *resp);

/*
* Get sim state.
*/
mbtk_ril_err_enum mbtk_sim_state_get(mbtk_sim_state_enum *sim_state);

/*
* Get sim card type.
*/
mbtk_ril_err_enum mbtk_sim_type_get(mbtk_sim_card_type_enum *sim_card_type);

/*
* Get plmn list
*/
mbtk_ril_err_enum mbtk_plmn_list_get(mbtk_plmn_info *plmn_list);

/*
* call_start
*
*/
mbtk_ril_err_enum mbtk_call_start(char* phone_number);

/*
* Answer the phone call.
*
*/
mbtk_ril_err_enum mbtk_call_answer();

/*
* Hang up all call.
*
*/
mbtk_ril_err_enum mbtk_call_hang();

/*
* Hang up a call.
*
*/
mbtk_ril_err_enum mbtk_a_call_hang(int phone_id);

/*
* Hang up waiting or background call.
*
*/
mbtk_ril_err_enum mbtk_waiting_or_background_call_hang();

/*
* Hang up foreground resume background call.
*
*/
mbtk_ril_err_enum mbtk_foreground_resume_background_call_hang();

/*
* Get current call phone number.
*/
mbtk_ril_err_enum mbtk_call_reg_get(mbtk_call_info_t *reg);

/*
* Return mute state.
*/
mbtk_ril_err_enum mbtk_mute_state_get(int *mute_state);

/*
* Set mute state.
*
* mute_state:
* 0 : of mute.
* 1 : on mute.
*
* Restarting takes effect after execution.
*/
mbtk_ril_err_enum mbtk_mute_state_set(int mute_state);

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
int mbtk_time_get(int *time_type);

/*
* Get net time.
*/
int mbtk_net_time_get(char* time_str);

/*
* Absolute time conversion
*/
int mbtk_get_abs_time(char *time_str, time_t *time_out);

/*
* Set pdp state change callback function.
*/
int mbtk_pdp_state_change_cb_reg(mbtk_ril_callback_func cb);

/*
* Set network state change callback function.
*/
int mbtk_net_state_change_cb_reg(mbtk_ril_callback_func cb);

/*
* Set call state change callback function.
*/
int mbtk_call_state_change_cb_reg(mbtk_ril_callback_func cb);

/*
* Set sms state change callback function.
*/
int mbtk_sms_state_change_cb_reg(mbtk_ril_callback_func cb);

/*
* Set radio state change callback function.
*/
int mbtk_radio_state_change_cb_reg(mbtk_ril_callback_func cb);

/*
* Set sim state change callback function.
*/
int mbtk_sim_state_change_cb_reg(mbtk_ril_callback_func cb);

/*
* Set signal state change callback function.
*/
int mbtk_signal_state_change_cb_reg(mbtk_ril_callback_func cb);


#endif /* _MBTK_INFO_API_2_H */

