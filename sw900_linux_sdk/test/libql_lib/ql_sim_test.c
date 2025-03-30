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

#include "ql/ql_sim.h"
#include "mbtk_log.h"

static void help()
{
    printf("imsi : Get IMSI.\n");
    printf("iccid : Get ICCID.\n");
    printf("pn : Get phone number.\n");
    printf("pin_en <pin> : Enable pin.\n");
    printf("pin_dis <pin> : Disable pin.\n");
    printf("pin_ch <old_pin> <new_pin> : Change pin.\n");
    printf("pin_verify <pin> : Verify pin.\n");
    printf("puk_unlock <puk> <new_pin> : Unlock using PUK.\n");
    printf("sim : Get sim state.\n");
}

static int proc_exit()
{
    QL_SIM_ERROR_CODE err = ql_sim_release();
    if(QL_SIM_SUCCESS != err)
    {
        printf("ql_sim_release fail.");
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

    QL_SIM_ERROR_CODE err = ql_sim_init();
    if(QL_SIM_SUCCESS != err)
    {
        printf("ql_sim_init fail.");
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

            if(!strncasecmp(cmd, "imsi", 4)){
                char imsi[30];
                err = ql_sim_get_imsi(imsi, 30);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("IMSI : %s\n", imsi);
                }
            }else if(!strncasecmp(cmd, "iccid", 5)){
                char iccid[30];
                err = ql_sim_get_iccid(iccid, 30);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("ICCID : %s\n", iccid);
                }
            }else if(!strncasecmp(cmd, "pn", 2)){
                char phonenumber[30];
                err = ql_sim_get_phonenumber(phonenumber, 30);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("PhoneNumber : %s\n", phonenumber);
                }
            }else if(!strncasecmp(cmd, "pin_en", 6)){ // pin_en <pin>
                QL_SIM_VERIFY_PIN_INFO pin = {0};
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                memcpy(pin.pin_value, ptr, strlen(ptr));

                err = ql_sim_enable_pin(&pin);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Enable PIN(%s) success.\n", pin.pin_value);
                }
            }else if(!strncasecmp(cmd, "pin_dis", 7)){ // pin_dis <pin>
                QL_SIM_VERIFY_PIN_INFO pin = {0};
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                memcpy(pin.pin_value, ptr, strlen(ptr));

                err = ql_sim_disable_pin(&pin);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Disable PIN(%s) success.\n", pin.pin_value);
                }
            }else if(!strncasecmp(cmd, "pin_ch", 6)){ // pin_ch <old_pin> <new_pin>
                QL_SIM_CHANGE_PIN_INFO pin = {0};
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;

                char *tmp = pin.old_pin_value;
                while(*ptr != '\0' && *ptr != ' ' && *ptr != '\r' && *ptr != '\n') {
                    *tmp++ = *ptr++;
                }
                *tmp = '\0';
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;

                tmp = pin.new_pin_value;
                while(*ptr != '\0' && *ptr != ' ' && *ptr != '\r' && *ptr != '\n') {
                    *tmp++ = *ptr++;
                }
                *tmp = '\0';

                err = ql_sim_change_pin(&pin);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Change PIN(%s -> %s) success.\n", pin.old_pin_value, pin.new_pin_value);
                }
            }else if(!strncasecmp(cmd, "pin_verify", 10)){ // pin_verify <pin>
                QL_SIM_VERIFY_PIN_INFO pin = {0};
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                memcpy(pin.pin_value, ptr, strlen(ptr));

                err = ql_sim_verify_pin(&pin);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Verify PIN(%s) success.\n", pin.pin_value);
                }
            }else if(!strncasecmp(cmd, "puk_unlock", 10)){ // puk_unlock <puk> <new_pin>
                QL_SIM_UNBLOCK_PIN_INFO pin = {0};
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;

                char *tmp = pin.puk_value;
                while(*ptr != '\0' && *ptr != ' ' && *ptr != '\r' && *ptr != '\n') {
                    *tmp++ = *ptr++;
                }
                *tmp = '\0';
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;

                tmp = pin.new_pin_value;
                while(*ptr != '\0' && *ptr != ' ' && *ptr != '\r' && *ptr != '\n') {
                    *tmp++ = *ptr++;
                }
                *tmp = '\0';

                err = ql_sim_unblock_pin(&pin);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("PUI unlock(PUK:%s   PIN:%s) success.\n", pin.puk_value, pin.new_pin_value);
                }
            }else if(!strncasecmp(cmd, "sim", 3)){
                QL_SIM_CARD_STATUS_INFO sim;
                err = ql_sim_get_card_status(&sim);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Sim type:%d, state:%d, PIN:%d,%d,%d,%d\n", sim.card_type, sim.card_state, sim.card_pin_info.pin1_num_retries, sim.card_pin_info.pin2_num_retries, sim.card_pin_info.puk1_num_retries, sim.card_pin_info.puk2_num_retries);
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

