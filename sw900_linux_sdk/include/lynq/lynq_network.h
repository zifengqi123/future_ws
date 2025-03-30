#ifndef LYNQ_NETWORK_H
#define LYNQ_NETWORK_H 
#ifdef __cplusplus
extern "C" {
#endif
#define MODEM_GEN97 1
#define CELLINFO_MAX_NUM  10
#define LY_RECOVER_TIMER_INTERVAL 128
#define MAX_CELLINFO_ITEM_NUMBER 32
#define MAX_OOS_CFG_ITEM_NUMBER 32
/*T800 platform support gsm,wcdma lte,nr */
typedef struct{
    int gw_sig_valid; /*1 valid,1 invalid*/
    int rssi;         /* Valid values are (0-31, 99) as defined in TS 27.007 8.5 */
    int wcdma_sig_valid;/*1 valid,0 invalid*/
    int wcdma_signalstrength;
    int rscp;        /* The Received Signal Code Power in dBm multipled by -1.
                      * Range : 25 to 120
                      * INT_MAX: 0x7FFFFFFF denotes invalid value.
                      * Reference: 3GPP TS 25.123, section 9.1.1.1 */
    int ecno;        /* Valid values are positive integers.  This value is the actual Ec/Io multiplied
                      * by -10.  Example: If the actual Ec/Io is -12.5 dB, then this response value
                      * will be 125.*/
    int lte_sig_valid;/*1 valid,0 invalid*/
    int lte_signalstrength;                      
    int rsrp;        /* The current Reference Signal Receive Power in dBm multipled by -1.
                      * Range: 44 to 140 dBm
                      * INT_MAX: 0x7FFFFFFF denotes invalid value.
                      * Reference: 3GPP TS 36.133 9.1.4 */
    int rsrq;        /* The current Reference Signal Receive Quality in dB multiplied by -1.
                      * Range: 20 to 3 dB.
                      * INT_MAX: 0x7FFFFFFF denotes invalid value.
                      * Reference: 3GPP TS 36.133 9.1.7 */
    int rssnr;       /* The current reference signal signal-to-noise ratio in 0.1 dB units.
                      * Range: -200 to +300 (-200 = -20.0 dB, +300 = 30dB).
                      * INT_MAX : 0x7FFFFFFF denotes invalid value.
                      * Reference: 3GPP TS 36.101 8.1.1 */
    int nr_sig_valid;/*1 valid,0 invalid*/
    int ssRsrp;      /* SS(Synchronization Signal) reference signal received power, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [44, 140], INT_MAX means invalid/unreported.*/
    int ssRsrq;      /* SS reference signal received quality, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [3, 20], INT_MAX means invalid/unreported.*/
    int ssSinr;      /* SS signal-to-noise and interference ratio.
                      * Reference: 3GPP TS 38.215 section 5.1.*, 3GPP TS 38.133 section 10.1.16.1.
                      * Range [-23, 40], INT_MAX means invalid/unreported.*/
    int csiRsrp;     /* CSI reference signal received power, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [44, 140], INT_MAX means invalid/unreported.*/
    int csiRsrq;     /* CSI reference signal received quality, multipled by -1.
                      * Reference: 3GPP TS 38.215.
                      * Range [3, 20], INT_MAX means invalid/unreported.*/
    int csiSinr;     /* CSI signal-to-noise and interference ratio.
                      * Reference: 3GPP TS 138.215 section 5.1.*, 3GPP TS 38.133 section 10.1.16.1.
                      * Range [-23, 40], INT_MAX means invalid/unreported.*/
}signalStrength_t;

typedef enum { 
    NETWORK_RADIO_ON_TYPE_CFUN_0=0,  
    NETWORK_RADIO_ON_TYPE_NORMAL_MODE=1,
    NETWORK_RADIO_ON_TYPE_FLIGHT_MODE=4,
}lynq_network_radio_on_type;  

int lynq_network_init(int utoken);
int lynq_network_deinit(void);
int lynq_query_operater(char *OperatorFN,char *OperatorSH,char *MccMnc);
int lynq_query_network_selection_mode(int *netselMode);
int lynq_set_network_selection_mode(const char *mode,const char* mccmnc);
int lynq_query_available_network(char *OperatorFN,char *OperatorSH,char *MccMnc,char * NetStatus);
int lynq_query_registration_state(const char *type,int* regState,int* imsRegState,char * LAC,char * CID,int *netType,int *radioTechFam,int *netRejected);
int lynq_query_prefferred_networktype(int *preNetType);
int lynq_set_prefferred_networktype(const int preffertype);
int lynq_query_cell_info(uint64_t cellinfo[CELLINFO_MAX_NUM],int tac[CELLINFO_MAX_NUM],int earfcn[CELLINFO_MAX_NUM],int * realNum);
int lynq_set_unsol_cell_info_listrate(const int rate);
int lynq_set_band_mode(const int bandmode);
int lynq_query_available_bandmode(int availBanMode[]);
int lynq_radio_on(const lynq_network_radio_on_type type);
int lynq_query_radio_state(int *radio_state);
int lynq_query_radio_tech(int* radioTech);
int lynq_solicited_signal_strength(signalStrength_t *solSigStren);
int lynq_set_ims(const int ims_mode);
int lynq_wait_signalchanges(int *handle);
int lynq_get_urc_info(const int handle,signalStrength_t *solSigStren,int *slot_id);

typedef enum {  
  LYNQ_CELL_INFO_TYPE_GSM    = 2,  
  LYNQ_CELL_INFO_TYPE_WCDMA  = 3,
  LYNQ_CELL_INFO_TYPE_LTE    = 4, 
  LYNQ_CELL_INFO_TYPE_NR    = 5
} lynq_network_cell_info_type;

int lynq_query_serve_cell_info(int serve_cell_info[MAX_CELLINFO_ITEM_NUMBER], int * valid_number);

typedef enum {
    NETWORK_STATE_BUFFER_TYPE_VOICE_REG,
    NETWORK_STATE_BUFFER_TYPE_DATA_REG,
    NETWORK_STATE_BUFFER_TYPE_IMS_REG,
    NETWORK_STATE_BUFFER_TYPE_MAX,
}lynq_network_state_buffer_type;    
int lynq_set_state_buffer_valid_period_of_network(lynq_network_state_buffer_type type,int valid_period);
int lynq_get_state_buffer_valid_period_of_network(lynq_network_state_buffer_type type);
#ifdef MODEM_GEN97
/**@brief set or get OOS recover timer interval
* @param mode [IN] <mode>:
*  0:set
*  1:get
* @param interval [IN] <interval>: the recovery timer interval.
*  field:
*    T1,T2,...,T23:integer value 0~0xFFFFFFFF
*    The length of time in seconds.
*    defualt interval: 20,20,60,60,60,60,90,90,90,90,90,90,180,180,180,180,180,180,360,360,360,360,360
* @param result [OUT] <result>: the recovery timer interval,when the mode is 1, this parameter is valid.

* @return
*  0:success
*  other:failure
*/
int lynq_oos_recover_timer_interval(int mode, char interval[LY_RECOVER_TIMER_INTERVAL],char result[LY_RECOVER_TIMER_INTERVAL]);
/**@brief set deep sleep recover timer interval

* @param recovery_threshold [IN] <recovery_threshold>: valid range 2-10  //After performing several rounds of normal recovery, deep sleep mode optimization will be applied

* @param fullband_timer [IN] <fullband_timer>:valid range 90-360 //fullband intervarl length in deep sleep mode

* @param sniffer_timer [IN] <sniffer_timer>:valid range 10-60 // sniffe intervarl length in deep sleep mode.

* @param inactive_mode [IN] <inactive_mode>:
*  0 Exit inactive mode 
*  1 enter inactive mode

* @return
*  0:success
*  other:failure
*/
int lynq_oos_deep_sleep_recover_timer_interval(int recovery_threshold,int fullband_timer,int sniffer_timer,int inactive_mode);
#endif
#if (defined MOBILETEK_TARGET_PLATFORM_T106) || (defined MODEM_GEN97)
int lynq_set_oos_net_scan_cfg(int config_list[MAX_OOS_CFG_ITEM_NUMBER], int valid_num);
int lynq_get_oos_net_scan_cfg(int config_list[MAX_OOS_CFG_ITEM_NUMBER], int* valid_num);
#endif
void lynq_set_test_mode(const int test_mode);
#ifdef __cplusplus
}
#endif

#endif
