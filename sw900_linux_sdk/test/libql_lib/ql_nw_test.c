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

#include "ql/ql_nw.h"
#include "mbtk_log.h"

static void help()
{
    printf("net_pref : Get net prefferred.\n");
    printf("net_pref <net_pref> <roaming> : Set net prefferred.\n");
    printf("net_time : Get network time.\n");
    printf("operator : Get current operator information.\n");
    printf("net_avail : Get available networks.\n");
    printf("reg : Get current reg info.\n");
    printf("sel_mode: Get network select mode.\n");
    printf("sel_mode <sel_mode> <net_type> <plmn>: Set network select mode.\n");
    printf("signal : Get current signal.\n");
    printf("cell : Get cell info.\n");
    printf("volte : Get VOLTE state.\n");
    printf("csq_signal : Get current csq signal.\n");
}

static int proc_exit()
{
    QL_NW_ERROR_CODE err = ql_nw_release();
    if(QL_NW_SUCCESS != err)
    {
        printf("ql_nw_release fail.");
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

    QL_NW_ERROR_CODE err = ql_nw_init();
    if(QL_NW_SUCCESS != err)
    {
        printf("ql_nw_init fail.");
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

            if(!strncasecmp(cmd, "net_pref", 8)){
                QL_NW_CONFIG_INFO_T net_pref;
                if(!strcasecmp(cmd, "net_pref")) { // Get
                    err = ql_nw_get_config(&net_pref);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("net_pref : %d, roaming : %d\n", net_pref.preferred_nw_mode, net_pref.roaming_pref);
                    }
                } else { // Set
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    net_pref.preferred_nw_mode = atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    net_pref.roaming_pref = atoi(ptr);

                    err = ql_nw_set_config(&net_pref);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("net_pref set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "net_time", 8)){
                QL_NW_NITZ_TIME_INFO_T time;
                err = ql_nw_get_nitz_time_info(&time);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Time : %s, %ld, %d\n", time.nitz_time, time.abs_time, time.leap_sec);
                }
            } else if(!strncasecmp(cmd, "operator", 8)){
                QL_NW_OPERATOR_INFO_T operator;
                err = ql_nw_get_operator_name(&operator);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Operator : %s, %s, %s, %s\n", operator.long_eons, operator.short_eons, operator.mcc, operator.mnc);
                }
            } else if(!strncasecmp(cmd, "net_avail", 9)){
                QL_NW_SCAN_RESULT_LIST_INFO_T nets;
                err = ql_nw_perform_scan(&nets);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    if(nets.entry_len > 0) {
                        int i = 0;
                        for(; i < nets.entry_len; i++) {
                            printf("Net %d: %d, %d, %s, %s, %s, %s\n", i+1, nets.entry[i].status, nets.entry[i].act,
                                nets.entry[i].operator_name.long_eons, nets.entry[i].operator_name.short_eons,
                                nets.entry[i].operator_name.mcc, nets.entry[i].operator_name.mnc);
                        }
                    }
                }
            } else if(!strncasecmp(cmd, "reg", 3)){
                QL_NW_REG_STATUS_INFO_T reg;
                err = ql_nw_get_reg_status(&reg);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Data Reg:%d, %d, %x, %x\n", reg.data_reg.state, reg.data_reg.rat, reg.data_reg.lac, reg.data_reg.cid);
                    printf("Voice Reg:%d, %d, %x, %x\n", reg.voice_reg.state, reg.voice_reg.rat, reg.voice_reg.lac, reg.voice_reg.cid);
                }
            } else if(!strncasecmp(cmd, "sel_mode", 8)){ // "sel_mode" or "sel_mode 460 00 7"
                QL_NW_SELECTION_INFO_T net;
                memset(&net, 0, sizeof(QL_NW_SELECTION_INFO_T));
                if(!strcasecmp(cmd, "sel_mode")) { // Get
                    err = ql_nw_get_selection(&net);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Net : %d, %s, %s, %d\n", net.nw_selection_mode , net.mcc , net.mnc , net.act);
                    }
                } else { // Set
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    net.nw_selection_mode = atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    //net.mcc = (uint8)atoi(ptr);
                    memcpy(net.mcc, ptr, 4);
                    int i = 0;
                    while(i < 4) {
                        if(net.mcc[i] == ' ') {
                            net.mcc[i] = '\0';
                            break;
                        }
                        i++;
                    }

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    //net.mnc = (uint32)atoi(ptr);
                    memcpy(net.mnc, ptr, 4);
                    i = 0;
                    while(i < 4) {
                        if(net.mnc[i] == ' ') {
                            net.mnc[i] = '\0';
                            break;
                        }
                        i++;
                    }

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    net.act = (QL_NW_ACCESS_TECHNOLOGY)atoi(ptr);

                    err = ql_nw_set_selection(&net);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Net select mode set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "signal", 6)){
                QL_NW_SIGNAL_STRENGTH_INFO_T sig;
                err = ql_nw_get_signal_strength(&sig);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Signal GSM:%d, %d, %d, %d\n", sig.GW_SignalStrength.rscp, sig.GW_SignalStrength.bitErrorRate, sig.GW_SignalStrength.rscp, sig.GW_SignalStrength.ecio);
                    printf("Signal LTE:%d, %d, %d, %d, %d\n", sig.LTE_SignalStrength.rssi , sig.LTE_SignalStrength.rsrp, sig.LTE_SignalStrength.rsrq, sig.LTE_SignalStrength.rssnr, sig.LTE_SignalStrength.cqi);
                }
            } else if(!strncasecmp(cmd, "cell", 4)){
                QL_NW_CELL_INFO_T cell;
                err = ql_nw_get_cell_info(&cell);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    int i = 0;
                    if(cell.gsm_info_valid) {
                        while(i < cell.gsm_info_num) {
                            printf("GSM cell %d: %d, %x, %d, %d, %x, %d, %d\n", i + 1, cell.gsm_info[i].flag, cell.gsm_info[i].cid, cell.gsm_info[i].mcc, cell.gsm_info[i].mnc, cell.gsm_info[i].lac, cell.gsm_info[i].arfcn, cell.gsm_info[i].bsic);
                            i++;
                        }
                    }

                    if(cell.umts_info_valid) {
                        i = 0;
                        while(i < cell.umts_info_num) {
                            printf("UMTS cell %d: %d, %x, %x, %d, %d, %x, %d, %d\n", i + 1, cell.umts_info[i].flag , cell.umts_info[i].cid, cell.umts_info[i].lcid, cell.umts_info[i].mcc, cell.umts_info[i].mnc, cell.umts_info[i].lac, cell.umts_info[i].uarfcn, cell.umts_info[i].psc);
                            i++;
                        }
                    }

                    if(cell.lte_info_valid) {
                        i = 0;
                        while(i < cell.lte_info_num) {
                            printf("LTE cell %d: %d, %x, %d, %d, %x, %d, %d\n", i + 1, cell.lte_info[i].flag, cell.lte_info[i].cid, cell.lte_info[i].mcc, cell.lte_info[i].mnc, cell.lte_info[i].tac, cell.lte_info[i].pci, cell.lte_info[i].earfcn);
                            i++;
                        }
                    }
                }
            } else if(!strncasecmp(cmd, "volte", 5)){
                VOLTE_STATE state;
                err = ql_nw_get_volte_state(&state);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("VOLTE state : %d\n", state.reg_state);
                }
            } else if(!strncasecmp(cmd, "csq_signal", 10)){
                QL_NW_CSQ_SIGNAL_STRENGTH_INFO_T signal;
                err = ql_nw_csq_get_signal_strength(&signal);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("+CSQ : %d, %d\n", signal.rssi, signal.bitErrorRate);
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

