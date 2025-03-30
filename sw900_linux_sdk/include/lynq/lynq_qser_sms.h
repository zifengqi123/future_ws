#ifndef LYNQ_QSER_SMS_H
#define LYNQ_QSER_SMS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <stdint.h>

#define MIN_MSM_PARAM_NUM 4
#define MIN_IMS_MSM_PARAM_NUM 6
#define MIN_WRITMSM_PARAM_NUM 5
#define MSG_MAX_LEN 1024
#define TELEPHONNUM_LEN 64
#define STORAGSMS_MAX_SIZE 128
#define SMSC_MAX_LEN 22
#define SMS_NUM_MAX 255

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef uint32_t sms_client_handle_type;

/**  Maximum length of an SMS. */
#define QSER_SMS_MAX_MT_MSG_LENGTH    1440

/**  Maximum string length. */
#define QSER_SMS_MAX_ADDR_LENGTH      252

/**  Maximum string length. */
#define QSER_SMS_MAX_SCA_TYPLENGTH 3

typedef enum   
{    
    QSER_SMS_7BIT        = 0,
    QSER_SMS_8BIT     = 1,
    QSER_SMS_UCS2            = 2,
    //<2017/12/28-QCM9XOL00004C001-P01-Vicent.Gao, <[SMS] Segment 1==> CharSet to Alpha implementation.>
    //QSER_SMS_IRA             = 3,//just for inside test 
    //>2017/12/28-QCM9XOL00004C001-P01-Vicent.Gao

}QSER_SMS_T;   

typedef enum   
{    
    QSER_SMS_MO               = 0,    ///< SMS mobile terminated message.  
    QSER_SMS_MT               = 1,    ///< SMS mobile originated message. 
    QSER_SMS_BROADCAST_MT      = 2     ///< SMS Cell Broadcast message.   
}QSER_SMS_TYPT; 

typedef enum 
{
    QSER_SMS_STORAGTYPNONE      = -1,   /**<  Message no need to store. */
    QSER_SMS_STORAGTYPUIM       = 0,    /**<  Message store to UIM. */
    QSER_SMS_STORAGTYPNV        = 1,    /**<  Message store to NV. */
    QSER_SMS_STORAGTYPDB     = 2,    /**<  Message store to NV. */
}QSER_SMS_STORAGTYPT;

typedef enum 
{
    QSER_SMS_MESSAGMODUNKNOWN   = -1,   /**<  Message type CDMA */
    QSER_SMS_MESSAGMODCDMA      = 0,    /**<  Message type CDMA */
    QSER_SMS_MESSAGMODGW        = 1,    /**<  Message type GW. */
}QSER_SMS_MODTYPT;

typedef struct 
 {
    uint8_t total_segments;     /**<     The number of long  short message*/
    uint8_t seg_number;         /**<     Current number.*/
	uint8_t referencnumber;   /**< referencnumber.*/
}QSER_sms_user_data_head_t; 

typedef struct
{
    /* If sms is stored, it won't parse, you need read it by yourself */
    QSER_SMS_STORAGTYPT storage;                          ///specify where stored this msg
    
    QSER_SMS_T       format;
    QSER_SMS_TYPT         type;
    char                    src_addr[QSER_SMS_MAX_ADDR_LENGTH]; ///Telephone number string.  
    int                     sms_data_len;
    char                    sms_data[QSER_SMS_MAX_MT_MSG_LENGTH];   ///SMS content, data format depends on format
    char                    timestamp[21];                      ///Message time stamp (in text mode). string format: "yy/MM/dd,hh:mm:ss+/-TimeZone" 
    uint8_t                 user_data_head_valid;               //indicate whether long sms. TRUE-long sms; FALSE-short message;
    QSER_sms_user_data_head_t  user_data_head;                    //long sms user data head info.
    QSER_SMS_MODTYPT    mode;                             ///specify where stored this msg cdma or gw area
    uint32_t                storage_index;                      ///storage index, -1 means not store
} QSER_sms_info_t;
     
typedef struct
{
    QSER_SMS_STORAGTYPT storage;
    QSER_SMS_MODTYPT    mode;
    uint32_t                storage_idx;
} QSER_sms_storage_info_t;

typedef enum 
{
    QSER_SMS_UNKNOWN            = -1, 
    QSER_SMS_DISCARD            = 0x00, /*  Incoming messages for this route are discarded by the WMS service without 
                                            notifying QMI_WMS clients */
    QSER_SMS_STORAND_NOTIFY   = 0x01, /*  Incoming messages for this route are stored to the specified device 
                                            memory, and new message notifications */
    QSER_SMS_TRANSFER_ONLY      = 0x02, /*  Incoming messages for this route are transferred to the client, and the
                                            client is expected to send ACK to the network */
    QSER_SMS_TRANSFER_AND_ACK   = 0x03, /*  Incoming messages for this route are transferred to the client, and ACK is
                                            sent to the network */
}QSER_SMS_RECEPTION_ACTION_TYPT;

#define QSER_WMS_MESSAGLENGTH_MAX 255

typedef enum 
 {
    QSER_WMS_MESSAGCDMA            = 0x00,     //- 0x00 -- MESSAGCDMA -- CDMA \n 
    QSER_WMS_MESSAGGW_PP           = 0x06,     //- 0x06 -- MESSAGGW_PP -- GW_PP
}QSER_WMS_MESSAGTYPE;


typedef struct
 {
  QSER_WMS_MESSAGTYPE format;
  uint32_t raw_messaglen;                             /**< Must be set to # of elements in raw_message */
  uint8_t raw_message[QSER_WMS_MESSAGLENGTH_MAX];       /**< Raw message data*/
}QSER_wms_send_raw_message_data_t; 

typedef enum
{
  QSER_WMS_TL_CAUSCODADDR_VACANT                        = 0x00, 
  QSER_WMS_TL_CAUSCODADDR_TRANSLATION_FAILURE           = 0x01, 
  QSER_WMS_TL_CAUSCODNETWORK_RESOURCSHORTAGE          = 0x02, 
  QSER_WMS_TL_CAUSCODNETWORK_FAILURE                    = 0x03, 
  QSER_WMS_TL_CAUSCODINVALID_TELESERVICID             = 0x04, 
  QSER_WMS_TL_CAUSCODNETWORK_OTHER                      = 0x05, 
  QSER_WMS_TL_CAUSCODNO_PAGRESPONSE                   = 0x20, 
  QSER_WMS_TL_CAUSCODDEST_BUSY                          = 0x21, 
  QSER_WMS_TL_CAUSCODNO_ACK                             = 0x22, 
  QSER_WMS_TL_CAUSCODDEST_RESOURCSHORTAGE             = 0x23, 
  QSER_WMS_TL_CAUSCODSMS_DELIVERY_POSTPONED             = 0x24, 
  QSER_WMS_TL_CAUSCODDEST_OUT_OF_SERV                   = 0x25, 
  QSER_WMS_TL_CAUSCODDEST_NOT_AT_ADDR                   = 0x26, 
  QSER_WMS_TL_CAUSCODDEST_OTHER                         = 0x27, 
  QSER_WMS_TL_CAUSCODRADIO_IF_RESOURCSHORTAGE         = 0x40, 
  QSER_WMS_TL_CAUSCODRADIO_IF_INCOMPATABILITY           = 0x41, 
  QSER_WMS_TL_CAUSCODRADIO_IF_OTHER                     = 0x42, 
  QSER_WMS_TL_CAUSCODENCODING                           = 0x60, 
  QSER_WMS_TL_CAUSCODSMS_ORIG_DENIED                    = 0x61, 
  QSER_WMS_TL_CAUSCODSMS_TERM_DENIED                    = 0x62, 
  QSER_WMS_TL_CAUSCODSUPP_SERV_NOT_SUPP                 = 0x63, 
  QSER_WMS_TL_CAUSCODSMS_NOT_SUPP                       = 0x64, 
  QSER_WMS_TL_CAUSCODMISSING_EXPECTED_PARAM             = 0x65, 
  QSER_WMS_TL_CAUSCODMISSING_MAND_PARAM                 = 0x66, 
  QSER_WMS_TL_CAUSCODUNRECOGNIZED_PARAM_VAL             = 0x67, 
  QSER_WMS_TL_CAUSCODUNEXPECTED_PARAM_VAL               = 0x68, 
  QSER_WMS_TL_CAUSCODUSER_DATA_SIZERR                 = 0x69, 
  QSER_WMS_TL_CAUSCODGENERAL_OTHER                      = 0x6A, 
}QSER_WMS_TL_CAUSCODTYPE;



typedef struct
 {
  uint16_t                              messagid;            /*  Message ID */
  uint8_t                               causcodvalid;      /**< Must be set to true if causcode is being passed */
  QSER_WMS_TL_CAUSCODTYPE           causcode;  
}QSER_wms_raw_send_resp_t;

typedef struct 
 {
  char service_center_addr[QSER_SMS_MAX_ADDR_LENGTH + 1];         /**<   Address of the service center.*/
  uint8_t service_center_addr_typvalid;
  char service_center_addr_type[QSER_SMS_MAX_SCA_TYPLENGTH + 1];    /**<   129 if the SMSC address does not start with a "+" characte;
                                                                           145 if the SMSC address starts with a "+" character*/
}QSER_sms_service_center_cfg_t;

typedef QSER_sms_info_t       QSER_SMS_Msg_t; 
typedef QSER_sms_info_t*      QSER_SMS_MsgRef; 

/* Callback function registered to qser_sms_addrxmsghandler, msgRef contains the detail msg infor */
typedef void (*QSER_SMS_RxMsgHandlerFunc_t)(QSER_SMS_MsgRef msgRef, void* contextPtr);

/* Init SMS module and return h_sms, this should be called before any othe ones */
int qser_sms_client_init(sms_client_handle_type  *ph_sms);

/* Add callback function, if any new msg arrived, it will notify app */
int qser_sms_addrxmsghandler(QSER_SMS_RxMsgHandlerFunc_t handlerPtr, void* contextPtr);

/* Send sms, you just need to fill format/src_addr/sms_data_len/sms_data,
   if format is UCS2, the data should be Unicode-BE format.
*/
int qser_sms_send_sms(sms_client_handle_type h_sms, QSER_sms_info_t *pt_sms_info);

/* DeInit SMS module and release resource, this should be called in the last one. */
int qser_sms_client_deinit(sms_client_handle_type h_sms);
     
/* Delete the SMS specified in the  pt_sms_storage */
int qser_sms_deletefromstorage(sms_client_handle_type  h_sms, QSER_sms_storage_info_t  *pt_sms_storage);

/* Send sms PDU.
*/
int qser_sms_send_smspdu( sms_client_handle_type     h_sms,
                         QSER_wms_send_raw_message_data_t *raw_messagdata,
                         QSER_wms_raw_send_resp_t *rawresp);


/* Get sms center address.
*/
int qser_sms_getsmscenteraddress( sms_client_handle_type     h_sms,
                       QSER_sms_service_center_cfg_t *set_sca_cfg);

/* Set sms center address.
*/
int qser_sms_setsmscenteraddress( sms_client_handle_type     h_sms,
                       QSER_sms_service_center_cfg_t *get_sca_cfg);

/*
Usage 1 (register callback and wait for new sms arrive): 
1, qser_sms_client_init
2, qser_sms_addrxmsghandler(pf_cb)
3, wait for sms arrive, pf_cb will pass the detail sms info to app.
4, qser_sms_client_deinit


Usage 2 (Send sms): 
1, qser_sms_client_init
2, qser_sms_addrxmsghandler(pf_cb)
3, qser_sms_send_sms
4, qser_sms_client_deinit


Usage 3 (store sms): 
1, qser_sms_client_init
2, qser_sms_addrxmsghandler(pf_cb)
3, qser_sms_setroutelist (Notice: class-0 not allowed to store, class-2 must be stored to UIM)
4, pf_cb will be called when new sms arrived, and tell you where the sms stored
5, you can call qser_sms_deletefromstorage to delete specified sms, the location can be got from pf_cb.
6, qser_sms_client_deinit
*/
    
#ifdef __cplusplus
}
#endif
#endif