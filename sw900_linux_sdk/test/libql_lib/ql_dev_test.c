#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include "ql/ql_dev.h"
#include "mbtk_log.h"

static void help()
{
    printf("version: Get version.\n");
    printf("imei: Get IMEI.\n");
    printf("sn: Get SN.\n");
    printf("model: Get Model.\n");
    printf("cfun: Get radio state.\n");
    printf("cfun <state> <rest>: Set radio state.\n");
}

static int proc_exit()
{
    QL_DEV_ERROR_CODE err = ql_dev_release();
    if(QL_DEV_SUCCESS != err)
    {
        printf("ql_dev_release fail.");
        return -1;
    }
    return 0;
}

static void sig_process(int sig)
{
    LOGI("I got signal %d\n", sig);
    switch(sig)
    {
        case SIGINT: // Ctrl + C
        {
            LOGI("Exit by SIGINT.\n");
            proc_exit();
            exit(0);
        }
        case SIGQUIT: // Ctrl + \ (类似 SIGINT ，但要产生core文件)
        {
            LOGI("Exit by SIGQUIT.\n");
            proc_exit();
            exit(0);
        }
        case SIGTERM:// 默认kill   (同 SIGKILL ，但 SIGKILL 不可捕获)
        {
            LOGI("Exit by SIGTERM.\n");
            proc_exit();
            exit(0);
        }
        case SIGTSTP:// Ctrl + Z (同 SIGSTOP ，但 SIGSTOP 不可捕获)
        {
            LOGI("Exit by SIGTSTP.\n");
            exit(0);
        }
        case SIGSEGV: // 如空指针
        {
            LOGI("Exit by SIGSEGV.\n");
            exit(0);
        }
        default:
        {
            LOGI("Unknown sig:%d\n",sig);
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_process);
    signal(SIGQUIT, sig_process);
    signal(SIGTERM, sig_process);
    //signal(SIGTSTP, sig_process);
    //signal(SIGSEGV, sig_process);

    mbtk_log_init(NULL,"MBTK_QL_TEST");

    //test2(0, "192.168.1.198");
    //test2(1, "2409:8162:140:cd3c:1:2:1494:72ba");
    //test2(1, "254.128.0.0.0.0.0.0.0.1.0.2.144.5.212.239");
    //test2(1, "2400:3200::1");

    QL_DEV_ERROR_CODE err = ql_dev_init();
    if(QL_DEV_SUCCESS != err)
    {
        printf("ql_dev_init fail.");
        return -1;
    }

    printf(">>>>>>>>>>>>>>>>>>>>>>>>Enter cmd:\n");
    char cmd[100];
    while(1)
    {
        memset(cmd, 0, 100);
        if(fgets(cmd, 100, stdin))
        {
            char *ptr = cmd + strlen(cmd) - 1;
            while(ptr >= cmd && (*ptr == '\r' || *ptr == '\n'))
            {
                *ptr-- = '\0';
            }

            if(!strncasecmp(cmd, "version", 7))
            {
                char version[50] = {0};
                err = ql_dev_get_firmware_version(version);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Version : %s\n", version);
                }
            } else if(!strncasecmp(cmd, "imei", 4)){
                char imei[50] = {0};
                err = ql_dev_get_imei(imei);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("IMEI : %s\n", imei);
                }
            } else if(!strncasecmp(cmd, "sn", 2)){
                char sn[50] = {0};
                err = ql_dev_get_sn(sn);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("SN : %s\n", sn);
                }
            } else if(!strncasecmp(cmd, "model", 5)){
                char model[50] = {0};
                err = ql_dev_get_model(model);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Model : %s\n", model);
                }
            } else if(!strncasecmp(cmd, "cfun", 4)){
                int cfun;
                if(!strcasecmp(cmd, "cfun")) { // Get
                    err = ql_dev_get_modem_fun(&cfun);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Cfun : %d\n", cfun);
                    }
                } else { // Set
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    cfun = atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    int rst = atoi(ptr);
                    err = ql_dev_set_modem_fun((QL_DEV_MODEM_FUNCTION)cfun, rst);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("cfun set success\n");
                    }
                }
            }
            else if(!strcasecmp(cmd, "h") || !strcasecmp(cmd, "help")) {
                help();
            } else if(!strcasecmp(cmd, "q")) {
                break;
            } else {
                printf("\n");
            }
        }
    }

    proc_exit();

    LOGI("Client exec complete.");
#if 1
    while(1)
    {
        sleep(1000 * 365 * 24 * 60 * 60);
    }
#else
    sleep(1);
#endif
    return 0;
}

