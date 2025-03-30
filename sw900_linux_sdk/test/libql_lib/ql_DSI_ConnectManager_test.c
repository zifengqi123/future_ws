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

#include "ql/DSI_ConnectManager.h"
#include "mbtk_log.h"

static void help()
{
    printf("apn_get <cid> : Get current apns.\n");
    printf("apn <cid> <0/1/2> <apn> [<user> <pass> <auth>] : Set apn (IPV4V6/IPV4/IPV6).\n");
    printf("data_call <0/1/2> <cid> <timeout>: Stop/Start/State data call.\n");
    printf("data_call_ex <0/1/2> <cid> <timeout>: Stop/Start/State data call.\n");
}

static int proc_exit()
{
    int err = ql_wan_release();
    if(err)
    {
        printf("ql_wan_release fail.");
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

static void data_call_status_cb(int status)
{
    printf("DATA_CALL_STATE:%d\n", status);
}

static void data_call_ex_status_cb(int cid,int iptype,int status,int cause)
{
    printf("DATA_CALL_EX_STATE:%d, %d, %d, %d\n", cid, iptype, status, cause);
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

    int err = ql_wan_init();
    if(err)
    {
        printf("ql_wan_init fail.");
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

            if(!strncasecmp(cmd, "apn", 3)){
                if(!strncasecmp(cmd, "apn_get", 7)) { // Get apn
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    int cid = atoi(ptr);
                    int ip_type = -1;
                    char apn[128] = {0};
                    char userName[128] = {0};
                    char password[128] = {0};
                    int auth = -1;
                    err = ql_wan_getapn(cid, &ip_type, apn, 128, userName, 128, password, 128, &auth);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN : %d, %d, %s, %s, %s, %d\n", cid, ip_type, apn, userName, password, auth);
                    }
                } else { // apn <cid> <0/1/2> <apn> [<user> <pass> <auth>]
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    int cid = atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    int ip_type = atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    char apn[128] = {0};
                    memcpy(apn, ptr, strlen(ptr));
                    char *tmp = apn;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL) {
                        err = ql_wan_setapn(cid, ip_type, apn, NULL, NULL, QL_DSI_AUTH_PREF_NULL);
                    } else {
                        while(*ptr != '\0' && *ptr == ' ')
                            ptr++;
                        char user[128] = {0};
                        memcpy(user, ptr, strlen(ptr));
                        tmp = user;
                        while(*tmp) {
                            if(*tmp == ' ') {
                                *tmp = '\0';
                                break;
                            }
                            tmp++;
                        }

                        ptr = strstr(ptr, " ");
                        if(ptr == NULL)
                            continue;
                        while(*ptr != '\0' && *ptr == ' ')
                            ptr++;
                        char pass[128] = {0};
                        memcpy(pass, ptr, strlen(ptr));
                        tmp = pass;
                        while(*tmp) {
                            if(*tmp == ' ') {
                                *tmp = '\0';
                                break;
                            }
                            tmp++;
                        }

                        ptr = strstr(ptr, " ");
                        if(ptr == NULL)
                            continue;
                        while(*ptr != '\0' && *ptr == ' ')
                            ptr++;
                        QL_DSI_AUTH_PREF_T auth  = (QL_DSI_AUTH_PREF_T)atoi(ptr);

                        err = ql_wan_setapn(cid, ip_type, apn, user, pass, auth);
                    }
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "data_call_ex", 12)){ // data_call_ex <0/1/2> <cid> <timeout>
                // data_call <0/1/2> <cid> <timeout>
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                int type = atoi(ptr);

                ptr = strstr(ptr, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                int cid = atoi(ptr);

                ptr = strstr(ptr, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                int timeout = atoi(ptr);

                switch (type)
                {
                    case 0:
                        err = ql_wan_stop(cid);
                        break;
                    case 1:
                        err = ql_wan_start_ex(cid, 1, data_call_ex_status_cb);
                        break;
                    case 2: {
                        ql_data_call_info info;
                        err = ql_get_data_call_info(cid, &info);
                        if(!err) {
                            printf("cid : %d, ip_type : %d\n", info.profile_idx, info.ip_type);
                            if(info.v4.state) {
                                printf("%s: %s, %s, %s\n", info.v4.addr.name, info.v4.addr.ip, info.v4.addr.pri_dns, info.v4.addr.sec_dns);
                            } else {
                                printf("IPV4 not available.\n");
                            }

                            if(info.v6.state) {
                                printf("%s: %s, %s, %s\n", info.v6.addr.name, info.v6.addr.ip, info.v6.addr.pri_dns, info.v6.addr.sec_dns);
                            } else {
                                printf("IPV6 not available.\n");
                            }
                        }
                        break;
                    }
                    default:
                        printf("Type error:%d\n", type);
                        break;
                }

                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("DATA_CALL success\n");
                }
            } else if(!strncasecmp(cmd, "data_call", 9)){ // data_call <0/1/2> <cid> <timeout>
                // data_call <0/1/2> <cid> <timeout>
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                int type = atoi(ptr);

                ptr = strstr(ptr, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                int cid = atoi(ptr);

                ptr = strstr(ptr, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                int timeout = atoi(ptr);

                switch (type)
                {
                    case 0:
                        err = ql_wan_stop(cid);
                        break;
                    case 1:
                        err = ql_wan_start(cid, 1, data_call_status_cb);
                        break;
                    case 2: {
                        ql_data_call_info info;
                        err = ql_get_data_call_info(cid, &info);
                        if(!err) {
                            printf("cid : %d, ip_type : %d\n", info.profile_idx, info.ip_type);
                            if(info.v4.state) {
                                printf("%s: %s, %s, %s\n", info.v4.addr.name, info.v4.addr.ip, info.v4.addr.pri_dns, info.v4.addr.sec_dns);
                            } else {
                                printf("IPV4 not available.\n");
                            }

                            if(info.v6.state) {
                                printf("%s: %s, %s, %s\n", info.v6.addr.name, info.v6.addr.ip, info.v6.addr.pri_dns, info.v6.addr.sec_dns);
                            } else {
                                printf("IPV6 not available.\n");
                            }
                        }
                        break;
                    }
                    default:
                        printf("Type error:%d\n", type);
                        break;
                }

                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("DATA_CALL success\n");
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

