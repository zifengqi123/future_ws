#ifndef LYNQ_CALL_H
#define LYNQ_CALL_H 1
#ifdef __cplusplus
extern "C" {
#endif
#define LYNQ_PHONE_NUMBER_MAX 32
#define INDEX_IS_NUMBER( alpha_char )   \
   ( ( (alpha_char >= '0') && (alpha_char <= '9') ) ? 1 : 0 )
#define LYNQ_CALL_MAX 3
#define LYNQ_CALL_FAIL_VENDOR_CAUSE 64
#define MAX_IP_LENGTH 128
#define MAX_VLAN_INFO_LENGTH 32
#define MAX_SSRC_INFO_LENGTH 10   /*2^32=4294967296, 10 digit*/

typedef enum {
    LYNQ_CALL_FAIL_UNOBTAINABLE_NUMBER = 1,
    LYNQ_CALL_FAIL_NORMAL = 16,
    LYNQ_CALL_FAIL_BUSY = 17,
    LYNQ_CALL_FAIL_CONGESTION = 34,
    LYNQ_CALL_FAIL_ACM_LIMIT_EXCEEDED = 68,
    LYNQ_CALL_FAIL_CALL_BARRED = 240,
    LYNQ_CALL_FAIL_FDN_BLOCKED = 241,
    LYNQ_CALL_FAIL_IMSI_UNKNOWN_IN_VLR = 242,
    LYNQ_CALL_FAIL_IMEI_NOT_ACCEPTED = 243,
    LYNQ_CALL_FAIL_DIAL_MODIFIED_TO_USSD = 244, /* STK Call Control */
    LYNQ_CALL_FAIL_DIAL_MODIFIED_TO_SS = 245,
    LYNQ_CALL_FAIL_DIAL_MODIFIED_TO_DIAL = 246,
    LYNQ_CALL_FAIL_CDMA_LOCKED_UNTIL_POWER_CYCLE = 1000,
    LYNQ_CALL_FAIL_CDMA_DROP = 1001,
    LYNQ_CALL_FAIL_CDMA_INTERCEPT = 1002,
    LYNQ_CALL_FAIL_CDMA_REORDER = 1003,
    LYNQ_CALL_FAIL_CDMA_SO_REJECT = 1004,
    LYNQ_CALL_FAIL_CDMA_RETRY_ORDER = 1005,
    LYNQ_CALL_FAIL_CDMA_ACCESS_FAILURE = 1006,
    LYNQ_CALL_FAIL_CDMA_PREEMPTED = 1007,
    LYNQ_CALL_FAIL_CDMA_NOT_EMERGENCY = 1008, /* For non-emergency number dialed
                                            during emergency callback mode */
    LYNQ_CALL_FAIL_CDMA_ACCESS_BLOCKED = 1009, /* CDMA network access probes blocked */
    LYNQ_CALL_FAIL_ERROR_UNSPECIFIED = 0xffff
} lynq_last_call_fail_cause_t;
typedef struct
{
    lynq_last_call_fail_cause_t cause_code;
    char vendor_cause[LYNQ_CALL_FAIL_VENDOR_CAUSE];
}lynqLastCallFailCause;
int lynq_init_call(int uToken);
int lynq_deinit_call();
int lynq_call(int *handle,char addr[]);
int lynq_call_answer();
int lynq_call_hungup(int*handle);
int lynq_call_hungup_all();
int lynq_set_auto_answercall(const int mode);
int lynq_get_mute_mic(int *status);
int lynq_set_mute_mic(const int enable);
int lynq_get_speech_volume(int *volume);
int lynq_set_speech_volume(const int volume);
int lynq_incall_record_start(const char* file_path);
int lynq_incall_record_stop();
int lynq_set_DTMF(const char callnum);
int lynq_set_DTMF_volume(const int volume);
int lynq_get_current_call_state(int *handle,    int *call_state,int *toa,int *direction,char addr[]);
/*lei add for T8TSK-17 2022/7/20*/
int lynq_switch_waiting_or_holding_and_active(void);
int lynq_hangup_waiting_or_background(void);
int lynq_hangup_foreground_resume_background(void);

/* add for T106bug-213 2023/9/18 */
void lynq_release_wait_call();

/*lei add for API-163/API-164 2023/1/3*/
/**
 * @brief                  wait_call_state_change
 *                        
 * @param  handle            call_id            
 * @return int
 */
int lynq_wait_call_state_change(int *handle);
/*lei add for API-163/API-164 2023/1/3*/
/*lei add for T8TSK-17 2022/7/20*/

#ifdef ECALL_SUPPORT
typedef enum{
    LYNQ_ECALL_SET_NUMBER = 0,     /* Set Number */
    LYNQ_ECALL_SET_URI = 1,      /* Set Uri */   
}LYNQ_ECall_Set_Type;

typedef enum{
    LYNQ_EMER_CAT_MANUAL_ECALL = 0,      /* Manual Emergency eCall */
    LYNQ_EMER_CAT_AUTO_ECALL = 1,     /*  Automatic Emergency eCall */
}LYNQ_ECall_Category;

typedef enum{
    LYNQ_ECALL_TEST = 0,     /* Test eCall */
    LYNQ_ECALL_EMERGENCY = 1,      /* Emergency eCall */
    LYNQ_ECALL_RECONFIG = 2,     /*  Reconfiguration eCall */
    LYNQ_ECALL_MO_MAX=3,
    LYNQ_ECALL_CALLBACK = 3,     /*  Callback eCall */
    LYNQ_ECALL_VAR_MAX, 
    LYNQ_ECALL_VAR_NONE=LYNQ_ECALL_VAR_MAX,
}LYNQ_ECall_Variant;

typedef enum{
    LYNQ_ECALL_SENDING_START = 1,
    LYNQ_ECALL_SENDING_MSD = 2,
    LYNQ_ECALL_LLACK_RECEIVED = 3,
    LYNQ_ECALL_ALACK_POSITIVE_RECEIVED = 4,
    LYNQ_ECALL_ALACK_CLEARDOWN_RECEIVED = 5,
    LYNQ_ECALL_DAILING = 9,
    LYNQ_ECALL_ALERTING = 10,
    LYNQ_ECALL_ACTIVE = 11,
    LYNQ_ECALL_DISCONNECTED = 12,
    LYNQ_ECALL_IMS_ACTIVE = 13,
    LYNQ_ECALL_IMS_DISCONNECTED = 14,
    LYNQ_ECALL_ABNORMAL_HANGUP=15,
    LYNQ_ECALL_IMS_MSD_ACK = 20,
    LYNQ_ECALL_IMS_UPDATE_MSD = 21,
    LYNQ_ECALL_IMS_IN_BAND_TRANSFER = 22,
    LYNQ_ECALL_IMS_MSD_NACK = 23,
    LYNQ_ECALL_IMS_SRVCC = 24,
    LYNQ_ECALL_ONLY_DEREGISTRATION = 31,
    LYNQ_ECALL_MAY_DEREGISTER = 32,
    LYNQ_ECALL_PSAP_CALLBACK_START = 40,
    LYNQ_ECALL_PSAP_CALLBACK_IMS_UPDATE_MSD = 41,
    LYNQ_ECALL_SENDING_START_IN_VOICE=8000,    
    LYNQ_ECALL_T2_TIMER_OUT = 9000,
    LYNQ_ECALL_T5_TIMER_OUT = 9001,
    LYNQ_ECALL_T6_TIMER_OUT = 9002,
    LYNQ_ECALL_T7_TIMER_OUT = 9003,
    LYNQ_ECALL_REDIAL_TIMER_OUT = 9004,    
    LYNQ_ECALL_AUTO_ANS_TIMER_OUT = 9005,
    LYNQ_ECALL_AUTO_ANS_IMS_TIMER_OUT = 9006,
    LYNQ_ECALL_UNSPECIFIED = 0xffff,
}LYNQ_ECall_Indication;

int lynq_set_test_num(LYNQ_ECall_Set_Type type, const char *test_num, int test_num_length);
int lynq_fast_ecall(int* handle, LYNQ_ECall_Category lynq_ecall_cat, LYNQ_ECall_Variant lynq_ecall_variant,  const char *addr, int addr_length, const unsigned char *msd_data, int msd_length);
int lynq_set_msd(int* handle, const unsigned char *msd_data, int msd_length);
int lynq_set_ivs(int enable);
int lynq_ivs_push_msd();
int lynq_wait_ecall_indication(int* handle, LYNQ_ECall_Indication *eCall_Indication);
#endif

/*Audio Path setting begin*/
typedef enum{
    AUDIO_MODE_CODEC = 0,     /* Codec */
    AUDIO_MODE_RTP = 1,      /* RTP */       
}LYNQ_Audio_Mode;

typedef enum{
    RTP_CLIENT = 0,     
    RTP_SERVER =1,
    RTP_MODE_MAX
}LYNQ_Rtp_Mode;

typedef enum{
    Rtp_Ssrc_random = 0,     
    Rtp_Ssrc_specific =1,   
}LYNQ_Rtp_Ssrc_Mode;

/*set*/
int lynq_set_voice_audio_mode(const LYNQ_Audio_Mode audio_mode); 
int lynq_set_remote_rtp_ip(const char* ip, const int ip_length);
int lynq_set_vlan_info(const char* vlan_info, const int vlan_info_length);
int lynq_set_rtp_port(const LYNQ_Rtp_Mode rtp_mode, const int port);
int lynq_set_rtp_param(const int clock_rate,const int channels,const int latency); //only for client
int lynq_set_call_rtp_ssrc(const LYNQ_Rtp_Ssrc_Mode ssrc_mode, const unsigned int ssrc);

/*get*/
LYNQ_Audio_Mode lynq_get_voice_audio_mode();
int lynq_get_remote_rtp_ip(char* ip, const int ip_length);
int lynq_get_vlan_info(char* vlan_info, const int vlan_info_length);
int lynq_get_rtp_port(const LYNQ_Rtp_Mode rtp_mode, int* port);
int lynq_get_rtp_param(int* clock_rate,int* channels, int* latency);//only for client
int lynq_get_call_rtp_ssrc(int* ssrc_mod, unsigned int* ssrc);
/*Audio Path setting end*/

void lynq_set_test_mode(const int test_mode);
#ifdef __cplusplus
}
#endif
#endif
