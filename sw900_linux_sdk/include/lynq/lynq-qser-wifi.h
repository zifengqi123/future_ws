/*******************************************************
* 
* @brief: Add wifi api
* @details:  add liblynq-qser-wifi api
* @author:   wz.wang
* @date:     2024.3.15
* @version:  V1.0
* @copyright：Copyright (c) MobileTek
*
*********************************************/
#ifndef LYNQ_QSER_WIFI
#define LYNQ_QSER_WIFI

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	LYNQ_WIFI_AP_INDEX_AP0 = 0,       /* Index 0 of AP/AP-STA/AP-AP Mode */
	LYNQ_WIFI_AP_INDEX_AP1            /* Index 1 of AP/AP-STA/AP-AP Mode */
} lynq_wifi_ap_index_e;

typedef enum
{
	LYNQ_WIFI_MODE_MIN = -1,
	LYNQ_WIFI_MODE_80211B = 0,
	LYNQ_WIFI_MODE_80211BG,
	LYNQ_WIFI_MODE_80211BGN,
	LYNQ_WIFI_MODE_80211A,
	LYNQ_WIFI_MODE_80211AN,
	LYNQ_WIFI_MODE_80211AC,
	LYNQ_WIFI_MODE_80211BGNAX_2G,
	LYNQ_WIFI_AP_MODE_MAX
} lynq_wifi_mode_type_e;

typedef enum LYNQ_WIFI_BANDWIDTH_ENUM
{
	LYNQ_WIFI_BANDWIDTH_MIN = -1,
	LYNQ_WIFI_BANDWIDTH_HT20 = 0,
	LYNQ_WIFI_BANDWIDTH_HT40,
	LYNQ_WIFI_BANDWIDTH_HT80,
	LYNQ_WIFI_BANDWIDTH_MAX
} lynq_wifi_bandwidth_type_e;

typedef enum
{ 
	LYNQ_WIFI_AUTH_MIN = -1,
	LYNQ_WIFI_AUTH_OPEN = 0,
	LYNQ_WIFI_AUTH_WPA_PSK,
	LYNQ_WIFI_AUTH_WPA2_PSK, //AES
	LYNQ_WIFI_AUTH_WPA_WPA2_PSK_BOTH, //TKIP & AES
	LYNQ_WIFI_AUTH_WPA3_PSK, //AES
	LYNQ_WIFI_AUTH_WPA2_WPA3_PSK_BOTH, //AES
	LYNQ_WIFI_AUTH_MAX
} lynq_wifi_auth_e;

int  qser_wifi_enable(void);
int  qser_wifi_disable(void);
int  qser_wifi_ap_ssid_set(lynq_wifi_ap_index_e idx, const char *ssid);
int  qser_wifi_ap_ssid_get(lynq_wifi_ap_index_e idx, char *ssid);
int  qser_wifi_ap_mode_set(lynq_wifi_ap_index_e idx, lynq_wifi_mode_type_e mode);
int  qser_wifi_ap_mode_get(lynq_wifi_ap_index_e idx, lynq_wifi_mode_type_e *mode);
int  qser_wifi_ap_bandwidth_set(lynq_wifi_ap_index_e idx, lynq_wifi_bandwidth_type_e bandwidth);
int  qser_wifi_ap_bandwidth_get(lynq_wifi_ap_index_e idx, lynq_wifi_bandwidth_type_e *bandwidth);
int  qser_wifi_ap_channel_set(lynq_wifi_ap_index_e idx, const char *country_code, int channel);
int  qser_wifi_ap_channel_get(lynq_wifi_ap_index_e idx, char *country_code, int *channel);
int  qser_wifi_ap_auth_set(lynq_wifi_ap_index_e idx, lynq_wifi_auth_e auth_mode, const char * auth_passwd);
int  qser_wifi_ap_auth_get(lynq_wifi_ap_index_e idx, lynq_wifi_auth_e *auth_mode, char * auth_passwd);
int  qser_wifi_ap_max_sta_set(lynq_wifi_ap_index_e idx, int max_sta_num);
int  qser_wifi_ap_max_sta_get(lynq_wifi_ap_index_e idx, int *max_sta_num);
int  qser_wifi_ap_start(lynq_wifi_ap_index_e idx);
int  qser_wifi_ap_stop(lynq_wifi_ap_index_e idx);
int  qser_wifi_ap_restart(lynq_wifi_ap_index_e idx);



#ifdef __cplusplus
}
#endif
#endif

