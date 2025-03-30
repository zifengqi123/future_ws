#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>
#include <pthread.h>

int (*lynq_get_upgrade_status)(void);
int (*lynq_fota_set_addr_value)(char *value,int size);
int (*lynq_fota_nrestart)(void);
int (*lynq_rock_main)(int first_run);
int (*lynq_read_process)(void);
void *dlHandle_fota = NULL;

void *thread_function_noreboot(void *arg) 
{

        lynq_fota_nrestart();
        return NULL;
}

void *thread_function_reboot(void *arg) 
{

        lynq_rock_main(1);
        return NULL;
}


int main(int argc,char *argv[])
{
    int ret = 0;
    int reboot_flag; 
    char *value = argv[1];
    printf("Enter main function\n");
    
    const char *lynqLibPath_fota = "/lib/liblynq-fota.so";
    dlHandle_fota = dlopen(lynqLibPath_fota, RTLD_NOW);
    if (dlHandle_fota == NULL) 
    {
        printf("dlopen dlHandle_fota failed: %s\n", dlerror());
        return -1;
    }

    lynq_fota_set_addr_value = (int (*)(char *value,int size))dlsym(dlHandle_fota, "lynq_fota_set_addr_value");
    if(lynq_fota_set_addr_value == NULL)
    {
        printf("lynq fota ser addr value is null\n");
        return -1;
    }

    lynq_get_upgrade_status = (int (*)(void))dlsym(dlHandle_fota,"lynq_get_upgrade_status");
    if(lynq_get_upgrade_status == NULL)
    {
        printf("lynq_get_upgrade_status is null\n");
        return -1;
    }
    lynq_fota_nrestart = (int (*)())dlsym(dlHandle_fota,"lynq_fota_nrestart");
    if(lynq_fota_nrestart == NULL)
    {
        printf("lynq_fota_nrestart is null\n");
        return -1;
    }
    lynq_rock_main = (int (*)(int first_run))dlsym(dlHandle_fota,"lynq_rock_main");
    if(lynq_rock_main == NULL)
    {
        printf("lynq_rock_main is null\n");
        return -1;
    }

    lynq_read_process = (int (*)(void))dlsym(dlHandle_fota,"lynq_read_process");
    if(lynq_read_process == NULL)
    {
        printf("lynq_read_process is null\n");
        return -1;
    }
    
    ret = lynq_fota_set_addr_value(value,(int )strlen(value));
    if(ret != 0)
    {
        printf("set upgrade package addr failed\n");
        return -1;
    }
    while(1)
    {
        printf("Please chose action  0:  upgrade done ,not reboot 1: upgrade done ,reboot 2:get upgrade status 3:read fota process \n");
        scanf("%d",&reboot_flag);

        switch(reboot_flag)
        {
            case 0:
                {
                    pthread_t thread_id_noreboot;
                    int result = pthread_create(&thread_id_noreboot, NULL, thread_function_noreboot, NULL);
                    if (result != 0) 
                    {
                        printf("pthread_create failed \n");
                        return -1;;
                    }
                }
                break;

            case 1:
                {
                    pthread_t thread_id_reboot;
                    int result = pthread_create(&thread_id_reboot, NULL, thread_function_reboot, NULL);
                    if (result != 0) 
                    {
                        printf("pthread_create failed \n");
                        return -1;;
                    }
                }
                break;
            case 2:
                printf("Get fota upgrade status \n");
                ret = lynq_get_upgrade_status();
                printf("lynq_get_upgrade_status ret is %d\n",ret);
                break;
            case 3:
                printf("get fota upgrade process\n");
                ret = lynq_read_process();
                printf("Now upgrade process is %d\n",ret);
                break;
            default:
                printf("please input right flag 0 or 1 or 2 or 3\n");
                break;

        }
         
    }
    return 0;

}

