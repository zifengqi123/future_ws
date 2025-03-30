#ifndef __LYNQ_QSER_SIM__
#define __LYNQ_QSER_SIM__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "mbtk_type.h"

#define QSER_SIM_IMSI_LEN_MAX     16  /**  Maximum length of IMSI data. */
#define QSER_SIM_ICCID_LEN_MAX    20  /**  Maximum length of ICCID data. */

typedef enum
{
    E_QSER_SUCCESS                        = 0,    /**<  Success. */
    E_QSER_ERROR_BADPARM                  = 4,    /**<  Bad parameter. */
}E_QSER_ERROR_CODE_T;

typedef uint32_t sim_client_handle_type;

typedef enum
{
    QSER_SIM_SLOT_ID_1          = 0xB01,    /**< Identify card in slot 1.  */
    QSER_SIM_SLOT_ID_2          = 0xB02,    /**< Identify card in slot 2.  */
}QSER_SIM_SLOT_ID_TYPE_T;

typedef enum
{
    QSER_SIM_APP_TYPE_UNKNOWN   = 0xB00,    /**<  Unknown application type  */
    QSER_SIM_APP_TYPE_3GPP      = 0xB01,    /**< Identify the SIM/USIM application on the card.  */
    QSER_SIM_APP_TYPE_3GPP2     = 0xB02,    /**< Identify the RUIM/CSIM application on the card.  */
    QSER_SIM_APP_TYPE_ISIM      = 0xB03,    /**< Identify the ISIM application on the card.  */
}QSER_SIM_APP_TYPE_T;

typedef struct
{
    QSER_SIM_SLOT_ID_TYPE_T     e_slot_id;  /**< Indicates the slot to be used. */
    QSER_SIM_APP_TYPE_T         e_app;      /**< Indicates the type of the application. */
}QSER_SIM_APP_ID_INFO_T;  /* Type */

#define QSER_SIM_PIN_LEN_MAX  8 + 1   /**  Maximum length of PIN data. */

typedef enum
{
    QSER_SIM_PIN_ID_1 = 0xB01,  /**< Level 1 user verification.  */
    QSER_SIM_PIN_ID_2 = 0xB02,  /**< Level 2 user verification.  */
}QSER_SIM_PIN_ID_TYPE_T;

typedef struct
{
    QSER_SIM_APP_ID_INFO_T        app_info;                           /**< Application identification information. */
    QSER_SIM_PIN_ID_TYPE_T  pin_id;                             /**< PIN ID. */
    uint32_t                    pin_value_len;                      /**< Must be set to the number of elements in pin_value. */
    char                        pin_value[QSER_SIM_PIN_LEN_MAX];  /*  Value of the PIN */
}QSER_SIM_VERIFY_PIN_INFO_T;

/** Changes the PIN value of an application. The application must pass both the
    new and the old values of the PIN to complete the operation.
    The same PIN can be used by multiple sessions (i.e., the PIN is shared
    between GSM and RUIM in an ICC card). The PIN is automatically verified
    for all the sessions when the command is executed. */
typedef struct
{
    QSER_SIM_APP_ID_INFO_T        app_info;                               /**< Application identification information. */
    QSER_SIM_PIN_ID_TYPE_T  pin_id;                                 /**< PIN ID. */
    uint32_t                    old_pin_value_len;                      /**< Must be set to the number of elements in old_pin_value. */
    char                        old_pin_value[QSER_SIM_PIN_LEN_MAX];  /**< Value of the old PIN as a sequence of ASCII characters. */
    uint32_t                    new_pin_value_len;                      /**< Must be set to the number of elements in new_pin_value. */
    char                        new_pin_value[QSER_SIM_PIN_LEN_MAX];  /**< Value of the new PIN as a sequence of ASCII characters. */
}QSER_SIM_CHANGE_PIN_INFO_T;

/** Unblocks a blocked PIN using the PUK code. The client must pass PUK1 to unblock PIN1 or PUK2 to unblock PIN2.
    The same PIN can be used by multiple sessions (i.e., the PIN is shared between GSM and RUIM in an ICC card).
    The PIN is automatically verified for all the sessions when the command is executed. */
typedef struct
{
    QSER_SIM_APP_ID_INFO_T        app_info;                               /**< Application identification information. */
    QSER_SIM_PIN_ID_TYPE_T  pin_id;                                 /**< PIN ID. */
    uint32_t                    puk_value_len;                          /**< Must be set to the number of elements in puk_value. */
    char                        puk_value[QSER_SIM_PIN_LEN_MAX];      /**< Value of the PUK as a sequence of ASCII characters. */
    uint32_t                    new_pin_value_len;                      /**< Must be set to the number of elements in new_pin_value. */
    char                        new_pin_value[QSER_SIM_PIN_LEN_MAX];  /**< Value of the new PIN as a sequence of ASCII characters. */
}QSER_SIM_UNBLOCK_PIN_INFO_T;

/** Enables the PIN on an application. */
typedef  QSER_SIM_VERIFY_PIN_INFO_T QSER_SIM_ENABLE_PIN_INFO_T; //Same

/** Disables the PIN of an application, */
typedef  QSER_SIM_VERIFY_PIN_INFO_T QSER_SIM_DISABLE_PIN_INFO_T; //Same


typedef enum
{
    QSER_SIM_PERSO_FEATURE_TYPE_UNKNOWN                 = 0xB00, /**<  Unknown personalization feature.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP_NETWORK            = 0xB01, /**<  Featurization based on 3GPP MCC and MNC.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP_NETWORK_SUBSET     = 0xB02, /**<  Featurization based on 3GPP MCC, MNC, and IMSI digits 6 and 7.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP_SERVICE_PROVIDER   = 0xB03, /**<  Featurization based on 3GPP MCC, MNC, and GID1.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP_CORPORATE          = 0xB04, /**<  Featurization based on 3GPP MCC, MNC, GID1, and GID2.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP_SIM                = 0xB05, /**<  Featurization based on the 3GPP IMSI.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP2_NETWORK_TYPE_1    = 0xB06, /**<  Featurization based on 3GPP2 MCC and MNC.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP2_NETWORK_TYPE_2    = 0xB07, /**<  Featurization based on 3GPP2 IRM code.  */
    QSER_SIM_PERSO_FEATURE_TYPE_3GPP2_RUIM              = 0xB08, /**<  Featurization based on 3GPP2 IMSI_M.  */
}QSER_SIM_PERSO_FEATURE_TYPE_T;

typedef enum
{
    QSER_SIM_CARD_STATE_UNKNOWN                     = 0xB01,    /**< Card state unknown. */
    QSER_SIM_CARD_STATE_ABSENT                      = 0xB02,    /**< Card is absent. */
    QSER_SIM_CARD_STATE_PRESENT                     = 0xB03,    /**< Card is present. */
    QSER_SIM_CARD_STATE_ERROR_UNKNOWN               = 0xB04,    /**< Unknown error state. */
    QSER_SIM_CARD_STATE_ERROR_POWER_DOWN            = 0xB05,    /**< Power down. */
    QSER_SIM_CARD_STATE_ERROR_POLL_ERROR            = 0xB06,    /**< Poll error. */
    QSER_SIM_CARD_STATE_ERROR_NO_ATR_RECEIVED       = 0xB07,    /**<  Failed to receive an answer to reset.  */
    QSER_SIM_CARD_STATE_ERROR_VOLT_MISMATCH         = 0xB08,    /**< Voltage mismatch. */
    QSER_SIM_CARD_STATE_ERROR_PARITY_ERROR          = 0xB09,    /**< Parity error. */
    QSER_SIM_CARD_STATE_ERROR_SIM_TECHNICAL_PROBLEMS= 0xB0A,    /**< Card returned technical problems. */
}QSER_SIM_CARD_STATE_TYPE_T;  /**< Card state. */

typedef enum
{
    QSER_SIM_CARD_TYPE_UNKNOWN  = 0xB00,    /**<  Unidentified card type.  */
    QSER_SIM_CARD_TYPE_ICC      = 0xB01,    /**<  Card of SIM or RUIM type.  */
    QSER_SIM_CARD_TYPE_UICC     = 0xB02,    /**<  Card of USIM or CSIM type.  */
}QSER_SIM_CARD_TYPE_T;

typedef enum
{
    QSER_SIM_PROV_STATE_NONE    = 0xB00,    /**<  Nonprovisioning.  */
    QSER_SIM_PROV_STATE_PRI     = 0xB01,    /**<  Primary provisioning subscription.  */
    QSER_SIM_PROV_STATE_SEC     = 0xB02,    /**<  Secondary provisioning subscription.  */
}QSER_SIM_SUBSCRIPTION_TYPE_T;

typedef enum
{
    QSER_SIM_APP_STATE_UNKNOWN                  = 0xB00,    /**< Application state unknown. */
    QSER_SIM_APP_STATE_DETECTED                 = 0xB01,    /**<  Detected state.  */
    QSER_SIM_APP_STATE_PIN1_REQ                 = 0xB02,    /**<  PIN1 required.  */
    QSER_SIM_APP_STATE_PUK1_REQ                 = 0xB03,    /**<  PUK1 required.  */
    QSER_SIM_APP_STATE_INITALIZATING            = 0xB04,    /**<  Initializing.  */
    QSER_SIM_APP_STATE_PERSO_CK_REQ             = 0xB05,    /**<  Personalization control key required.  */
    QSER_SIM_APP_STATE_PERSO_PUK_REQ            = 0xB06,    /**<  Personalization unblock key required.  */
    QSER_SIM_APP_STATE_PERSO_PERMANENTLY_BLOCKED= 0xB07,    /**<  Personalization is permanently blocked.  */
    QSER_SIM_APP_STATE_PIN1_PERM_BLOCKED        = 0xB08,    /**<  PIN1 is permanently blocked.  */
    QSER_SIM_APP_STATE_ILLEGAL                  = 0xB09,    /**<  Illegal application state.  */
    QSER_SIM_APP_STATE_READY                    = 0xB0A,    /**<  Application ready state.  @newpage */
}QSER_SIM_APP_STATE_TYPE_T;

typedef enum
{
    QSER_SIM_PIN_STATE_UNKNOWN                  = 0xB01,    /**< Unknown PIN state. */
    QSER_SIM_PIN_STATE_ENABLED_NOT_VERIFIED     = 0xB02,    /**<  PIN required, but has not been verified.  */
    QSER_SIM_PIN_STATE_ENABLED_VERIFIED         = 0xB03,    /**<  PIN required and has been verified.  */
    QSER_SIM_PIN_STATE_DISABLED                 = 0xB04,    /**<  PIN not required.  */
    QSER_SIM_PIN_STATE_BLOCKED                  = 0xB05,    /**<  PIN verification has failed too many times and is blocked. Recoverable through PUK verification.  */
    QSER_SIM_PIN_STATE_PERMANENTLY_BLOCKED      = 0xB06,    /**<  PUK verification has failed too many times and is not recoverable.  */
}QSER_SIM_PIN_STATE_TYPE_T;

typedef struct
{
    QSER_SIM_SUBSCRIPTION_TYPE_T      subscription;           /**<   Type of subscription (i.e., primary, secondary, etc.). */
    QSER_SIM_APP_STATE_TYPE_T         app_state;              /**<   Current state of the application. */
    QSER_SIM_PERSO_FEATURE_TYPE_T   perso_feature;          /**<   Current personalization state and feature enabled. */
    uint8_t                             perso_retries;          /**<   Number of personalization retries. */
    uint8_t                             perso_unblock_retries;  /**<   Number of personalization unblock retries. */
    QSER_SIM_PIN_STATE_TYPE_T         pin1_state;             /**<   Current PIN 1 state. */
    uint8_t                             pin1_num_retries;       /**<   Number of PIN 1 retries. */
    uint8_t                             puk1_num_retries;       /**<   Number of PUK 1 retries. */
    QSER_SIM_PIN_STATE_TYPE_T         pin2_state;             /**<   Current PIN 2 state. */
    uint8_t                             pin2_num_retries;       /**<   Number of PIN 2 retries. */
    uint8_t                             puk2_num_retries;       /**<   Number of PUK 2 retries. */
}QSER_SIM_CARD_APP_INFO_T;

typedef struct
{
    QSER_SIM_CARD_APP_INFO_T          app_3gpp;               /**<   Stores 3GPP application information. */
    QSER_SIM_CARD_APP_INFO_T          app_3gpp2;              /**<   Stores 3GPP2 application information. */
    QSER_SIM_CARD_APP_INFO_T          app_isim;               /**<   Stores ISIM application information. */
}QSER_SIM_CARD_ALL_APP_INFO_T;

typedef struct
{
    QSER_SIM_CARD_STATE_TYPE_T      e_card_state;/**<   Current card and card error state. */
    QSER_SIM_CARD_TYPE_T            e_card_type; /**<   Card type. */
    QSER_SIM_CARD_ALL_APP_INFO_T      card_app_info; /**<   Stores all relevant application information. */
}QSER_SIM_CARD_STATUS_INFO_T;

typedef QSER_SIM_CARD_STATUS_INFO_T*      QSER_SIM_MsgRef; 
/* Callback function registered to qser_sms_addrxmsghandler, msgRef contains the detail msg infor */
typedef void (*QSER_SIM_RxMsgHandlerFunc_t)(QSER_SIM_MsgRef msgRef);


int qser_sim_client_init(sim_client_handle_type  *ph_sim);

int qser_sim_client_deinit(sim_client_handle_type h_sim);

/*===========================================================================

  FUNCTION:  qser_sim_getimsi

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to retrieve the IMSI (for 3GPP)
    or IMSI_M (for 3GPP2) from the SIM in ASCII form

    @return
    void
*/
/*=========================================================================*/
int qser_sim_getimsi(
    sim_client_handle_type          h_sim,
    QSER_SIM_APP_ID_INFO_T            *pt_info,   ///< [IN] The SIM identifier info.
    char                            *imsi,      ///< [OUT] IMSI buffer
    size_t                          imsiLen     ///< [IN] IMSI buffer length
);


/*===========================================================================

  FUNCTION:  qser_sim_geticcid

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to retrieve the ICCID from
    SIM in ASCII form

    @return
    int
*/
/*=========================================================================*/
int qser_sim_geticcid
(
    sim_client_handle_type          h_sim,
    QSER_SIM_SLOT_ID_TYPE_T     simId,     ///< [IN] The SIM identifier.
    char                            *iccid,    ///< [OUT] ICCID
    size_t                          iccidLen   ///< [IN] ICCID buffer length
);


/*===========================================================================

  FUNCTION:  qser_sim_getphonenumber

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to retrieve the device phone
    number from MSISDN (for 3GPP) or MDN (for 3GPP2) from the SIM in
    ASCII form

    @return
    int
*/
/*=========================================================================*/
int qser_sim_getphonenumber
(
    sim_client_handle_type          h_sim,
    QSER_SIM_APP_ID_INFO_T            *pt_info,   ///< [IN] The SIM identifier.
    char                            *phone_num, ///< [OUT] phone number
    size_t                          phoneLen    ///< [IN] phone number buffer length
);

/*===========================================================================

  FUNCTION:  qser_sim_verifypin

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to verify either PIN1 or PIN2

    @return
    int
*/
/*=========================================================================*/
int qser_sim_verifypin
(
    sim_client_handle_type      h_sim,
    QSER_SIM_VERIFY_PIN_INFO_T    *pt_info   ///< [IN] Verify PIN infor
);


/*===========================================================================

  FUNCTION:  qser_sim_changepin

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to change the value of
    either PIN1 or PIN2

    @return
    int
*/
/*=========================================================================*/
int qser_sim_changepin
(
    sim_client_handle_type      h_sim,
    QSER_SIM_CHANGE_PIN_INFO_T    *pt_info   ///< [IN] Change PIN infor
);


/*===========================================================================

  FUNCTION:  qser_sim_unblockpin

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to unblock a PIN1 or PIN2 that
    has been blocked

    @return
    int
*/
/*=========================================================================*/
int qser_sim_unblockpin
(
    sim_client_handle_type      h_sim,
    QSER_SIM_UNBLOCK_PIN_INFO_T   *pt_info   ///< [IN] Unblock PIN infor
);

/*===========================================================================

  FUNCTION:  qser_sim_enablepin

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to enable PIN1 or PIN2

    @return
    int
*/
/*=========================================================================*/
int qser_sim_enablepin
(
    sim_client_handle_type      h_sim,
    QSER_SIM_ENABLE_PIN_INFO_T    *pt_info   ///< [IN] Enable PIN infor
);

/*===========================================================================

  FUNCTION:  qser_sim_disablepin

===========================================================================*/
/*
    @brief
    Function sends a command to the modem to disable PIN1 or PIN2

    @return
    int
*/
/*=========================================================================*/
int qser_sim_disablepin
(
    sim_client_handle_type      h_sim,
    QSER_SIM_DISABLE_PIN_INFO_T   *pt_info   ///< [IN] Disable PIN infor
);


/*===========================================================================

  FUNCTION:  qser_sim_getcardstatus

===========================================================================*/
/*
    @brief
    Function retrieves the server cached card status informations and
    sends the information to the client

    @return
    int
*/
/*=========================================================================*/
int qser_sim_getcardstatus
(
    sim_client_handle_type          h_sim,
    QSER_SIM_SLOT_ID_TYPE_T     simId,     ///< [IN] The SIM identifier.
    QSER_SIM_CARD_STATUS_INFO_T   *pt_info   ///< [OUT] Cart status infor output
);

/*===========================================================================

  FUNCTION:  qser_sim_getimei

===========================================================================*/
/*
    @brief
    get imei

    @return
    int
*/
/*=========================================================================*/
int qser_sim_getimei(sim_client_handle_type h_sim, char *imei);
int qser_get_imei_and_sv(sim_client_handle_type h_sim,char *imei, char*sv);


/*===========================================================================

  FUNCTION:  qser_get_imei_and_sv

===========================================================================*/
/*
    @brief
    get imei and sv

    @return
    int
*/
/*=========================================================================*/
int qser_get_imei_and_sv
(
    sim_client_handle_type h_sim,
    char *imei, char*sv
);

/*===========================================================================

  FUNCTION:  qser_reset_modem

===========================================================================*/
/*
    @brief
    reboot modem

    @return
    int
*/
/*=========================================================================*/
int qser_reset_modem(sim_client_handle_type h_sim);


/*===========================================================================

  FUNCTION:  qser_reset_modem

===========================================================================*/
/*
    @brief
    reboot modem

    @return
    int
*/
/*=========================================================================*/
int qser_get_version(sim_client_handle_type h_sim, char *buf);


/*===========================================================================

  FUNCTION:  qser_reset_modem

===========================================================================*/
/*
    @brief
    reboot modem

    @return
    int
*/
/*=========================================================================*/
int qser_reset_sim(sim_client_handle_type h_sim);

int qser_sim_addrxmsghandler(QSER_SIM_RxMsgHandlerFunc_t handlerPtr);

#ifdef __cplusplus
}
#endif

#endif