/**@file lynq-systime.h
 *
 * @brief Sync systime form each time source.
 *
 * @author sj.zhang
 *
 * @date 2023-08-14
 *
 * @version V1.0
 *
 * @copyright Copyright (c) MobileTek
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lynq_uci.h"

#ifndef _LYNQ_SYSTIME_H_
#define _LYNQ_SYSTIME_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct time_source_status
{
    int ntp;
    int nitz;
    int gnss;
}time_src_status_s;

#ifdef MOBILETEK_TARGET_PLATFORM_T106

    /**@brief Set the rtc time to the system time.
     *
     * @param void
     *
     * @return
     *      0:success
     *      other:failure
     */
    int lynq_sync_time_from_rtc(void);

    /**@brief Set the system time to the rtc time.
     *
     * @param void
     *
     * @return
     *      0:success
     *      other:failure
     */
    int lynq_set_rtc_time(void);

    /**@brief Query the rtc time.
     *
     * @param [OUT] ulsec: rtc time, secconds from 1900.1.1 00:00
     *
     * @return
     *      0:success
     *      other:failure
     */
    int lynq_get_rtc_time(unsigned long *ulsec);
#endif

    /**@brief Query the time source status now.
     *
     * @param [OUT] ntp, nitz, gnss:
     *        0: disable
     *        1: enable
     *
     * @return
     *      0 : success
     *      other:failure
     */
    int lynq_get_time_src_status(time_src_status_s *time_src);

    /**@brief Turn sntp on/off
     *
     * @param enable [IN] : 0  disenable; 1 enable;
     *
     * @return
     *    0 set success
     *    1 SNTP_DISENABLE
     *    2 SNTP_ALREADY_ENABLE
     */
    int ntp_sync_time(int enable);

    /**@brief Enable/Disenable sync time from NITZ.
     *
     * @param enable [IN] : 0  disenable; 1 enable;
     *
     * @return
     *      0:success
     *      other:failure
     */
    int modem_time_enable(int enable);

    /**@brief Enable/Disenable sync time from GNSS.
     *
     * @param enable [IN] : 0  disenable; 1 enable;
     *
     * @return
     *      0:success
     *      other:failure
     */
    int gnss_time_enable(int enable);

    /**@brief User set systime manually.
     *
     * @param enable [IN] : 0  disenable; 1 enable;
     *
     * @return
     *      0:success
     *      other:failure
     */
    int user_set_time(char *date, char *time);

#ifdef __cplusplus
}
#endif
#endif
