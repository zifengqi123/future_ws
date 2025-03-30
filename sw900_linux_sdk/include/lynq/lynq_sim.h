/*=============================================================================
#     FileName: lynq_sim.h
#     Desc: about SIMAPI
#     Author: lei
#     Version: V2.0
#     LastChange: 2023-03-13
#     History:
# If you need to use any API under lynq_sim, you must first call the lynq_sim_init() function to initialize these functions.
=============================================================================*/
#ifndef __LYNQ_SIM__
#define __LYNQ_SIM__
#ifdef __cplusplus
extern "C" {
#endif
#include "mbtk_type.h"

int lynq_get_sim_status(int *card_status);
int lynq_get_imsi(char buf[]);
int lynq_sim_init(int utoken);
int lynq_sim_deinit(void);
int lynq_enable_pin(char pin[]);
int lynq_disable_pin(char pin[]);
int lynq_get_iccid(char buf[]);
int lynq_query_pin_lock(char *pin,int buf[]);
int lynq_verify_pin(char *pin);
int lynq_change_pin(char *old_pin, char *new_pin);
int lynq_unlock_pin(char *puk, char *pin);
int lynq_query_phone_number(char buf[]);
int lynq_switch_card(int slot);
int lynq_screen(int num);
int lynq_get_imei(char buf[]);
int lynq_get_imei_and_sv(char imei[],char sv[]);

/**
 * @brief                   Request SIM I/O operation.
 *                          This is similar to the TS 27.007 "restricted SIM" operation
 *                          where it assumes all of the EF selection will be done by the callee.
 * @param  list             type: [IN] list[0]:one of the commands listed for TS 27.007 +CRSM.(command)
 *                          type: [IN] list[1]:EF id(fileid)
 *                          type: [IN] list[2]:offset(p1)
 *                          type: [IN] list[3]:offset(p2)
 *                          type: [IN] list[4]:response len,sometimes needn't care(p3)
 * @param  path             type: [IN] "pathid" from TS 27.007 +CRSM command.
                            type: [IN] Path is in hex asciii format eg "7f205f70"
                            type: [IN] Path must always be provided.
 * @param  data             type: [IN] May be NULL
 * @param  pin2             type: [IN] May be NULL
 * @param  aidPtr           type: [IN] AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value.
 * @param  sw               type: [OUT]
 * @param  simResponse      type: [OUT] response
 * @return int
 */
int lynq_req_sim_io(int list[5], char *path, char *data, char *pin2, char *aidPtr, int sw[2], char *simResponse);

/**
 * @brief
 * @param  options         type: [IN] [options] define whether you want to halt, power-off, or reboot the machine.May be NULL
 * @param  time            type: [IN] [time] specifies when you want the shutdown to perform.May be NULL
 * @param  message         type: [IN] [message] adds a message that announces the shutdown.May be NULL
 * @return int
 */
int lynq_shutdown(char options[], char time[], char message[]);

/**
 * @brief                   get currnet version of mobiletek
 * @param  buf              type: [out] My Param doc
 * @return int
 */
int lynq_get_version(char buf[]);

/**
 * @brief sim power on/off
 *
 */
#ifdef MOBILETEK_TARGET_PLATFORM_T106
int lynq_reset_sim(void);
#else
int lynq_sim_power(int mode);
#endif
/**
 * @brief reset modem
 *
 */
int lynq_reset_modem(void);

/**
 * @brief factory test to both radio on/off
 *
 */
int lynq_factory_radio_state(int num);


#ifdef __cplusplus
}
#endif

#endif

