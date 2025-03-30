#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <time.h>
#include <sys/ioctl.h>
#include <dlfcn.h>
#include <stdint.h>
#include "lynq_gnss.h"

void cb(uint32_t h_loc, e_msg_id_t e_msg_id, void *pv_data, void *context_ptr)
{
    printf("e_msg_id=%d\n", e_msg_id);
    switch(e_msg_id)
    {
    case E_MT_LOC_MSG_ID_LOCATION_INFO:				//选择mode1，输出定位信息
    {
        mopen_location_info_t *pt_location = (mopen_location_info_t *)pv_data;
        printf("**** flag=0x%X, Latitude = %f, Longitude=%f, altitude = %f, speed = %f, timestamp = %lld ****\n",
               pt_location->flags,
               pt_location->latitude,
               pt_location->longitude,
               pt_location->altitude,
               pt_location->speed,
               pt_location->timestamp);
        break;
    }
    case E_MT_LOC_MSG_ID_NMEA_INFO:				//选择mode3，输出nmea数据信息
    {
        mopen_gnss_nmea_info_t  *pt_nmea = (mopen_gnss_nmea_info_t *)pv_data;

        printf("**** NMEA info: timestamp=%lld, length=%d, nmea=%s ****\n",
               pt_nmea->timestamp, pt_nmea->length, pt_nmea->nmea);
        break;
    }
    }
}

void async_cb(qser_gnss_error_e state)
{
    printf("start gnss async state: %d \n", state);
}

void user_help(void)
{
    printf("\t-1 exit\n"
           "\t1 gnss init\n"
           "\t2 gnss deinit \n"
           "\t3 gnss add nmea callback\n"
           "\t4 gnss set local mode\n"
           "\t5 gnss set nmea mode\n"
           "\t6 gnss start\n"
           "\t7 gnss stop\n"
           "\t8 gnss Delete_Aiding_Data and reset (new not support)\n"
           "\t9 gnss injecttime (new not support)\n"
           "\t10 gnss download agps\n"
           "\t11 gnss set frequency\n"
           "\t12 gnss send agps data to gnss\n"
           "\t13 gnss config agps server info\n"
           "\t14 gnss set async cb\n"
           "\t15 gnss start async\n"
           "please input operator: >> \n");
}
void delete_type(void)
{
    printf("\t0 DELETE_NOTHING\n"
           "\t1 DELETE_EPHEMERIS\n"
           "\t2 DELETE_ALMANAC\n"
           "\t3 DELETE_POSITION_TIME \n"
           "\t4 DELETE_UTC\n"
           "\t5 DELETE_ALL\n"
           "please input operator: >> \n");
}

int main(int argc, char *argv[])
{
    int ret;
    int opt = 0;
    //char *lynqLib_gnss = "/lib/liblynq-qser-gnss.so";
    char dev_file[12] = {0};
    uint32_t ph_gnss;
    //dlHandle_gnss = dlopen(lynqLib_gnss, RTLD_NOW);
    while(1)
    {
        printf("=========gnss main=========\n");
        user_help();
        scanf("%d", &opt);
        switch (opt)
        {
            case -1:
            {
                qser_Gnss_Stop(ph_gnss);
                qser_Gnss_Deinit(ph_gnss);
                printf("main exit\n");
                return 0;
            }

            case 1:
            {
                ret = qser_Gnss_Init(&ph_gnss);
                if(ret < 0)
                {
                    printf("qser_Gnss_Init FAIL.\n");
                }
                else
                {
                    printf("qser_Gnss_Init success , with address=%08x\n",  ph_gnss);
                }
                break;
            }
            case 2:
            {
                ret =qser_Gnss_Deinit(ph_gnss);
                if(ret < 0)
                {
                    printf("qser_Gnss_Deinit FAIL.\n");
                }
                else
                {
                    printf("qser_Gnss_Deinit success \n");
                }
                break;
            }
            case 3:
            {
                ret = qser_AddRxIndMsgHandler((gnss_handler_func_t)&cb,ph_gnss);
                if(ret < 0)
                {
                    printf("qser_AddRxIndMsgHandler fail\n");
                }
                else
                {
                    printf("add success\n");
                }
                break;
            }
            case 4:
            {
                e_msg_id_t ptr2=E_MT_LOC_MSG_ID_LOCATION_INFO;
                ret = qser_Set_Indications(ph_gnss,ptr2);
                if(ret < 0)
                {
                    printf("qser_Set_Indications fail\n");
                }
                else
                {
                    printf("set location mode success\n");
                }
                break;
            }
            case 5:
            {
                e_msg_id_t ptr4=E_MT_LOC_MSG_ID_NMEA_INFO;
                ret = qser_Set_Indications(ph_gnss,ptr4);
                if(ret < 0)
                {
                    printf("qser_Set_Indications fail\n");
                }
                else
                {
                    printf("set nmea mode success\n");
                }
                break;
            }
            case 6:
            {
                ret = qser_Gnss_Start(ph_gnss);
                if(ret < 0)
                {
                    printf("qser_Gnss_Start fail\n");
                }
                else
                {
                    printf("start success\n");
                }
                break;
            }
            case 7:
            {
                ret = qser_Gnss_Stop(ph_gnss);
                if(ret < 0)
                {
                    printf("qser_Gnss_Stop fail\n");
                }
                else
                {
                    printf("stop success\n");
                }
                break; 
            }
            case 8:
            {
                int opt_1;
                DELETE_AIDING_DATA_TYPE_T ptr;
                printf("=========delete aiding data type=========\n");
                delete_type();
                scanf("%d", &opt_1);
                switch(opt_1)
                {
                case 0:
                {
                    ptr = DELETE_NOTHING;//hot
                    break;
                }
                case 1:
                {
                    ptr = DELETE_EPHEMERIS;//warm
                    break;
                }
                case 2:
                {
                    ptr = DELETE_ALMANAC;
                    break;
                }
                case 3:
                {
                    ptr = DELETE_POSITION_TIME;
                    break;
                }
                case 4:
                {
                    ptr = DELETE_UTC;
                    break;
                }
                case 5:
                {
                    ptr = DELETE_ALL;//cold
                    break;
                }
                }
                ret = qser_Gnss_Delete_Aiding_Data(ph_gnss,ptr);
                if(ret < 0)
                {
                    printf("qser_Gnss_Delete_Aiding_Data %d fail\n",opt_1);
                }
                else
                {
                    printf("qser_Gnss_Delete_Aiding_Data %d success\n",opt_1);
                }
                break;
            }
            case 9:
            {
                LYNQ_INJECT_TIME_INTO_T time_test;
                ret = qser_Gnss_InjectTime(ph_gnss,&time_test);
                if(ret < 0)
                {
                    printf("qser_Gnss_InjectTime fail\n");
                }
                else
                {
                    printf("qser_Gnss_InjectTime success\n");
                }
                break;
            }
            case 10:
            {
                ret = qser_Gnss_download_tle();
                if(ret < 0)
                {
                    printf("qser_Gnss_download_tle fail\n");
                }
                else
                {
                    printf("qser_Gnss_download_tle success\n");
                }
                break;
            }
            case 11:
            {
                int frequency;
                printf("=========Set Frequency=========\n");
                scanf("%d", &frequency);
                ret = qser_Gnss_Set_Frequency(ph_gnss,frequency);
                if(ret < 0)
                {
                    printf("qser_Gnss_Set_Frequency fail\n");
                }
                else
                {
                    printf("frequency is %d\n",frequency);
                    printf("qser_Gnss_Set_Frequency success\n");
                }
                break;
            }
            case 12:
            {
                ret = qser_Gnss_injectEphemeris(ph_gnss);
                if(ret < 0)
                {
                    printf("qser_Gnss_injectEphemeris fail\n");
                }
                else
                {
                    printf("qser_Gnss_injectEphemeri ssuccess\n");
                }
                break;
            }
            case 13:
            {
                ret = qser_Gnss_Server_Configuration("http://uagnss.allystar.com:80/ephemeris/HD_GPS_BDS.hdb","yikecs1","Z38w5urAuawubTxi");
                if(ret < 0)
                {
                    printf("qser_Gnss_Server_Configuration fail\n");
                }
                else
                {
                    printf("qser_Gnss_Server_Configuration ssuccess\n");
                }
                break;
            }
            case 14:
            {
                ret = qser_Gnss_Set_Async_Callback(async_cb);
                if(ret < 0)
                {
                    printf("qser_Gnss_Set_Async_Callback fail\n");
                }
                else
                {
                    printf("qser_Gnss_Set_Async_Callback ssuccess\n");
                }
                break;
            }
            case 15:
            {
                ret = qser_Gnss_Start_Async(ph_gnss);
                if(ret < 0)
                {
                    printf("qser_Gnss_Start_Async fail\n");
                }
                else
                {
                    printf("qser_Gnss_Start_Async ssuccess\n");
                }
                break;
            }
        }
    }
    return 0;
}
