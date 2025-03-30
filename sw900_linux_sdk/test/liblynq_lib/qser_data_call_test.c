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

#include "lynq-qser-data.h"
#include "mbtk_log.h"

static void help()
{
    printf("apn_get <cid> : Get current apns.\n");
    printf("apn_del <cid> : del apns.\n");
    printf("apn_list <cid> : list apns.\n");
    printf("apn_set <cid> <0/1/2/3> <apn> <apn_type> [<user> <pass> <(0 1)/2/3>] : \n");
    printf("Set apn (1-6) (IPV4/PPP/IPV6/IPV4V6) (150) (iot_net_i) (127) (127) (NONE/PAP/CHAP).\n");
    printf("apn_add <0/1/2/3> <apn> <apn_type> [<user> <pass> <(0 1)/2/3>] : \n");
    printf("data_call <0/1/2/3> <cid> <type> <re-con> <user> <pass>: Stop/Start/State data call.\n");
    printf("data_call <0/1/2/3> <cid> <IPV4/IPV6/IPV4V6> [<0/1> <user> <pass>].\n");
}

static int proc_exit()
{
    qser_data_call_destroy();
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

static void data_call_status_cb(qser_data_call_state_s *state)
{
    printf("entry data_call_status_cb\n");
    if(state == NULL)
    {
        printf("state is NULL\n");
    }
    printf("DATA_DEMO_CALL_BACK: profile_idx=%d, name=%s, ip_family=%d, state=%d, error=%d\n"
        , state->profile_idx, state->name, state->ip_family, state->state, state->err);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_process);
    signal(SIGQUIT, sig_process);
    signal(SIGTERM, sig_process);
    //signal(SIGTSTP, sig_process);
    //signal(SIGSEGV, sig_process);

    mbtk_log_init(NULL,"MBTK_QSER_DATA_CALL_TEST");

    //test2(0, "192.168.1.198");
    //test2(1, "2409:8162:140:cd3c:1:2:1494:72ba");
    //test2(1, "254.128.0.0.0.0.0.0.0.1.0.2.144.5.212.239");
    //test2(1, "2400:3200::1");

    int err = qser_data_call_init(data_call_status_cb);
    if(err)
    {
        printf("qser_data_call_init fail.");
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
                    qser_apn_info_s qser_apn = {0};
                    err = qser_apn_get(cid, &qser_apn);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN : %d, %d, %s, %s, %s, %d, %s.\n",qser_apn.profile_idx, qser_apn.pdp_type, qser_apn.apn_name, qser_apn.username, qser_apn.password, qser_apn.auth_proto, qser_apn.apn_type);
                    }
                } else if(!strncasecmp(cmd, "apn_list", 8)){
                    qser_apn_info_list_s apn_list = {0};
                    err = qser_apn_get_list(&apn_list);
                    if(err)
                    {
                        printf("Error : %d\n", err);
                    }
                    else
                    {
                        printf("cnt: %d.\n", apn_list.cnt);
                        int i = 0;
                        for(i = 0; i < apn_list.cnt; i++)
                        {
                            printf("APN : %d, %d, %s, %s, %s, %d, %s.\n", apn_list.apn[i].profile_idx, apn_list.apn[i].pdp_type, apn_list.apn[i].apn_name, apn_list.apn[i].username, apn_list.apn[i].password, apn_list.apn[i].auth_proto, apn_list.apn[i].apn_type);
                        }
                    }
                } else if(!strncasecmp(cmd, "apn_del", 7)){
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    char profile_idx = atoi(ptr);
                    err = qser_apn_del(profile_idx);
                    if(err)
                    {
                        printf("Error : %d\n", err);
                    }
                    else
                    {
                        printf("APN set success.\n");
                    }
                } else if(!strncasecmp(cmd, "apn_add", 7)){
                    char qser_idx = 0;
                    qser_apn_add_s qser_apn = {0};
                    char *tmp = NULL;
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    qser_apn.pdp_type = (qser_apn_pdp_type_e)atoi(ptr);


                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                    {
                        err = qser_apn_add(&qser_apn, &qser_idx);
                        if(err) 
                        {
                            printf("Error : %d\n", err);
                        } 
                        else 
                        {
                            printf("APN set success. get idx = [%d]\n", qser_idx);
                        }
                        continue;
                    }
                
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_apn.apn_name, ptr, strlen(ptr));
                    tmp = qser_apn.apn_name;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }


                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                    {
                        err = qser_apn_add(&qser_apn, &qser_idx);
                        if(err) 
                        {
                            printf("Error : %d\n", err);
                        } 
                        else 
                        {
                            printf("APN set success. get idx = [%d]\n", qser_idx);
                        }
                        continue;
                    }
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_apn.apn_type, ptr, strlen(ptr));
                    tmp = qser_apn.apn_type;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }

                    
                    ptr = strstr(ptr, " ");
                    if(ptr == NULL) 
                    {
                        err = qser_apn_add(&qser_apn, &qser_idx);
                         if(err) 
                        {
                            printf("Error : %d\n", err);
                        } 
                        else 
                        {
                            printf("APN set success. get idx = [%d]\n", qser_idx);
                        }
                        continue;
                    } 
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_apn.username, ptr, strlen(ptr));
                    tmp = qser_apn.username;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                    {
                        continue;
                    } 
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_apn.password, ptr, strlen(ptr));
                    tmp = qser_apn.password;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                    {
                        continue;
                    } 
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    qser_apn.auth_proto = (qser_apn_auth_proto_e)atoi(ptr);

                    err = qser_apn_add(&qser_apn, &qser_idx);
              
                    if(err) 
                    {
                        printf("Error : %d\n", err);
                    } 
                    else 
                    {
                        printf("APN set success. get idx = [%d]\n", qser_idx);
                    }
                }
                else { // apn <cid> <0/1/2> <apn> [<user> <pass> <auth>]
                    qser_apn_info_s qser_apn = {0};
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    qser_apn.profile_idx = atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    qser_apn.pdp_type = (qser_apn_pdp_type_e)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_apn.apn_name, ptr, strlen(ptr));
                    char *tmp = qser_apn.apn_name;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }


                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                    {
                        err = qser_apn_set(&qser_apn);
                        if(err) 
                        {
                            printf("Error : %d\n", err);
                        }   
                        else 
                        {
                            printf("APN set success\n");
                        }
                        continue;
                    }
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_apn.apn_type, ptr, strlen(ptr));
                    tmp = qser_apn.apn_type;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }


                    ptr = strstr(ptr, " ");
                    if(ptr == NULL) {
                       err = qser_apn_set(&qser_apn);
                       if(err) 
                       {
                           printf("Error : %d\n", err);
                       }   
                       else 
                       {
                           printf("APN set success\n");
                       }
                       continue;
                    } 
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_apn.username, ptr, strlen(ptr));
                    tmp = qser_apn.username;
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
                    memcpy(qser_apn.password, ptr, strlen(ptr));
                    tmp = qser_apn.password;
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
                    qser_apn.auth_proto = (qser_apn_auth_proto_e)atoi(ptr);

                    err = qser_apn_set(&qser_apn);
                    
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "data_call", 9)){ // data_call <0/1/2> <cid> <timeout>
                // data_call <0/1/2> <cid> <type> <re-con> <user> <pass>
                qser_data_call_s qser_data_call = {0};
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
                qser_data_call_ip_family_e ip_type = atoi(ptr);

                ptr = strstr(ptr, " ");
                if(ptr == NULL)
                {
                    
                }
                else
                {
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    qser_data_call.reconnect = atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(qser_data_call.cdma_username, ptr, strlen(ptr));
                    char *tmp = qser_data_call.cdma_username;
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
                    memcpy(qser_data_call.cdma_password, ptr, strlen(ptr));
                    tmp = qser_data_call.cdma_password;
                    while(*tmp) {
                        if(*tmp == ' ') {
                            *tmp = '\0';
                            break;
                        }
                        tmp++;
                    }
                }
#if 1
                qser_data_call_error_e qser_err;
                switch (type)
                {
                    case 0:
                        err = qser_data_call_stop(cid, ip_type, &qser_err);
                        break;
                    case 1:
                        qser_data_call.profile_idx = cid;
                        qser_data_call.ip_family = ip_type;
                        err = qser_data_call_start(&qser_data_call, &qser_err);
                        break;
                    case 2: {
                        qser_data_call_info_s info;
                        err = qser_data_call_info_get(cid, ip_type, &info, &qser_err);
                        if(!err) {
                            printf("cid : %d, ip_type : %d\n", info.profile_idx, info.ip_family);
                            if(info.v4.state) {
                                LOGE("[qser_data_call] IP: 0x%08x pri_DNS: 0x%08x sec_DNS: 0x%08x.", info.v4.addr.ip.s_addr, info.v4.addr.pri_dns.s_addr, info.v4.addr.sec_dns.s_addr);
                                //printf("%s: %s, %s, %s\n", info.v4.name, inet_ntoa(info.v4.addr.ip), inet_ntoa(info.v4.addr.pri_dns), inet_ntoa(info.v4.addr.sec_dns));
                                printf("%s: %s ", info.v4.name, inet_ntoa(info.v4.addr.ip));
                                printf("%s ", inet_ntoa(info.v4.addr.pri_dns));
                                printf("%s\n", inet_ntoa(info.v4.addr.sec_dns));
                            } else {
                                printf("IPV4 not available.\n");
                            }

                            if(info.v6.state) {
                                char IP_buf[128] = {0};
                                char pri_dns_buf[128] = {0};
                                char sec_dns_buf[128] = {0};
                                ipv6_2_str(&(info.v6.addr.ip), IP_buf);
                                ipv6_2_str(&(info.v6.addr.pri_dns), pri_dns_buf);
                                ipv6_2_str(&(info.v6.addr.sec_dns), sec_dns_buf);
                                printf("%s: %s, %s, %s\n", info.v6.name, IP_buf, pri_dns_buf, sec_dns_buf);
                            } else {
                                printf("IPV6 not available.\n");
                            }
                        }
                        break;
                    }
                    case 3:
                    {
                        qser_data_call.profile_idx = cid;
                        qser_data_call.ip_family = ip_type;
                        err = qser_data_call_start_async(&qser_data_call, &qser_err);
                        break;
                    }
                    default:
                        printf("Type error:%d\n", type);
                        break;
                }
#endif
                printf("qser_err: %d\n", qser_err);
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

