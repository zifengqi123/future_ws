#ifndef __MBTK_NTP_H__
#define __MBTK_NTP_H__


/*****************************************************************
* Function:     mbtk_at_systime
*
* Description:  
*               This function is used to get the time from ntp server "cn.pool.ntp.org"
*
* Parameters:
*               null
*
* Return:
*               time_t is success
*               0 is fail
*****************************************************************/
int mbtk_at_systime(void);

/*****************************************************************
* Function:     mbtk_ntp_server_set
*
* Description:  
*               This function is used to set the ip address and port of NTP server.
*               return the time obtained from ntp.
*
* Parameters:
*               server_ip  : NTP server
*
*		        port : ntp port
*
* Return:
*               time_t is success
*               0 is fail
*****************************************************************/
time_t mbtk_ntp_server_set(const char* server_ip, const char * port);


#endif