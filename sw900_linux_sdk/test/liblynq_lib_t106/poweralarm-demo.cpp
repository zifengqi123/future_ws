#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>


#define INIT_SUCCESS    0
#define RTC_DEINIT    -1
typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;


//for server test
st_api_test_case at_api_testcases[] = 
{
    {0,   "print_help"},
    {1,   "lynq_rtc_service_init"},
    {2,   "poweralarm"},
    {3,   "wakealarm"},
    {4,   "cancel_wakealarm"},
    {5,   "lynq_set_poweralarm"},
    {6,   "lynq_set_wakealarm"},
    {7,   "lynq_rtc_service_deinit"},
    {-1,    NULL}
};

 int init_flag = -1;
 int src_id = -1;
 void *dlHandle_poweralarm = NULL;
 
typedef int (*lynq_wakealarm_add_cb)(unsigned int src_id, int rtc_id);

int (*lynq_rtc_service_init)(void);
int (*lynq_rtc_service_deinit)(void);
int (*poweralarm)(char *buffer,int src_id);
int (*wakealarm)(char *buffer,int srcid,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify);
int (*cancel_wakealarm)(int src_id,int rtc_id);
int (*lynq_set_poweralarm)(unsigned long time_sec,int src_id);
int (*lynq_set_wakealarm)(unsigned long time_sec,int srcid,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify);



void print_help(void)
{
    int i;
    printf("Supported test cases:\n");
    for(i = 0; ; i++)
    {
        if(at_api_testcases[i].cmdIdx == -1)
        {
            break;
        }
        printf("%d:\t%s\n", at_api_testcases[i].cmdIdx, at_api_testcases[i].funcName);
    }
}

int lynq_wakealarm_add_callback(unsigned int src_id, int rtc_id)
{
    printf("wake alarm callback test \n");
    return 0;
}


int main(int argc,char *argv[])
{

    int cmdIdx = 0;
    printf("Enter main function\n");
    
    const char *dlHandle_Path_poweralarm = "/lib/libpoweralarm.so";
    dlHandle_poweralarm = dlopen(dlHandle_Path_poweralarm, RTLD_NOW);
    if (dlHandle_poweralarm == NULL) 
    {
        printf("dlopen dlHandle_fota failed: %s\n", dlerror());
        return -1;
    }

    poweralarm = (int (*)(char *buffer,int src_ids))dlsym(dlHandle_poweralarm, "poweralarm");
    if(poweralarm == NULL)
    {
        printf("poweralarm is null\n");
        return -1;
    }

    wakealarm = (int (*)(char *buffer,int srcid,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify))dlsym(dlHandle_poweralarm, "wakealarm");
    if(wakealarm == NULL)
    {
        printf("wakealarm is null\n");
        return -1;
    }
    lynq_set_poweralarm = (int (*)(unsigned long,int src_id))dlsym(dlHandle_poweralarm,"lynq_set_poweralarm");
    if(lynq_set_poweralarm == NULL)
    {
        printf("lynq_set_poweralarm is null\n");
        return -1;
    }
    lynq_set_wakealarm = (int (*)(unsigned long,int srcid,int rtc_id,lynq_wakealarm_add_cb wakealarm_notify))dlsym(dlHandle_poweralarm,"lynq_set_wakealarm");
    if(lynq_set_wakealarm == NULL)
    {
        printf("lynq_set_wakealarm is null\n");
        return -1;
    }
    
    cancel_wakealarm = (int (*)(int src_id,int rtc_id))dlsym(dlHandle_poweralarm, "cancel_wakealarm");
     if(cancel_wakealarm == NULL)
    {
        printf("cancel_wakealarm is null\n");
        return -1;
    }

    lynq_rtc_service_init = (int(*)())dlsym(dlHandle_poweralarm,"lynq_rtc_service_init");
    if(lynq_rtc_service_init == NULL)
    {
        printf("lynq_rtc_service_init is null\n");
        return -1;
    }

    lynq_rtc_service_deinit = (int (*)())dlsym(dlHandle_poweralarm,"lynq_rtc_service_deinit");
    if(lynq_rtc_service_deinit == NULL)
    {
        printf("lynq_rtc_service_deinit is null \n");
        return -1;
    }
    
    print_help();
    while(1)
    {
        
        printf("\nplease input cmd index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            break;
        }
        switch(cmdIdx)
        {   
            case 0:
                print_help();
                break;
            case 1:
                {
                    src_id = lynq_rtc_service_init();
                    if(src_id <= 0)
                    {

                        printf("RTC service init failed ");
                        return -1;
                    }
                    else
                    {
                        init_flag = INIT_SUCCESS;
                    }
                    
                    break;
                }
            case 2:
                {
                    
                    if(init_flag != INIT_SUCCESS)
                    {
                        printf("*****ERROR must init rtc service ,Please reselect *******\n ");
                    }
                    else
                    {
                        int ret = 0;
                        char tmp_time[32] = {0};
                        printf("Set poweralarm time,e.g: 60 (seconds)\n");
                        scanf("%s",tmp_time);
                        ret = poweralarm(tmp_time,src_id);
                        if(ret != 0)
                        {
                            printf("set poweralarm failed\n");
                            return -1;
                        }
                    }
                break;
                }
            case 3:
                {
                    if(init_flag != INIT_SUCCESS)
                    {
                        printf("*****ERROR must init rtc service ,Please reselect *******\n ");
                    }
                    else
                    {
                        int ret = 0;
                        char tmp_time[32]={0};
                        int rtc_id = 0;
                        printf("Set wakealarm time ,e.g: 60  1 (seconds)\n");
                        scanf("%s%d",tmp_time,&rtc_id);
                   
                        ret = wakealarm(tmp_time,src_id,rtc_id,lynq_wakealarm_add_callback);
                        if(ret != 0)
                        {
                            printf("set wakealarm failed\n");
                            return -1;
                        }
                    }
                    break;
                }
             case 4:
                {
                    if(init_flag != INIT_SUCCESS)
                    {
                        printf("*****ERROR must init rtc service ,Please reselect *******\n ");
                    }
                    else
                    {
                        int ret = 0;
                        int rtc_id = 0;
                        printf("Please input you want cancel rtc timer rtc id\n");
                        scanf("%d",&rtc_id);
                        ret = cancel_wakealarm(src_id,rtc_id);
                        if(ret != 0)
                        {
                            printf("Cancel_wakealarm failed!!!");
                            return -1;
                        }
                    }
                    break;
                }
             case 5:
                { 
                    if(init_flag != INIT_SUCCESS)
                    {
                        printf("*****ERROR must init rtc service ,Please reselect *******\n ");
                    }
                    else
                    {
                        int ret = 0; 
                        unsigned long time_sec = 0;
                        printf("Input time_sec you want poweralarm,e.g: 60 (seconds)\n");
                        scanf("%lu",&time_sec);
                        ret =lynq_set_poweralarm(time_sec,src_id);
                        printf("ret is %d\n",ret);
                        if(ret != 0)
                        {
                            printf("lynq_set_poweralarm failed\n");
                            return -1;
                        }
                    }
                   break;
                }
            case 6:
                { 
                    if(init_flag != INIT_SUCCESS)
                    {
                        printf("*****ERROR must init rtc service ,Please reselect *******\n ");
                   }
                   else
                   {
                        int ret = 0; 
                        unsigned long time_sec = 0;
                        int rtc_id;
                        printf("Input time_sec you want wakealarm,e.g:60  1(seconds)\n");
                        scanf("%lu%d",&time_sec,&rtc_id);
                        ret =lynq_set_wakealarm(time_sec,src_id,rtc_id,lynq_wakealarm_add_callback);
                        if(ret != 0)
                        {
                            printf("lynq_set_wakealarm failed\n");
                            return -1;
                        }
                    }
                   break;
                }
            case 7:
                {
                    if(init_flag != INIT_SUCCESS)
                    {
                        printf("*****ERROR must init rtc service ,Please reselect *******\n ");
                    }
                    else
                    {
                        int ret  = -1;
                        printf("rtc servce deinit !!!!!!!!!!!!");
                        ret = lynq_rtc_service_deinit();
                        if(ret != 0)
                        {
                            printf("lynq rtc service deinit failed \n");
                            return -1;
                        }
                        else
                        {
                            init_flag = RTC_DEINIT;
                        }
                    }
                    break;
                }
                
            default:
                break;
        }
    }

    return 0;

}

