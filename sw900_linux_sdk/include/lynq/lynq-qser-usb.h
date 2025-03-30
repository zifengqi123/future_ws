/*******************************************************
* 
* @brief: Add usb api
* @details:  add liblynq-qser-usb api
* @author:   wz.wang
* @date:     2024.2.18
* @version:  V1.0
* @copyright：Copyright (c) MobileTek
*
*********************************************/
#ifndef LYNQ_QSER_USB
#define LYNQ_QSER_USB

#ifdef __cplusplus
extern "C" {
#endif

int  qser_get_usb_usermode_net_state(void);
int  qser_get_usb_usermode_adb_state(void);

#ifdef __cplusplus
}
#endif
#endif

