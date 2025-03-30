/**@File lynq-systime-demo.cpp
 *
 * @Brief As a example for liblynq-systime.
 *
 * @Author sj.zhang
 *
 * @Date 2023-08-15
 *
 * @Version V1.0
 *
 * @copyright Copyright (c) MobileTek
 */
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct time_source_status
{
    int ntp;
    int nitz;
    int gnss;
} time_src_status_s;

#ifdef MOBILETEK_TARGET_PLATFORM_T106
int (*lynq_sync_time_from_rtc)(void);
int (*lynq_set_rtc_time)(void);
int (*lynq_get_rtc_time)(unsigned long *ulsec);

#endif
void *dlHandle_systime = NULL;
int (*modem_time_enable)(int enable);
int (*gnss_time_enable)(int enable);
int (*ntp_sync_time)(int enable);
int (*user_set_time)(char *date, char *time);
int (*lynq_get_time_src_status)(time_src_status_s *time_src);
int (*get_sync_time_result)();


int lynq_ntp_sync_demo(int argc, char *argv[])
{
    int ret = -1;
    if (argc != 3)
    {
        printf("Parameter number wrong !\n");
        dlclose(dlHandle_systime);
        return -1;
    }
    ntp_sync_time = (int (*)(int enable))dlsym(dlHandle_systime, "ntp_sync_time");
    if (NULL == ntp_sync_time)
    {
        fprintf(stderr, "Failed to dlsym function ntp_sync_time : %s\n", dlerror());
        dlclose(dlHandle_systime);
        return -1;
    }
    ret = ntp_sync_time(atoi(argv[2]));
    switch (ret)
    {
    case 0:
        printf("set success!\n");
        break;
    case 1:
        printf("NTP_ALREADY_ENABLE!\n");
        break;
    case 2:
        printf("NTP_ALREADY_DISENABLE!\n");
        break;
    default:
        break;
    }
    return ret;
}

int lynq_modem_sync_demo(int argc, char *argv[])
{
    int ret = -1;
    if (argc != 3)
    {
        printf("Parameter number wrong !\n");
        dlclose(dlHandle_systime);
        return -1;
    }
    modem_time_enable = (int (*)(int enable))dlsym(dlHandle_systime, "modem_time_enable");
    if (NULL == modem_time_enable)
    {
        fprintf(stderr, "Failed to dlsym function modem_time_enable: %s\n", dlerror());
        dlclose(dlHandle_systime);
        return -1;
    }
    ret = modem_time_enable(atoi(argv[2]));
    printf("ret = %d\n", ret);
    if (ret != 0 && ret != 4)
    {
        printf("modem_time_enable failed !\n");
        dlclose(dlHandle_systime);
        return -1;
    }
    return 0;
}

int lynq_gnss_sync_demo(int argc, char *argv[])
{
    int ret = -1;
    if (argc != 3)
    {
        printf("Parameter number wrong !\n");
        dlclose(dlHandle_systime);
        return -1;
    }
    gnss_time_enable = (int (*)(int enable))dlsym(dlHandle_systime, "gnss_time_enable");
    if (NULL == gnss_time_enable)
    {
        fprintf(stderr, "Failed to dlsym function gnss_time_enable: %s\n", dlerror());
        dlclose(dlHandle_systime);
        return -1;
    }
    ret = gnss_time_enable(atoi(argv[2]));
    printf("ret = %d\n", ret);
    if (ret != 0 && ret != 4)
    {
        printf("gnss_time_enable failed !\n");
        dlclose(dlHandle_systime);
        return -1;
    }
    return ret;
}

int lynq_user_set_time_demo(int argc, char *argv[])
{
    int ret = -1;
    if (argc != 4)
    {
        printf("Parameter number wrong !\n");
        dlclose(dlHandle_systime);
        return -1;
    }
    user_set_time = (int (*)(char *date, char *time))dlsym(dlHandle_systime, "user_set_time");
    if (NULL == user_set_time)
    {
        fprintf(stderr, "Failed to dlsym function user_set_time: %s\n", dlerror());
        dlclose(dlHandle_systime);
        return -1;
    }
    ret = user_set_time(argv[2], argv[3]);
    return ret;
}

int lynq_get_time_src_status_demo(int arg_c, char *arg_v[])
{
    lynq_get_time_src_status = (int (*)(time_src_status_s *time_src))dlsym(dlHandle_systime, "lynq_get_time_src_status");
    if (NULL == lynq_get_time_src_status)
    {
        fprintf(stderr, "Failed to dlsym function lynq_get_time_src_status : %s\n", dlerror());
        dlclose(dlHandle_systime);
        return -1;
    }
    time_src_status_s time_src = {0};
    lynq_get_time_src_status(&time_src);
    printf("time source status are :\nntp=%d\nnitz=%d\ngnss=%d\n", time_src.ntp, time_src.nitz, time_src.gnss);
    return 0;
}

int lynq_get_sync_status_demo(int arg_c, char *arg_v[])
{
    get_sync_time_result = (int (*)(void))dlsym(dlHandle_systime, "get_sync_time_result");
    int ret = 0;
    if(NULL == get_sync_time_result)
    {
        printf("Failed to dlsym function get_sync_time_result: %s\n", dlerror());
        dlclose(dlHandle_systime);
        return -1;
    }
    ret = get_sync_time_result();
    printf("get_sync_time_result = %d\n",ret);
	dlclose(dlHandle_systime);
    return 0;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    char *cmd = argv[1];
    printf("enter lynq_systime_demo \n");
    dlHandle_systime = dlopen("/lib/liblynq-systime.so", RTLD_NOW);
    if (!dlHandle_systime)
    {
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
        return 1;
    }

    if (cmd == NULL)
    {
        printf("No command received!\n");
        dlclose(dlHandle_systime);
        return 1;
    }
    else if (strcmp(cmd, "ntp") == 0)
    {
        ret = lynq_ntp_sync_demo(argc, argv);
    }
    else if (strcmp(cmd, "nitz") == 0)
    {
        ret = lynq_modem_sync_demo(argc, argv);
    }
    else if (strcmp(cmd, "gnss") == 0)
    {
        ret = lynq_gnss_sync_demo(argc, argv);
    }
    else if (strcmp(cmd, "user") == 0)
    {
        ret = lynq_user_set_time_demo(argc, argv);
    }
#ifdef MOBILETEK_TARGET_PLATFORM_T106
    else if (strcmp(cmd, "sync_rtc") == 0)
    {
        lynq_sync_time_from_rtc = (int (*)(void))dlsym(dlHandle_systime, "lynq_sync_time_from_rtc");
        if (NULL == lynq_sync_time_from_rtc)
        {
            fprintf(stderr, "Failed to dlsym function lynq_sync_time_from_rtc : %s\n", dlerror());
            dlclose(dlHandle_systime);
            return -1;
        }
        ret = lynq_sync_time_from_rtc();
    }
    else if (strcmp(cmd, "rtc_set") == 0)
    {
        lynq_set_rtc_time = (int (*)(void))dlsym(dlHandle_systime, "lynq_set_rtc_time");
        if (NULL == lynq_set_rtc_time)
        {
            fprintf(stderr, "Failed to dlsym function lynq_set_rtc_time : %s\n", dlerror());
            dlclose(dlHandle_systime);
            return -1;
        }
        ret = lynq_set_rtc_time();
    }
    else if (strcmp(cmd, "rtc_get") == 0)
    {
        lynq_get_rtc_time = (int (*)(unsigned long *ulsec))dlsym(dlHandle_systime, "lynq_get_rtc_time");
        if (NULL == lynq_get_rtc_time)
        {
            fprintf(stderr, "Failed to dlsym function lynq_get_rtc_time : %s\n", dlerror());
            dlclose(dlHandle_systime);
            return -1;
        }
        unsigned long time_ret;
        ret = lynq_get_rtc_time(&time_ret);
        time_t tp = (time_t)time_ret;
        char *rtc_time = ctime(&tp);
        printf("rtc_time seconds = %lu\n", time_ret);
        printf("rtc time = %s\n", rtc_time);
    }
#endif
    else if (strcmp(cmd, "src_status") == 0)
    {
        ret = lynq_get_time_src_status_demo(argc, argv);
    }
    else if (strcmp(cmd,"sync_status") == 0)
    {
        ret = lynq_get_sync_status_demo(argc, argv);
    }
    else
    {
        printf("command wrong !\n");
        dlclose(dlHandle_systime);
        return -1;
    }
    if (0 != ret)
    {
        printf("ret = %d\n Command execution failure.\n", ret);
    }
    dlclose(dlHandle_systime);
    return ret;
}
