/*******************************************************
* 
* @brief:   none
* @details:  add libpoweralarm api
* @author:   l.yang
* @date:     2023.8.21
* @version:  V1.0
* @copyright：Copyright (c) MobileTek 
*
*********************************************/

#ifndef _LYNQ_POWERALARM_
#define _LYNQ_POWERALARM_
#ifdef __cplusplus
    extern "C" {
#endif

typedef int (*lynq_wakealarm_add_cb)(unsigned int src_id, int rtc_id);
int lynq_rtc_service_init(void);
int lynq_rtc_service_deinit(void);
ssize_t poweralarm(char *buffer,int src_id);
ssize_t wakealarm(char *buffer,int src_id,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify );
ssize_t cancel_wakealarm(int src_id, int rtc_id);

int lynq_set_wakealarm(unsigned long time_sec,int src_id,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify );
int  lynq_set_poweralarm(unsigned long time_sec,int src_id);




#ifdef __cplusplus
}
#endif
#endif


