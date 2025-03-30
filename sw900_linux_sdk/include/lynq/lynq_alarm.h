/*
* qser_alarm.h
*
* QSER alarm header.
*
* Author : lb
* Date   : 2023/11/23 10:45:44
*/
#ifndef _QSER_ALARM_H
#define _QSER_ALARM_H
#include "mbtk_type.h"

typedef int (*lynq_wakealarm_add_cb)(unsigned int src_id, int rtc_id);
int lynq_rtc_service_init(void);
int lynq_rtc_service_deinit(void);
ssize_t poweralarm(char *buffer,int src_id);
ssize_t wakealarm(char *buffer,int src_id,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify );
ssize_t cancel_wakealarm(int src_id, int rtc_id);

int lynq_set_wakealarm(unsigned long time_sec,int src_id,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify );
int  lynq_set_poweralarm(unsigned long time_sec,int src_id);

#endif /* _QSER_ALARM_H */
