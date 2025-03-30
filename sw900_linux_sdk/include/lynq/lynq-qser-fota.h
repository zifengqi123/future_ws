/*******************************************************
* 
* @brief:
* @details:  add fota upgrade api
* @author:   l.yang
* @date:     2023.8.3
* @version:  V1.0
* @copyright：Copyright (c) MobileTek 
*
*********************************************/
#ifndef LYNQ_QSER_FOTA
#define LYNQ_QSER_FOTA 
#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
* @brief get reboot upgrade status 
  @param 
    NULL
  @return 
        if reboot the other system successfully return 1, else return 0
 *******************************************************************************/
int lynq_get_reboot_upgrade_status(void);
int lynq_get_upgrade_status(void);
int lynq_fota_set_addr_value(char        *value,int size);
int lynq_fota_nrestart(void);
int lynq_rock_main(int first_run);
int lynq_read_process(void);

#ifdef __cplusplus
}
#endif
#endif

