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

#include "mbtk_ril_api.h"

#define CLI_THREAD_MAX 3

typedef struct {
    pthread_t pid;
    bool is_running;
    char name[20];
} ril_cli_thread_info_t;

static ril_cli_thread_info_t cli_threads[CLI_THREAD_MAX];
static int cli_pid_index = 1;

static void help()
{
    printf("version: Get version.\n");
    printf("imei: Get IMEI.\n");
    printf("sn: Get SN.\n");
    printf("meid: Get MEID.\n");
    printf("volte: Get VoLTE state.\n");
    printf("volte 0: Close VoLTE.\n");
    printf("volte 1: Open VoLTE.\n");
    printf("radio: Get radio state.\n");
    printf("radio 0 [0/1]: Close radio.\n");
    printf("radio 1 [0/1]: Open radio.\n");
    printf("temp <0,1>: Get SOC/RF temperature.\n");
    printf("cell_time: Get cell time.\n");
    printf("sim_state: Get sim state.\n");
    printf("sim_type: Get sim type.\n");
    printf("imsi: Get IMSI.\n");
    printf("iccid: Get ICCID.\n");
    printf("pn: Get Phone Number.\n");
    printf("pin_state:Get Sim lock state.\n");
    printf("pin_times:Get PIN/PUK last times.\n");
    printf("pin_open <PIN>:Enable sim lock.\n");
    printf("pin_close <PIN>:Disable sim lock.\n");
    printf("pin_change <old_pin> <new_pin>:Change sim PIN.\n");
    printf("pin_verify <PIN>:Verify PIN.\n");
    printf("puk_verify <PUK> <PIN>:Verify PUK.\n");
    printf("plmn:Get PLMN List.\n");
    printf("avail_net: Get available network.\n");
    printf("sel_mode: Get network select mode.\n");
    printf("sel_mode <sel_mode> <net_type> <plmn>: Set network select mode.\n");
    printf("band: Get current bands.\n");
    printf("band support: Get support bands.\n");
    printf("band <net_pref> <gsm_band> <umts_band> <tdlte_band> <fddlte_band>: Set current bands.\n");
    printf("signal: Get network signal.\n");
    printf("reg: Get network registe information.\n");
    printf("cell: Get current cell information.\n");
    printf("shutdown <0,1,2>: reboot/poweroff/halt system.\n");
    printf("power_sim <0,1>: Power off/on sim.\n");
    printf("time <0,1,2> YYYY-MM-DD HH:MM:SS : Set system time as CELL/NTP/User.\n");
    printf("apn : Get current apns.\n");
    printf("apn <cid> <ip_type:1/2/3/4> <save:0/1> <auto_call:0/1> <def_route:0/1> <as_dns:0/1> <apn/null> : Set apn.\n");
    printf("apn_del <cid> <save:0/1> : Delete APN.\n");
    printf("data_call <0/1/2> <cid> <auto_boot_call> <def_route> <as_dns>: Stop/Start/State data call.\n");
    printf("call: Call the phone number.\n");
    printf("answer: Answer the phone call.\n");
    printf("hangup: Hang up all phone call. No id.\n");
    printf("hangup 0: Hang up waiting or background phone call.\n");
    printf("hangup 1: Hang up a phone call.\n");
    printf("hangup 2: Hang up a phone call.\n");
    printf("hangup 3: Hangup foreground resume background call.\n");
    printf("waitin: Returns the list of current calls.\n");
    printf("mute: Get mute state.\n");
    printf("mute 0: Close mute.\n");
    printf("mute 1: Open mute.\n");
    printf("dtmf : Set dtmf character[0, 1, 2, ..., A, B, C, D, *, #], duration [300-600].\n   Such as: dtmf 0 300\n");
    printf("ims_en 0/1 : Close/Open IMS(Restart takes effect).\n");
    printf("ims_state : Get IMS open or not?\n");
    printf("ims_reg: Get IMS register state.\n");
}

static void thread_exit_with_wait()
{
    int i = 0;
    while(i < CLI_THREAD_MAX) {
        cli_threads[i].is_running = FALSE;
        i++;
    }

    i = 0;
    while(i < CLI_THREAD_MAX) {
        if(cli_threads[i].pid) {
            pthread_join(cli_threads[i].pid, NULL);
            printf("Thread (%s) exit.\n", cli_threads[i].name);
        }
        i++;
    }
}

static void sig_process(int sig)
{
    LOGI("I got signal %d\n", sig);
    switch(sig)
    {
        case SIGINT: // Ctrl + C
        {
            LOGI("Exit by SIGINT.\n");
            thread_exit_with_wait();
            mbtk_ril_deinit();
            exit(0);
        }
        case SIGQUIT: // Ctrl + \ (类似 SIGINT ，但要产生core文件)
        {
            LOGI("Exit by SIGQUIT.\n");
            thread_exit_with_wait();
            mbtk_ril_deinit();
            exit(0);
        }
        case SIGTERM:// 默认kill   (同 SIGKILL ，但 SIGKILL 不可捕获)
        {
            LOGI("Exit by SIGTERM.\n");
            thread_exit_with_wait();
            mbtk_ril_deinit();
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

static char* thread_id2name(pthread_t pid)
{
    int i = 0;
    while(i < CLI_THREAD_MAX) {
        if(pid == cli_threads[i].pid) {
            return cli_threads[i].name;
        }
        i++;
    }

    return "UNKNOWN";
}

static void* sub_thread_run(void *arg)
{
    ril_cli_thread_info_t *cli = (ril_cli_thread_info_t*)arg;
    cli->pid = pthread_self();
    cli->is_running = TRUE;
    sprintf(cli->name, "PID-%d", cli_pid_index++);

    printf("[%s] enter.\n", thread_id2name(cli->pid));
    while(cli->is_running) {
        srand((int)(time(0) + cli->pid));
        int time_sec = 1 + (int)(10.0 * rand() / ( RAND_MAX + 1.0));
        char version[50] = {0};
        mbtk_ril_err_enum err = mbtk_version_get(version);
        if(err != MBTK_RIL_ERR_SUCCESS) {
            printf("[%s : %ds]Error : %d\n", thread_id2name(cli->pid), time_sec, err);
        } else {
            printf("[%s : %ds]Version : %s\n", thread_id2name(cli->pid), time_sec, version);
        }

        sleep(time_sec);
    }
    printf("[%s] exit.\n", thread_id2name(cli->pid));
    return NULL;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_process);
    signal(SIGQUIT, sig_process);
    signal(SIGTERM, sig_process);
    //signal(SIGTSTP, sig_process);
    //signal(SIGSEGV, sig_process);

    mbtk_log_init("radio","RIL_CLI");

#ifdef MBTK_DUMP_SUPPORT
    mbtk_debug_open(NULL, TRUE);
#endif

    //test2(0, "192.168.1.198");
    //test2(1, "2409:8162:140:cd3c:1:2:1494:72ba");
    //test2(1, "254.128.0.0.0.0.0.0.0.1.0.2.144.5.212.239");
    //test2(1, "2400:3200::1");

    if(mbtk_ril_init() != MBTK_RIL_ERR_SUCCESS)
    {
        return -1;
    }

#if 0
    memset(cli_threads, 0, sizeof(ril_cli_thread_info_t) * CLI_THREAD_MAX);

    pthread_t pid1, pid2, pid3;
    if(pthread_create(&pid1, NULL, sub_thread_run, &cli_threads[0]))
    {
        printf("pthread_create() fail.");
        goto exit;
    }

    if(pthread_create(&pid2, NULL, sub_thread_run, &cli_threads[1]))
    {
        printf("pthread_create() fail.");
        goto exit;
    }

    if(pthread_create(&pid3, NULL, sub_thread_run, &cli_threads[2]))
    {
        printf("pthread_create() fail.");
        goto exit;
    }
#endif
    printf(">>>>>>>>>>>>>>>>>>>>>>>>Enter cmd:\n");
    char cmd[100];
    while(1)
    {
        memset(cmd, 0, 100);
        mbtk_ril_err_enum err;
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
                err = mbtk_version_get(version);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Version : %s\n", version);
                }
            }
            else if(!strncasecmp(cmd, "imei", 4)){
                char imei[50] = {0};
                err = mbtk_imei_get(imei);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("IMEI : %s\n", imei);
                }
            } else if(!strncasecmp(cmd, "sn", 2)){
                char sn[50] = {0};
                err = mbtk_sn_get(sn);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("SN : %s\n", sn);
                }
            } else if(!strncasecmp(cmd, "meid", 4)){
                char meid[50] = {0};
                err = mbtk_meid_get(meid);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("MEID : %s\n", meid);
                }
            } else if(!strncasecmp(cmd, "volte", 5)){ // "volte" or "volte 0" or "volte 1"
                int volte;
                if(!strcasecmp(cmd, "volte")) { // Get
                    err = mbtk_volte_state_get(&volte);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE : %d\n", volte);
                    }
                } else { // Set
                    if(!strcasecmp(cmd, "volte 1")) { // Open VoLTE
                        volte = 1;
                    } else { // Close VoLTE
                        volte = 0;
                    }
                    err = mbtk_volte_state_set(volte);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "radio", 5)){ // "radio" or "radio 0" or "radio 1"
                mbtk_radio_state_enum radio;
                if(!strcasecmp(cmd, "radio")) { // Get
                    err = mbtk_radio_state_get(&radio);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Radio : %d\n", radio);
                    }
                } else { // Set
                    int reset;
                    int count = sscanf(cmd, "radio %d %d", &radio, &reset);
                    if(count > 0) {
                        if(count == 1) {
                            reset = 0;
                        }

                        err = mbtk_radio_state_set(radio, reset);
                        if(err != MBTK_RIL_ERR_SUCCESS) {
                            printf("Error : %d\n", err);
                        } else {
                            printf("Radio set success\n");
                        }
                    }
                }
            } else if(!strncasecmp(cmd, "temp", 4)){
                int temp;
                if(!strcasecmp(cmd, "temp 0")) {
                    err = mbtk_temp_get(MBTK_TEMP_TYPE_SOC, &temp);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("SOC : %d\n", temp);
                    }
                } else if(!strcasecmp(cmd, "temp 1")) {
                    err = mbtk_temp_get(MBTK_TEMP_TYPE_RF, &temp);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("RF : %d\n", temp);
                    }
                }
            } else if(!strncasecmp(cmd, "cell_time", 9)){
                char time[128] = {0};
                err = mbtk_cell_time_get(time);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Cell Time : %s\n", time);
                }
            } else if(!strncasecmp(cmd, "sim_state", 9)){
                mbtk_sim_state_enum sim;
                err = mbtk_sim_state_get(&sim);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Sim State : %d\n", sim);
                }
            } else if(!strncasecmp(cmd, "sim_type", 8)){
                mbtk_sim_card_type_enum type;
                err = mbtk_sim_type_get(&type);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Sim Type : %d\n", type);
                }
            } else if(!strncasecmp(cmd, "imsi", 4)){
                char imsi[50] = {0};
                err = mbtk_imsi_get(imsi);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("IMSI : %s\n", imsi);
                }
            } else if(!strncasecmp(cmd, "iccid", 5)){
                char iccid[50] = {0};
                err = mbtk_iccid_get(iccid);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("ICCID : %s\n", iccid);
                }
            } else if(!strncasecmp(cmd, "pn", 2)){
                char phone_number[50] = {0};
                err = mbtk_phone_number_get(phone_number);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Phone Number : %s\n", phone_number);
                }
            }
            /*
                printf("pin_state:Get Sim lock state.\n");
                printf("pin_times:Get PIN/PUK last times.\n");
                printf("pin_open <PIN>:Enable sim lock.\n");
                printf("pin_close <PIN>:Disable sim lock.\n");
                printf("pin_change <old_pin> <new_pin>:Change sim PIN.\n");
                printf("pin_verify <PIN>:Verify PIN.\n");
                printf("puk_verify <PUK> <PIN>:Verify PUK.\n");
            */
            else if(!strncasecmp(cmd, "pin_state", 9)){
                int lock_state;
                err = mbtk_sim_lock_get(&lock_state);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("PIN state : %d\n", lock_state);
                }
            } else if(!strncasecmp(cmd, "pin_times", 9)){
                mbtk_pin_puk_last_times_t times;
                err = mbtk_sim_lock_retry_times_get(&times);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("PIN/PUK retry times:%d,%d,%d,%d\n", times.p1_retry, times.p2_retry, times.puk1_retry, times.puk2_retry);
                }
            } else if(!strncasecmp(cmd, "pin_open", 8)){
                mbtk_sim_lock_info_t info;
                info.type = MBTK_SIM_LOCK_TYPE_ENABLE;
                int count = sscanf(cmd, "pin_open %s", info.pin1);
                if(count == 1) {
                    err = mbtk_sim_lock_set(&info);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Enable sim lock success.\n");
                    }
                }
            } else if(!strncasecmp(cmd, "pin_close", 9)){
                mbtk_sim_lock_info_t info;
                info.type = MBTK_SIM_LOCK_TYPE_DISABLE;
                int count = sscanf(cmd, "pin_close %s", info.pin1);
                if(count == 1) {
                    err = mbtk_sim_lock_set(&info);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Disable sim lock success.\n");
                    }
                }
            } else if(!strncasecmp(cmd, "pin_change", 10)){
                mbtk_sim_lock_info_t info;
                info.type = MBTK_SIM_LOCK_TYPE_CHANGE;
                int count = sscanf(cmd, "pin_change %s %s", info.pin1, info.pin2);
                if(count == 2) {
                    err = mbtk_sim_lock_set(&info);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("PIN change success.\n");
                    }
                }
            } else if(!strncasecmp(cmd, "pin_verify", 10)){
                mbtk_sim_lock_info_t info;
                info.type = MBTK_SIM_LOCK_TYPE_VERIFY_PIN;
                int count = sscanf(cmd, "pin_verify %s", info.pin1);
                if(count == 1) {
                    err = mbtk_sim_lock_set(&info);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("PIN verify success.\n");
                    }
                }
            } else if(!strncasecmp(cmd, "puk_verify", 10)){
                mbtk_sim_lock_info_t info;
                info.type = MBTK_SIM_LOCK_TYPE_VERIFY_PUK;
                int count = sscanf(cmd, "puk_verify %s %s", info.puk, info.pin1);
                if(count == 2) {
                    err = mbtk_sim_lock_set(&info);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("PUK verify success.\n");
                    }
                }
            } else if(!strncasecmp(cmd, "plmn", 4)){
                mbtk_plmn_info info;
                err = mbtk_plmn_list_get(&info);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("PLMN number:%d\n", info.count);
                    int i = 0;
                    while(i < info.count) {
                        printf("%d,%d,%s\n", i+1, info.plmn[i].format, info.plmn[i].name);
                        i++;
                    }
                }
            }
            else if(!strncasecmp(cmd, "avail_net", 9)){
                mbtk_net_info_array_t net_list;
                err = mbtk_available_net_get(&net_list);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Available net number:%d\n", net_list.num);
                    int i = 0;
                    while(i < net_list.num) {
                        printf("NET : %d,%d,%d,%d\n", net_list.net_info[i].net_sel_mode,
                            net_list.net_info[i].net_type, net_list.net_info[i].net_state,
                            net_list.net_info[i].plmn);
                        i++;
                    }
                }
            } else if(!strncasecmp(cmd, "sel_mode", 8)){ // "sel_mode" or "sel_mode 1 7 46000"
                mbtk_net_info_t net;
                memset(&net, 0, sizeof(mbtk_net_info_t));
                if(!strcasecmp(cmd, "sel_mode")) { // Get
                    err = mbtk_net_sel_mode_get(&net);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Net : %d, %d, %d, %d\n", net.net_sel_mode, net.net_type, net.net_state, net.plmn);
                    }
                } else { // Set
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    net.net_sel_mode = (uint8)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    net.net_type = (uint8)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    net.plmn = (uint32)atoi(ptr);

                    err = mbtk_net_sel_mode_set(&net);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Net select mode set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "band", 4)){ // "band" or "band support" or "band 0 79 147 482 524503"
                mbtk_band_info_t band;
                memset(&band, 0x0, sizeof(mbtk_band_info_t));
                if(!strcasecmp(cmd, "band")) { // Get
                    err = mbtk_current_band_get(&band);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Band : %d, %d, %d, %d, %d, %d\n", band.net_pref, band.gsm_band, band.umts_band, band.tdlte_band, band.fddlte_band, band.lte_ext_band);
                    }
                } else if(!strcasecmp(cmd, "band support")) { // Get
                    err = mbtk_support_band_get(&band);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Band : %d, %d, %d, %d, %d, %d\n", band.net_pref, band.gsm_band, band.umts_band, band.tdlte_band, band.fddlte_band, band.lte_ext_band);
                    }
                } else { // "band 0 79 147 482 524503"
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    band.net_pref = (uint8)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    band.gsm_band = (uint16)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    band.umts_band = (uint16)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    band.tdlte_band = (uint32)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    band.fddlte_band = (uint32)atoi(ptr);

                    err = mbtk_current_band_set(&band);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Band set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "signal", 6)){
                mbtk_signal_info_t signal;
                err = mbtk_net_signal_get(&signal);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Signal : %d, %d, %d, %d, %d, %d, %d, %d\n", signal.type, signal.rssi, signal.rxlev, signal.ber,
                        signal.rscp, signal.ecno, signal.rsrq, signal.rsrp);
                }
            } else if(!strncasecmp(cmd, "reg", 3)){
                mbtk_net_reg_info_t reg;
                err = mbtk_net_reg_get(&reg);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("REG : call_state=%d, data_state=%d, ims_state=%d, net_type=%d, %04x, %08x\n", reg.call_state, reg.data_state, reg.ims_state, reg.type, reg.lac, reg.ci);
                }
            } else if(!strncasecmp(cmd, "apn_del", 7)) {
                mbtk_apn_info_t apn;
                memset(&apn, 0, sizeof(mbtk_apn_info_t));
                int cid, auto_save;
                int count = sscanf(cmd, "apn_del %d %d", &cid, &auto_save);
                if(count == 2) {
                    apn.cid = (mbtk_ril_cid_enum)cid;
                    apn.auto_save = (uint8)auto_save;
                }
                err = mbtk_apn_set(&apn);
                if(err != MBTK_RIL_ERR_SUCCESS) {
                    printf("Error : %d\n", err);
                } else {
                    printf("APN delete success\n");
                }

            } else if(!strncasecmp(cmd, "apn", 3)){
                if(!strcasecmp(cmd, "apn")) { // Get apn
                    mbtk_apn_info_array_t apns;
                    err = mbtk_apn_get(&apns);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN Num:%d\n", apns.num);
                        int i = 0;
                        while(i < apns.num) {
                            // printf("APN : %d, %s, %s\n", apns.apns[i].cid, apn2str(apns.apns[i].ip_type), apns.apns[i].apn);
                            printf("APN : %d, %s, auth-%d, auto_save-%d, auto_boot_call-%d, %s, %s, %s, %s\n", apns.apns[i].cid, apn2str(apns.apns[i].ip_type),
                                apns.apns[i].auth, apns.apns[i].auto_save, apns.apns[i].auto_boot_call,
                                str_empty(apns.apns[i].apn) ? "NULL" : apns.apns[i].apn,
                                str_empty(apns.apns[i].user) ? "NULL" : apns.apns[i].user,
                                str_empty(apns.apns[i].pass) ? "NULL" : apns.apns[i].pass,
                                str_empty(apns.apns[i].type) ? "NULL" : apns.apns[i].type);
                            i++;
                        }
                        printf("Def route : %d, def dns : %d\n", apns.cid_for_def_route, apns.cid_for_def_dns);
                    }
                } else { // apn <cid> <0/1/2/3> <0/1> <0/1> <apn>
                    mbtk_apn_info_t apn;
                    memset(&apn, 0, sizeof(mbtk_apn_info_t));
#if 1
                    int cid, ip_type, auto_save, auto_boot_call, def_route, as_dns;
                    int count = sscanf(cmd, "apn %d %d %d %d %d %d %s",
                                    &cid, &ip_type, &auto_save, &auto_boot_call,
                                    &def_route, &as_dns, apn.apn);

                    // Delete APN
                    if(strcmp(apn.apn,"null") == 0 || strcmp(apn.apn,"NULL") == 0) {
                        memset(apn.apn, 0, sizeof(apn.apn));
                    }
                    if(count == 7) {
                        apn.cid = (mbtk_ril_cid_enum)cid;
                        apn.ip_type = (mbtk_ip_type_enum)ip_type;
                        apn.auto_save = (uint8)auto_save;
                        apn.auto_boot_call = (uint8)auto_boot_call;
                        apn.def_route = (uint8)def_route;
                        apn.as_dns = (uint8)as_dns;
                    }
#else
                    char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    apn.cid = (mbtk_ril_cid_enum)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    apn.ip_type = (mbtk_ip_type_enum)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    memcpy(apn.apn, ptr, strlen(ptr));
#endif
                    err = mbtk_apn_set(&apn);
                    if(err != MBTK_RIL_ERR_SUCCESS) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN set success\n");
                    }
                }
            }
            else if(!strncasecmp(cmd, "data_call", 9)){ // data_call <0/1/2> <cid> <timeout>
                // mbtk_ril_cid_enum cid, bool auto_boot_call, bool def_route, int retry_interval, int timeout, mbtk_ip_info_t *rsp_info
                // data_call <0/1/2> <cid> <timeout>
                int type, cid, auto_boot_call, def_route, as_dns;
                int count = sscanf(cmd, "data_call %d %d %d %d %d", &type, &cid, &auto_boot_call, &def_route, &as_dns);
                if(count != 5) {
                    count = sscanf(cmd, "data_call %d %d", &type, &cid);
                }

                if(count == 5 || count == 2) {
                    mbtk_ip_info_t ip;
                    memset(&ip, 0, sizeof(mbtk_ip_info_t));
                    if(type == MBTK_DATA_CALL_START) {
                        err = mbtk_data_call_start((mbtk_ril_cid_enum)cid, (bool)auto_boot_call, (bool)def_route, (bool)as_dns, 10, 10, &ip);
                    } else if(type == MBTK_DATA_CALL_STOP) {
                        err = mbtk_data_call_stop((mbtk_ril_cid_enum)cid, 10);
                    } else {
                        err = mbtk_data_call_state_get((mbtk_ril_cid_enum)cid, &ip);
                    }
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("DATA_CALL success\n");
                        if(type == MBTK_DATA_CALL_START || type == MBTK_DATA_CALL_STATE) {
                            if(ip.ipv4.valid) {
                                // log_hex("IPv4", &ipv4, sizeof(mbtk_ipv4_info_t));
                                char ip_tmp[20];

                                memset(ip_tmp, 0, 20);
                                if(inet_ntop(AF_INET, &(ip.ipv4.IPAddr), ip_tmp, 20) == NULL) {
                                    printf("IP error.\n");
                                } else {
                                    printf("IP : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 20);
                                if(inet_ntop(AF_INET, &(ip.ipv4.PrimaryDNS), ip_tmp, 20) == NULL) {
                                    printf("PrimaryDNS error.\n");
                                } else {
                                    printf("PrimaryDNS : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 20);
                                if(inet_ntop(AF_INET, &(ip.ipv4.SecondaryDNS), ip_tmp, 20) == NULL) {
                                    printf("SecondaryDNS error.\n");
                                } else {
                                    printf("SecondaryDNS : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 20);
                                if(inet_ntop(AF_INET, &(ip.ipv4.GateWay), ip_tmp, 20) == NULL) {
                                    printf("GateWay error.\n");
                                } else {
                                    printf("GateWay : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 20);
                                if(inet_ntop(AF_INET, &(ip.ipv4.NetMask), ip_tmp, 20) == NULL) {
                                    printf("NetMask error.\n");
                                } else {
                                    printf("NetMask : %s\n", ip_tmp);
                                }
                            }

                            if(ip.ipv6.valid) {
                                // log_hex("IPv6", &ipv6, sizeof(mbtk_ipv6_info_t));
                                char ip_tmp[50];

                                memset(ip_tmp, 0, 50);
                                if(ipv6_2_str(&(ip.ipv6.IPV6Addr), ip_tmp)) {
                                    printf("IP error.\n");
                                } else {
                                    printf("IP : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 50);
                                if(ipv6_2_str(&(ip.ipv6.PrimaryDNS), ip_tmp)) {
                                    printf("PrimaryDNS error.\n");
                                } else {
                                    printf("PrimaryDNS : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 50);
                                if(ipv6_2_str(&(ip.ipv6.SecondaryDNS), ip_tmp)) {
                                    printf("SecondaryDNS error.\n");
                                } else {
                                    printf("SecondaryDNS : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 50);
                                if(ipv6_2_str(&(ip.ipv6.GateWay), ip_tmp)) {
                                    printf("GateWay error.\n");
                                } else {
                                    printf("GateWay : %s\n", ip_tmp);
                                }

                                memset(ip_tmp, 0, 50);
                                if(ipv6_2_str(&(ip.ipv6.NetMask), ip_tmp)) {
                                    printf("NetMask error.\n");
                                } else {
                                    printf("NetMask : %s\n", ip_tmp);
                                }
                            }
                        }
                    }
                }
            }
            else if(!strncasecmp(cmd, "cell", 4)){
                char *ptr = strstr(cmd, ",");	 //CPMS,ME,ME,ME
				if(ptr == NULL)
				{
                    mbtk_cell_info_array_t cell;
                    err = mbtk_cell_get(&cell);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else if(cell.num > 0){
                        // Current server cell.
                        switch(cell.type)
                        {
                            case MBTK_CELL_TYPE_GSM:
                                printf("GSM : lac=%d, ci=%d, arfcn=%d, bsic=%d\n", cell.cell[0].value1, cell.cell[0].value2, cell.cell[0].value3, cell.cell[0].value4);
                                break;
                            case MBTK_CELL_TYPE_UMTS:
                                printf("UMTS : lac=%d, ci=%d, arfcn=%d\n", cell.cell[0].value1, cell.cell[0].value2, cell.cell[0].value3);
                                break;
                            case MBTK_CELL_TYPE_LTE:
                                printf("LTE : tac=%d, PCI=%d, dlEuarfcn=%d, ulEuarfcn=%d, band=%d\n", cell.cell[0].value1, cell.cell[0].value2, cell.cell[0].value3, cell.cell[0].value4, cell.cell[0].value5);
                                break;
                            default:
                                break;
                        }

                        int i = 1;
                        while (i < cell.num)
                        {
                            switch(cell.type)
                            {
                                case MBTK_CELL_TYPE_GSM:
                                    printf("CELL : %d, %d, %d, %d, %d", cell.cell[i].value1, cell.cell[i].value2, cell.cell[i].value3, cell.cell[i].value4, cell.cell[i].value5);
                                    break;
                                case MBTK_CELL_TYPE_UMTS:
                                    printf("CELL : lac=%d, ci=%d, arfcn=%d\n", cell.cell[i].value1, cell.cell[i].value2, cell.cell[i].value3);
                                    break;
                                case MBTK_CELL_TYPE_LTE:
                                    printf("CELL : phyCellId=%d, euArfcn=%d, rsrp=%d, rsrq=%d\n", cell.cell[i].value1, cell.cell[i].value2, cell.cell[i].value3, cell.cell[i].value4);
                                    break;
                                default:
                                    break;
                            }
                            i++;
                        }
                    } else {
                        printf("Cell no found.");
                    }
                }
                else{
                    char *ptr = strstr(cmd, ",");	 //cell,2,3,,40936,430
					char mem[50]={0};
					char resp[1024] = {0};
					if(ptr != NULL)
					{
						ptr++;
						memset(mem, 0, sizeof(mem));
						memcpy(mem, ptr, strlen(ptr));
						printf("cell:%s\n", mem);
					}
					printf("cell_mem: %s \n", mem);

					memset(resp, 0, sizeof(resp));
					err = mbtk_cell_set(mem, resp);
					if(err) {
						printf("Error : %d\n", err);
					} else {
						printf("cell set . resp:%s\n", resp);
					}
                }
            }
            else if(!strncasecmp(cmd, "call", 4)){
                char phone_number[12];
                char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                memset(phone_number,0,strlen(phone_number));
                memcpy(phone_number,ptr,strlen(ptr));
                printf("phone number is: %s\n",phone_number);
                err = mbtk_call_start(phone_number);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Call success.\n");
                }
            } else if(!strncasecmp(cmd, "answer", 6)){
                err = mbtk_call_answer();
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Call success.\n");
                }
            } else if(!strncasecmp(cmd, "hangup", 6)){
                int phone_id;
                if(!strcasecmp(cmd, "hangup")) { // hang up all
                    err = mbtk_call_hang();
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Call hang up all.\n");
                    }
                } else if(!strcasecmp(cmd, "hangup 0")) {
                    err = mbtk_waiting_or_background_call_hang();
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Call hang up waiting or background.\n");
                    }
                } else if(!strcasecmp(cmd, "hangup 3")) {
                    err = mbtk_foreground_resume_background_call_hang();
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Call hang up foreground resume background.\n");
                    }
                } else {
                    if(!strcasecmp(cmd, "hangup 1")) { // hang up a call
                        phone_id = 1;
                    } else if(!strcasecmp(cmd, "hangup 2")) {
                        phone_id = 2;
                    } else {
                        printf("Error : Invalid input\n");
                    }
                    err = mbtk_a_call_hang(phone_id);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("A Call hang up.\n");
                    }
                }
            } else if(!strncasecmp(cmd, "waitin", 6)){
                mbtk_call_info_t reg;
                err = mbtk_call_reg_get(&reg);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    if(reg.call_wait == 0) {
                        printf("No call ring\n");
                    }
                    else {
                        printf("RING : %d, %d, %d, %d, %d, %s, %d\n", reg.dir1, reg.dir, reg.state, reg.mode, reg.mpty, reg.phone_number, reg.type);
                    }
                }
            } else if(!strncasecmp(cmd, "mute", 4)){ // "mute" or "mute 0" or "mute 1"
                int mute;
                if(!strcasecmp(cmd, "mute")) { // Get
                    err = mbtk_mute_state_get(&mute);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("mute : %d\n", mute);
                    }
                } else { // Set
                    if(!strcasecmp(cmd, "mute 1")) { // on mute
                        mute = 1;
                    } else { // off mute
                        mute = 0;
                    }
                    err = mbtk_mute_state_set(mute);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("mute set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "DTMF", 4)){ // valid character: (0, 1, ..., 9, A, B, C, D, *, #)

                mbtk_call_dtmf_info_t reg;

                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                reg.character = *ptr;

                ptr = strstr(ptr, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                reg.duration = (uint32)atoi(ptr);
                printf("DTMF character is: %c,%d\n",reg.character, reg.duration);
                err = mbtk_dtmf_send(&reg);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("DTMF success.\n");
                }
            } else if(!strncasecmp(cmd, "cmgf", 4)){ // set mode 0: pud, 1:text
                int mode;
                if(!strcasecmp(cmd, "cmgf")) { // Get
                    err = mbtk_sms_cmgf_get(&mode);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE : %d\n", mode);
                    }
                } else { // Set
                    if(!strcasecmp(cmd, "cmgf 1")) { // cmgf 1
                        mode = 1;
                    } else { //
                        mode = 0;
                    }
                    printf("mode:%d\n", mode);
                    sleep(2);
                   err = mbtk_sms_cmgf_set(mode);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            }else if(!strncasecmp(cmd, "cpms", 4)){ // //CPMS=ME, ME, ME
				char mem[100] = {0};
                char resp[100] = {0};
                if(!strcasecmp(cmd, "cpms")) { // Get
                    err = mbtk_sms_cpms_get(&mem);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("cpms : %s\n", mem);
                    }
                } else { // Set

					char *ptr = strstr(cmd, ",");    //CPMS,ME,ME,ME
                    if(ptr != NULL)
				    {
				    	ptr++;
                        memset(mem, 0, sizeof(mem));
				        memcpy(mem, ptr, strlen(ptr));
						printf("cpms:%s\n", mem);
				    }
                    printf("cpms 0\n");

                    memset(resp, 0, sizeof(resp));
                    err = mbtk_sms_cpms_set(mem, resp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("cpms set success. resp:%s\n", resp);
                    }
                }
            }else if(!strncasecmp(cmd, "cmgs", 4)){ //   AT+CMGS="10086", CMGS TEST
                char cmgs[1024] = {0};
                char resp[50] = {0};
                if(!strcasecmp(cmd, "cmgs")) { // Get
                    int mode;
                //    err = mbtk_sms_cmgs_get(info_handle, &mode);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE : %d\n", mode);
                    }
                } else { // Set

							/*
							*AT+CMGS="10086", CMGS TEST	                  // Send a SMS
							> CMGS TEST
							+CMGS: 17
							OK
							*/

                    char *ptr = strstr(cmd, "cmgs,");      //CMGS="10086",hf
                    if(ptr != NULL)
				    {
				    	ptr = strstr(cmd, ",");
                        ptr++;
                        memset(cmgs, 0, sizeof(cmgs));
				        memcpy(cmgs, ptr, strlen(ptr));
						printf("1cmgs:%s, strlen(cmgs):%d\n", cmgs, strlen(cmgs));
				    }

					memset(resp, 0, sizeof(resp));
                    err = mbtk_sms_cmgs_set(cmgs, resp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("cmgs set success . resp:%s\n", resp);
                    }
                }
            }else if(!strncasecmp(cmd, "cmss", 4)){ // +CMSS=<index>[,<da>[,<toda>]]
                char cmss[20] = {0};
                char resp[20] = {0};
                if(!strcasecmp(cmd, "cmgs")) { // Get
                    printf("cmss : OK\n");

                } else {
                    char *ptr = strstr(cmd, "cmss,");    //CMSS=<index>
                    if(ptr != NULL)
				    {
                        ptr = strstr(cmd, ",");
				    	ptr++;
                        memset(cmss, 0, sizeof(cmss));
				        memcpy(cmss, ptr, strlen(ptr));
						printf("1cmss:%s\n", cmss);
				    }


                    err = mbtk_sms_cmss_set(cmss, resp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("cmss set success. resp:%s\n", resp);
                    }
                }
            }
            else if(!strncasecmp(cmd, "cmgr", 4)){ // +CMGR=<index
                int index = 0;
                char resp[1024] = {0};
                if(!strcasecmp(cmd, "cmgr")) { // Get
                    printf("cmgr : OK\n");

                } else {
                    char *ptr = strstr(cmd, "cmgr,");    //+CMGR <index>
                    if(ptr != NULL)
				    {
                        ptr = strstr(cmd, ",");
                        ptr++;
				    	index = atoi(ptr);
				    }
                    printf("1index:%d\n", index);

                    memset(resp, 0, sizeof(resp));
                    err = mbtk_sms_cmgr_set(index, resp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("cmgr set success. rep:%s\n", resp);
                    }
                }
            }
            else if(!strncasecmp(cmd, "cmgw", 4)){ // +CMGW=<oa/da>[,<tooa/toda>[,<stat>]]<CR>
                                                    //+CMGW=<length>[,<stat>]<CR>PDU is given<ctrl-Z/ESC>
                char cmgw[128] = {0};
                char resp[50] = {0};
                if(!strcasecmp(cmd, "cmgw")) { // Get
                    printf("cmgw : OK\n");

                } else {
                    char *ptr = strstr(cmd, "cmgw,");    //+CMGW, <oa/da>, data
                    if(ptr != NULL)
				    {
                        ptr = strstr(cmd, ",");
				    	ptr++;
                        memset(cmgw, 0, sizeof(cmgw));
				        memcpy(cmgw, ptr, strlen(ptr));
						printf("cmgw:%s\n", cmgw);
				    }

                    memset(resp, 0, sizeof(resp));
                    err = mbtk_sms_cmgw_set(cmgw, resp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("cmgw set success. resp:%s\n", resp);
                    }
                }
            }
            else if(!strncasecmp(cmd, "cmgd", 4)){ // +CMGD=<index>[,<delflag>
                                                    //
                char cmgd[128] = {0};
                if(!strcasecmp(cmd, "cmgd")) { // Get
                    printf("cmgd : OK\n");

                } else {
                    char *ptr = strstr(cmd, ",");    //+CMGD=<index>[,<delflag>
                    if(ptr != NULL)
				    {
				    	ptr++;
                        memset(cmgd, 0, sizeof(cmgd));
				        memcpy(cmgd, ptr, strlen(ptr));
						printf("1cmgd:%s\n", cmgd);
				    }


                    err = mbtk_sms_cmgd_set(cmgd);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            }
            else if(!strncasecmp(cmd, "cmgl", 4)){ // AT+CMGL[=<stat>]
                                                    //
                char cmgl[128] = {0};
                char resp[5*1024] ={0};
                if(!strcasecmp(cmd, "cmgl")) { // Get
                    printf("cmgl : OK\n");

                } else {
                    char *ptr = strstr(cmd, "cmgl,");  //  AT+CMGL[=<stat>]
                    if(ptr != NULL)
				    {
                        ptr = strstr(cmd, ",");
				    	ptr++;
                        memset(cmgl, 0, sizeof(cmgl));
				        memcpy(cmgl, ptr, strlen(ptr));
						printf("0cmgl:%s\n", cmgl);
				    }

					memset(resp, 0, sizeof(resp));
                    err = mbtk_sms_cmgl_set(cmgl, resp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                      //  printf("cmgl set success, reg:%s\n",resp);
                    }
                }
            }
            else if(!strncasecmp(cmd, "csca", 4)){ // AT+CSCA=<number> [,<type>]
                                                    //
                char csca[128] = {0};
                if(!strcasecmp(cmd, "csca")) { // Get
                    err = mbtk_sms_csca_get(csca);
                    if(err) {
                        printf("mbtk_sms_csca_get Error : %d\n", err);
                    } else {
                        printf("mbtk_sms_csca_get success\n");
                    }

                } else {
                    char *ptr = strstr(cmd, ",");  //  AT+CSCA=<number> [,<type>]
                    if(ptr != NULL)
				    {
				    	ptr++;
                        memset(csca, 0, sizeof(csca));
				        memcpy(csca, ptr, strlen(ptr));
						printf("csca:%s\n", csca);
				    }

                    err = mbtk_sms_csca_set(csca);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            }
            else if(!strncasecmp(cmd, "csmp", 4)){ // AT+CSMP=[<fo>[,<vp>[,<pid>[,<dcs>]]]]
                                                    //
                char csmp[128] = {0};
                if(!strcasecmp(cmd, "csmp")) { // Get
                    printf("cmgl : OK\n");

                } else {
                    char *ptr = strstr(cmd, ",");  //  AT+CSMP=[<fo>[,<vp>[,<pid>[,<dcs>]]]]
                    if(ptr != NULL)
				    {
				    	ptr++;
                        memset(csmp, 0, sizeof(csmp));
				        memcpy(csmp, ptr, strlen(ptr));
						printf("csmp:%s\n", csmp);
				    }

                    err = mbtk_sms_csmp_set(csmp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            }
            else if(!strncasecmp(cmd, "cscb", 4)){ // AT+CSCB=<[<mode>[,<mids>[,<dcss>]]]>
                                                    //
                char cscb[128] = {0};
                if(!strcasecmp(cmd, "cscb")) { // Get
                    printf("cmgl : OK\n");

                } else {
                    char *ptr = strstr(cmd, ",");  //  AT+CSCB=<[<mode>[,<mids>[,<dcss>]]]>
                    if(ptr != NULL)
				    {
				    	ptr++;
                        memset(cscb, 0, sizeof(cscb));
				        memcpy(cscb, ptr, strlen(ptr));
						printf("cscb:%s\n", cscb);
				    }

                    err = mbtk_sms_cscb_set(cscb);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            }
#if 0
            else if(!strncasecmp(cmd, "shutdown", 8)){
                if(!strcasecmp(cmd, "shutdown 0")) {
                    err = mbtk_system_reboot(0);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else if(!strcasecmp(cmd, "shutdown 1")) {
                    err = mbtk_system_reboot(1);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else if(!strcasecmp(cmd, "shutdown 2")) {
                    err = mbtk_system_reboot(2);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else {
                    printf("Error.");
                }
            } else if(!strncasecmp(cmd, "power_sim", 9)){
                if(!strcasecmp(cmd, "power_sim 0")) {
                    err = mbtk_sim_power_set(0);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else if(!strcasecmp(cmd, "power_sim 1")) {
                    err = mbtk_sim_power_set(1);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else {
                    printf("Error.");
                }
            } else if(!strncasecmp(cmd, "time", 4)){
                if(!strcasecmp(cmd, "time 0")) {
                    err = mbtk_time_set(info_handle, 0, NULL);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else if(!strcasecmp(cmd, "time 1")) {
                    err = mbtk_time_set(info_handle, 1, NULL);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else if(!strncasecmp(cmd, "time 2 ", 7)) {
                    err = mbtk_time_set(info_handle, 2, cmd + 7);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Success.\n");
                    }
                } else { // Get time type.
                    int time_type;
                    err = mbtk_time_get(info_handle, &time_type);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Time type:%d.\n", time_type);
                    }
                }
            }
#endif
            else if(!strcasecmp(cmd, "h") || !strcasecmp(cmd, "help")) {
                help();
            } else if(!strcasecmp(cmd, "q")) {
                mbtk_ril_deinit();
                break;
            } else {
                printf("\n");
            }
        }
    }

    thread_exit_with_wait();

exit:
    mbtk_ril_deinit();

    LOG("Client exec complete.");

    return 0;
}
