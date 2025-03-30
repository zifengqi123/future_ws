/**
 *@file     qser_voice.h
 *@date     2017-11-30
 *@author
 *@brief
 */


#ifndef __LYNQ_QSER_VOICE_H__
#define __LYNQ_QSER_VOICE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "mbtk_type.h"

typedef enum
{
    E_QSER_VCALL_EXTERNAL_SLOT_1,
    E_QSER_VCALL_EXTERNAL_SLOT_2,
    E_QSER_VCALL_EMBEDDED,
    E_QSER_VCALL_REMOTE,
    E_QSER_VCALL_ID_MAX
}E_QSER_VCALL_ID_T;

typedef enum
{
    E_QSER_VOICE_CALL_STATE_INCOMING      = 0x0000, /**< MT incoming; CC setup. */
    E_QSER_VOICE_CALL_STATE_DIALING       = 0x0001, /**< Dialing state. */
    E_QSER_VOICE_CALL_STATE_ALERTING      = 0x0002, /**< MT call waiting; MO alterting. */
    E_QSER_VOICE_CALL_STATE_ACTIVE        = 0x0003, /**< Call is active. */
    E_QSER_VOICE_CALL_STATE_HOLDING       = 0x0004, /**< Call is on hold. */
    E_QSER_VOICE_CALL_STATE_END           = 0x0005, /**< Call is disconnected. */
    E_QSER_VOICE_CALL_STATE_WAITING       = 0x0006, /**< Call is waiting. */
}qser_voice_call_state_t;

typedef enum
{
    E_QSER_VOICE_CALL_WAITING_VOICE_ENABLED       = 0, /**<  Voice call waiting enabled. */
    E_QSER_VOICE_CALL_WAITING_DISABLED            = 1, /**<  Voice call waiting disabled. */
}E_QSER_VOICE_CALL_WAITING_SERVICE_T;

typedef struct
{
    E_QSER_VOICE_CALL_WAITING_SERVICE_T   waiting_service;
}qser_voice_call_waiting_service_t;

typedef qser_voice_call_state_t E_QSER_VOICE_CALL_STATE_T;

typedef uint32_t voice_client_handle_type;

/* Callback function registered via QSER_Voice_Call_AddStateHandler;
   This will be called if any stated changed of call_id
*/
typedef void (*QSER_VoiceCall_StateHandlerFunc_t)
(
    int                     caLOCAL_C_INCLUDESll_id,
    char*                   phone_num,
    E_QSER_VOICE_CALL_STATE_T state,
    void                    *contextPtr
);

/* Init voice module and return h_voice, this should be called before any other APIs */
int qser_voice_call_client_init(voice_client_handle_type  *ph_voice);

/* DeInit voice module and release resources, this should be called at last */
int qser_voice_call_client_deinit(voice_client_handle_type h_voice);

/* Add callback function, if any call state changed, handlerPtr will be called to notify App */
int qser_voice_call_addstatehandler(voice_client_handle_type          h_voice,
                                  QSER_VoiceCall_StateHandlerFunc_t   handlerPtr,
                                  void*                             contextPtr);

/* Remove callback function, won't receive any notify anymore */
int qser_voice_call_removestatehandle(voice_client_handle_type       h_voice);

/* Start call and return call_id, this can be used in the later */
int qser_voice_call_start(voice_client_handle_type    h_voice,
                        E_QSER_VCALL_ID_T             simId,
                        char*                       phone_number,   ///< [IN] Destination identifier for the voice
                        int                         *call_id);       ///< [OUT] call id

/* End call of call_id, which returned by QSER_Voice_Call_Start or callback func register via QSER_Voice_Call_AddStateHandler */
int qser_voice_call_end(  voice_client_handle_type    h_voice,
                        int                         call_id);        ///< [IN] call id, return by QSER_Voice_Start

/* Answer the call of call_id, which returned by callback func register via QSER_Voice_Call_AddStateHandler */
int qser_voice_call_answer(voice_client_handle_type    h_voice,
                         int                         call_id );

int qser_voice_call_switch_waiting_or_holding_and_active(voice_client_handle_type h_voice);

//set voice speech volume
int qser_voice_set_speech_volume(const int volume);

//Get voice speech volume
int qser_voice_get_speech_volume(int *volume);

//Set voice call waiting
int qser_voice_call_setwaiting
(
    int                                 h_voice,
    qser_voice_call_waiting_service_t e_service
);

//Get voice call waiting status
int qser_voice_call_getwaitingstatus
(
    int                                 h_voice,
    qser_voice_call_waiting_service_t *pe_service
);


int qser_voice_set_dtmf(const char callnum);

/*
Usage 1 (register callback and wait for new call in, then answer):
1, QSER_Voice_Call_Client_Init
2, QSER_Voice_Call_AddStateHandler(pf_cb)
3, wait for new call arrive, pf_cb will pass the call_id to app.
4, QSER_Voice_Call_Answer(call_id)
5, QSER_Voice_Call_End
6, QSER_Voice_Call_Client_Deinit


Usage 2 (call out):
1, QSER_Voice_Call_Client_Init
2, QSER_Voice_Call_AddStateHandler(pf_cb)
3, QSER_Voice_Call_Start
4, QSER_Voice_Call_End
5, QSER_Voice_Call_Client_Deinit


*/

#ifdef ECALL_SUPPORT

#define QSER_MSD_MAX_LENGTH  140

typedef enum{ 
    E_QSER_VOICE_ECALL_SET_NUMBER = 0,     /* Set Number */
    E_QSER_VOICE_ECALL_SET_URI = 1,      /* Set Uri */   
}E_QSER_VOICE_ECALL_SET_TYPE_T; 

typedef enum{
    E_QSER_VOICE_ECALL_CAT_MANUAL = 0,      /* Manual Emergency eCall */
    E_QSER_VOICE_ECALL_CAT_AUTO = 1,     /*  Automatic Emergency eCall */
}E_QSER_VOICE_ECALL_CATEGORY_T;


typedef enum{
    E_QSER_VOICE_ECALL_TEST = 0,     /* Test eCall */
    E_QSER_VOICE_ECALL_EMERGENCY = 1,      /* Emergency eCall */
    E_QSER_VOICE_ECALL_RECONFIG = 2,     /*  Reconfiguration eCall */
}E_QSER_VOICE_ECALL_VARIANT_T;

typedef enum{
    E_QSER_VOICE_ECALL_IND_SENDING_START = 1,
    E_QSER_VOICE_ECALL_IND_SENDING_MSD = 2,
    E_QSER_VOICE_ECALL_IND_LLACK_RECEIVED = 3,
    E_QSER_VOICE_ECALL_IND_ALACK_POSITIVE_RECEIVED = 4,
    E_QSER_VOICE_ECALL_IND_ALACK_CLEARDOWN_RECEIVED = 5,
    E_QSER_VOICE_ECALL_IND_DAILING = 9,
    E_QSER_VOICE_ECALL_IND_ALERTING = 10,
    E_QSER_VOICE_ECALL_IND_ACTIVE = 11,
    E_QSER_VOICE_ECALL_IND_DISCONNECTED = 12,
    E_QSER_VOICE_ECALL_IND_IMS_ACTIVE = 13,
    E_QSER_VOICE_ECALL_IND_IMS_DISCONNECTED = 14,
    E_QSER_VOICE_ECALL_IND_ABNORMAL_HANGUP=15,
    E_QSER_VOICE_ECALL_IND_IMS_MSD_ACK = 20,
    E_QSER_VOICE_ECALL_IND_IMS_UPDATE_MSD = 21,
    E_QSER_VOICE_ECALL_IND_IMS_IN_BAND_TRANSFER = 22,
    E_QSER_VOICE_ECALL_IND_IMS_MSD_NACK = 23,
    E_QSER_VOICE_ECALL_IND_IMS_SRVCC = 24,
    E_QSER_VOICE_ECALL_IND_ONLY_DEREGISTRATION = 31,
    E_QSER_VOICE_ECALL_IND_MAY_DEREGISTER = 32,
    E_QSER_VOICE_ECALL_IND_PSAP_CALLBACK_START = 40,
    E_QSER_VOICE_ECALL_IND_PSAP_CALLBACK_IMS_UPDATE_MSD = 41,
    E_QSER_VOICE_ECALL_IND_SENDING_START_IN_VOICE=8000,    
    E_QSER_VOICE_ECALL_IND_T2_TIMER_OUT = 9000,
    E_QSER_VOICE_ECALL_IND_T5_TIMER_OUT = 9001,
    E_QSER_VOICE_ECALL_IND_T6_TIMER_OUT = 9002,
    E_QSER_VOICE_ECALL_IND_T7_TIMER_OUT = 9003,
    E_QSER_VOICE_ECALL_IND_REDIAL_TIMER_OUT = 9004,    
    E_QSER_VOICE_ECALL_IND_AUTO_ANS_TIMER_OUT = 9005,
    E_QSER_VOICE_ECALL_IND_AUTO_ANS_IMS_TIMER_OUT = 9006,
    E_QSER_VOICE_ECALL_IND_UNSPECIFIED = 0xffff,
}E_QSER_VOICE_ECALL_INDICATION_T;

typedef void (*QSER_ECall_IndHandlerFunc_t)
(
    int                     callid,   
    E_QSER_VOICE_ECALL_INDICATION_T ind,
    void                    *contextPtr
);

int qser_voice_set_test_num(voice_client_handle_type*       h_voice,E_QSER_VOICE_ECALL_SET_TYPE_T type, const char *test_num, int test_num_length);
int qser_voice_fast_ecall(voice_client_handle_type*       h_voice,
                        int                         *call_id,
                        E_QSER_VOICE_ECALL_CATEGORY_T cat,
                        E_QSER_VOICE_ECALL_VARIANT_T variant,
                        const char *addr, 
                        int addr_length, 
                        const unsigned char *msd_data,
                        int msd_length);  //msd_length should <= QSER_MSD_MAX_LENGTH
int qser_voice_set_msd( int callid, const unsigned char *msd_data, int msd_length); // msd_length should <= QSER_MSD_MAX_LENGTH
int qser_voice_add_ecall_indhandler(voice_client_handle_type*          h_voice,
                                  QSER_ECall_IndHandlerFunc_t   handlerPtr,
                                  void*                             contextPtr);

#endif
    
#ifdef __cplusplus
}
#endif

#endif // __QSER_VOICE_H__

