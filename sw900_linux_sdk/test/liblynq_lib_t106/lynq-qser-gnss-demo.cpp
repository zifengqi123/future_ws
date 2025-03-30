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
#include "lynq-qser-gnss-demo.h"

#ifdef __cplusplus
extern "C" {
#endif


int (*qser_Gnss_Init)(uint32_t *h_gnss);
int (*qser_Gnss_Deinit)(uint32_t);
int (*qser_AddRxIndMsgHandler)(gnss_handler_func_t handler_ptr,uint32_t);
int (*qser_Set_Indications)(uint32_t h_gnss,e_msg_id_t);
int (*qser_Gnss_Start)(uint32_t h_gnss);
int (*qser_Gnss_Stop)(uint32_t h_gnss);
int (*qser_Gnss_Delete_Aiding_Data)(uint32_t,DELETE_AIDING_DATA_TYPE_T);
int (*qser_Gnss_InjectTime)(uint32_t,LYNQ_INJECT_TIME_INTO_T *time_info);
int (*qser_Gnss_Server_Configuration)(char *host, char *id, char *password);
int (*qser_Gnss_download_tle)();
int (*qser_Gnss_injectEphemeris)(uint32_t);
int (*qser_Gnss_Set_Frequency)(uint32_t, int);
void *dlHandle_gnss;

int main(int argc, char *argv[])
{
    int ret;
    int opt = 0;
    char *lynqLib_gnss = "/lib/liblynq-qser-gnss.so";
    char dev_file[12] = {0};
    uint32_t ph_gnss;
    dlHandle_gnss = dlopen(lynqLib_gnss, RTLD_NOW);
    while(1)
    {
        printf("=========gnss main=========\n");
        user_help();
        scanf("%d", &opt);
        while(getchar()!='\n');
        switch (opt)
        {
        case -1:
        {
            printf("main exit\n");
            return 0;
        }

        case 1:
        {
            qser_Gnss_Init=(int(*)(uint32_t *h_gnss))dlsym(dlHandle_gnss, "qser_Gnss_Init");
            ret = qser_Gnss_Init(&ph_gnss);
            if(ret < 0)
            {
                printf("mopen_gnss_client_init FAIL.\n");
                return -1;
            }
            printf("mopen_gnss_client_init success , with address=%d\n",  ph_gnss);
            break;
        }
        case 2:
        {
            qser_Gnss_Deinit=(int(*)(uint32_t))dlsym(dlHandle_gnss, "qser_Gnss_Deinit");
            ret =qser_Gnss_Deinit(ph_gnss);
            if(ret < 0)
            {
                printf("mopen_gnss_client_Deinit FAIL.\n");
                return -1;
            }
            printf("mopen_gnss_client_Deinit success \n");
            break;
        }
        case 3:
        {
            qser_AddRxIndMsgHandler=(int(*)(gnss_handler_func_t,uint32_t))dlsym(dlHandle_gnss, "qser_AddRxIndMsgHandler");
            ret = qser_AddRxIndMsgHandler((gnss_handler_func_t)&cb,ph_gnss);
            if(ret < 0)
            {
                printf("lynq_AddRxIndMsgHandler fail\n");
                qser_Gnss_Deinit(ph_gnss);
                return -1;
            }
            printf("add success\n");
            break;
        }
        case 4:
        {
            e_msg_id_t ptr2=E_MT_LOC_MSG_ID_LOCATION_INFO;
            qser_Set_Indications=(int(*)(uint32_t h_gnss,e_msg_id_t))dlsym(dlHandle_gnss, "qser_Set_Indications");
            ret = qser_Set_Indications(ph_gnss,ptr2);
            if(ret < 0)
            {
                printf("lynq_Set_LOCATION_INFO fail\n");
                qser_Gnss_Deinit(ph_gnss);
                return -1;
            }
            printf("set location mode success\n");
            break;
        }
        case 5:
        {
            e_msg_id_t ptr4=E_MT_LOC_MSG_ID_NMEA_INFO;
            qser_Set_Indications=(int(*)(uint32_t h_gnss,e_msg_id_t))dlsym(dlHandle_gnss, "qser_Set_Indications");
            ret = qser_Set_Indications(ph_gnss,ptr4);
            if(ret < 0)
            {
                printf("lynq_Set_NMEA_INFO fail\n");
                qser_Gnss_Deinit(ph_gnss);
                return -1;
            }
            printf("set nmea mode success\n");
            break;
            
        }
        case 6:
        {
            qser_Gnss_Start=(int(*)(uint32_t))dlsym(dlHandle_gnss, "qser_Gnss_Start");
            ret = qser_Gnss_Start(ph_gnss);
            if(ret < 0)
            {
                printf("lynq_Gnss_Start fail\n");
                return -1;
            }
            printf("start success\n");
            break;
        }
        case 7:
        {
            qser_Gnss_Stop=(int(*)(uint32_t))dlsym(dlHandle_gnss, "qser_Gnss_Stop");
            ret = qser_Gnss_Stop(ph_gnss);
            if(ret < 0)
            {
                printf("lynq_Gnss_Stop fail\n");
                qser_Gnss_Deinit(ph_gnss);
                return -1;
            }
            printf("stop success\n");
            break; 
        }

        case 8:
        {
            int opt_1;
            DELETE_AIDING_DATA_TYPE_T ptr;
            qser_Gnss_Delete_Aiding_Data=(int(*)(uint32_t,DELETE_AIDING_DATA_TYPE_T))dlsym(dlHandle_gnss, "qser_Gnss_Delete_Aiding_Data");
            printf("=========delete aiding data type=========\n");
            delete_type();
            scanf("%d", &opt_1);
            while(getchar()!='\n');
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
            default:
            {
                printf("input error\n");
                return -1;
            }
            }
            ret = qser_Gnss_Delete_Aiding_Data(ph_gnss,ptr);
            if(ret < 0)
            {
                printf("lynq_Gnss_Delete_Aiding_Data %d fail\n",opt_1);
                qser_Gnss_Deinit(ph_gnss);
                return -1;
            }
            printf("lynq_Gnss_Delete_Aiding_Data %d success\n",opt_1);
            break;
        }
        case 9:
        {
            LYNQ_INJECT_TIME_INTO_T time_test;
            qser_Gnss_InjectTime=(int(*)(uint32_t,LYNQ_INJECT_TIME_INTO_T *time_info))dlsym(dlHandle_gnss, "qser_Gnss_InjectTime");
            ret = qser_Gnss_InjectTime(ph_gnss,&time_test);
            if(ret < 0)
            {
                printf("qser_Gnss_InjectTime fail\n");
                qser_Gnss_Deinit(ph_gnss);
                return -1;
            }
            printf("qser_Gnss_InjectTime success\n");
            break;
        }
        case 10:
        {
            qser_Gnss_download_tle=(int(*)())dlsym(dlHandle_gnss, "qser_Gnss_download_tle");
            ret = qser_Gnss_download_tle();
            if(ret < 0)
            {
                printf("qser_Gnss_download_tle fail\n");
                return -1;
            }
            printf("qser_Gnss_download_tle success\n");
            break;
        }
        case 11:
        {
            int frequency;
            qser_Gnss_Set_Frequency=(int(*)(uint32_t, int))dlsym(dlHandle_gnss, "qser_Gnss_Set_Frequency");
            printf("=========delete aiding data type=========\n");
            scanf("%d", &frequency);
            ret = qser_Gnss_Set_Frequency(ph_gnss,frequency);
            if(ret < 0)
            {
                printf("qser_Gnss_Set_Frequency fail\n");
                return -1;
            }
            printf("frequency is %d\n",frequency);
            printf("qser_Gnss_Set_Frequency success\n");
            break;
        }
        case 12:
        {
            int (*qser_Gnss_injectEphemeris)(uint32_t);
            qser_Gnss_injectEphemeris=(int(*)(uint32_t))dlsym(dlHandle_gnss, "qser_Gnss_injectEphemeris");
            ret = qser_Gnss_injectEphemeris(ph_gnss);
            if(ret < 0)
            {
                printf("qser_Gnss_injectEphemeris fail\n");
                return -1;
            }
            printf("qser_Gnss_injectEphemeri ssuccess\n");
            break;
        }
        case 13:
        {
            qser_Gnss_Server_Configuration=(int(*)(char *host, char *id, char *password))dlsym(dlHandle_gnss, "qser_Gnss_Server_Configuration");
            ret = qser_Gnss_Server_Configuration(NULL,"lcz","123456");
            if(ret < 0)
            {
                printf("qser_Gnss_Server_Configuration fail\n");
                return -1;
            }
            printf("qser_Gnss_Server_Configuration ssuccess\n");
            break;
        }
        default:
        {
            printf("input error,please re-enter\n");
            break;
        }
        }
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
