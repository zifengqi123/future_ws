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

#include "mbtk_info_api.h"

#define DSC_to_msg(DSC) (DSC == 0 ? "Bit7" : (DSC == 1 ? "Bit8" : "UCS2"))

//#include "lynq_net_api.h"

static mbtk_info_handle_t* info_handle = NULL;

void net_state_change_cb(const void* data, int data_len)
{
    if(data && data_len > 0) {
        const uint8 *ptr = (const uint8*)data;
        if(*ptr == 0) { // CS
            printf("CS:act=%d, mode=%d\n", *(ptr + 1), *(ptr + 2));
        } else { // PS
            printf("PS[%s]:act=%d, mode=%d\n", *(ptr + 3) == 0 ? "GSM/WCDMA" : "LTE", *(ptr + 1), *(ptr + 2));
        }
    } else
        LOGD("NET_CB : NULL");
}

void call_state_change_cb(const void* data, int data_len)
{
    mbtk_call_info_t *reg = (mbtk_call_info_t *)data;
    switch (reg->call_wait)
    {
        case MBTK_CLCC:
            printf("\r\nRING : %d, %d, %d, %d, %d, %s, %d\r\n", reg->dir1, reg->dir, reg->state, reg->mode, reg->mpty, reg->phone_number, reg->type);
            break;
        case MBTK_DISCONNECTED:
            printf("\r\nRING : call dis connected!\r\n");
            break;
        case MBTK_CPAS:
            printf("\r\nCALL : Call state = %d\r\n", reg->pas);
            /*
                MBTK_CALL_RADY,                 //MT allows commands from TA/TE
                MBTK_CALL_UNAVAILABLE,          //MT does not allow commands from TA/TE
                MBTK_CALL_UNKNOWN,              //MT is not guaranteed to respond to instructions
                MBTK_CALL_RINGING,              //MT is ready for commands from TA/TE, but the ringer is active
                MBTK_CALL_PROGRESS,             //MT is ready for commands from TA/TE, but a call is in progress
                MBTK_CALL_ASLEEP,               //MT is unable to process commands from TA/TE because it is in a low functionality state
                MBTK_CALL_ACTIVE,
            */
           switch (reg->pas)
           {
                case MBTK_CALL_RADY:
                    printf("CALL: call READY\r\n");
                    break;
                case MBTK_CALL_UNAVAILABLE:
                    printf("CALL: call unavaliable\r\n");
                    break;
                case MBTK_CALL_UNKNOWN:
                    printf("CALL: call unknown\r\n");
                    break;
                case MBTK_CALL_RINGING:
                    printf("CALL: call ringing\r\n");
                    break;
                case MBTK_CALL_PROGRESS:
                    printf("CALL: call progress\r\n");
                    break;
                case MBTK_CALL_ASLEEP:
                    printf("CALL: call asleep\r\n");
                    break;
                case MBTK_CALL_ACTIVE:
                    printf("CALL: call active\r\n");
                    break;
                default:
                    printf("\r\n");
                    break;
           }
            break;
        default:
            printf("\r\nRING : None call_wait = %d\r\n", reg->call_wait);
            break;
    }
    /*
    if(reg->call_wait == )         //CLCC
    {
        printf("\r\nRING : %d, %d, %d, %d, %d, %s, %d\r\n", reg->dir1, reg->dir, reg->state, reg->mode, reg->mpty, reg->phone_number, reg->type);
    }
    else if(reg->call_wait == 2)    //Disconnected
    {
        printf("\r\nRING : call dis connected!\r\n");
    }
    else
    {
        printf("\r\nRING : None\r\n");
    }
    */
}

void sms_state_change_cb(const void* data, int data_len)
{
    LOGV("sms_state_change_cb()----------start\n");
	uint8 *ptr = (uint8*)data;
		printf("3sms_state_change_cb() : %s\n", ptr);

	struct SMS_Struct s = PDUDecoding(ptr);
	printf("服务中心地址: %s\n", s.SCA);
    printf("发送方地址: %s\n", s.OA);
    printf("服务中心时间戳: %s\n", s.SCTS);
    printf("消息内容: %s\n", s.UD);
    printf("数据编码方案： %s\n", DSC_to_msg(s.DCS));

}

void radio_state_change_cb(const void* data, int data_len)
{
    uint8 *ptr = (uint8*)data;
    printf("Radio state : %d\n", *ptr);
}

void sim_state_change_cb(const void* data, int data_len)
{
    uint8 *ptr = (uint8*)data;
    printf("SIM state : %d\n", *ptr);
}

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static void test2(int is_ipv6, char *ip)
{
    char ipaddr[20] = {0};
	if(is_ipv6) {
        struct in6_addr sin_addr;
        if(inet_pton(AF_INET6, ip, &sin_addr) < 0) {
            printf("inet_pton() fail.\n");
        } else {
		    log_hex("IPv6", &sin_addr, sizeof(struct in6_addr));
        }
	} else {
	    struct in_addr sin_addr;
        if(inet_pton(AF_INET, ip, &sin_addr) < 0) {
            printf("inet_pton() fail.\n");
        } else {
		    log_hex("IPv4", &sin_addr, sizeof(struct in_addr));
        }
	}
}


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
    printf("radio 0: Close radio.\n");
    printf("radio 1: Open radio.\n");
    printf("sim: Get sim state.\n");
    printf("avail_net: Get available network.\n");
    printf("imsi: Get IMSI.\n");
    printf("iccid: Get ICCID.\n");
    printf("pn: Get Phone Number.\n");
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
    printf("temp <0,1>: Get SOC/RF temperature.\n");
    printf("time <0,1,2> YYYY-MM-DD HH:MM:SS : Set system time as CELL/NTP/User.\n");
    printf("apn : Get current apns.\n");
    printf("apn <cid> <1/2/3/4> <apn> : Set apn.\n");
    printf("data_call <0/1/2> <cid> <timeout>: Stop/Start/State data call.\n");
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

static void sig_process(int sig)
{
    LOGI("I got signal %d\n", sig);
    switch(sig)
    {
        case SIGINT: // Ctrl + C
        {
            LOGI("Exit by SIGINT.\n");
            mbtk_info_handle_free(&info_handle);
            exit(0);
        }
        case SIGQUIT: // Ctrl + \ (类似 SIGINT ，但要产生core文件)
        {
            LOGI("Exit by SIGQUIT.\n");
            mbtk_info_handle_free(&info_handle);
            exit(0);
        }
        case SIGTERM:// 默认kill   (同 SIGKILL ，但 SIGKILL 不可捕获)
        {
            LOGI("Exit by SIGTERM.\n");
            mbtk_info_handle_free(&info_handle);
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

    mbtk_log_init("radio","INFO_CLI");

    //test2(0, "192.168.1.198");
    //test2(1, "2409:8162:140:cd3c:1:2:1494:72ba");
    //test2(1, "254.128.0.0.0.0.0.0.0.1.0.2.144.5.212.239");
    //test2(1, "2400:3200::1");

    info_handle = mbtk_info_handle_get();
    if(!info_handle)
    {
        return -1;
    }

    mbtk_net_state_change_cb_reg(info_handle, net_state_change_cb);
    mbtk_call_state_change_cb_reg(info_handle, call_state_change_cb);
    mbtk_sms_state_change_cb_reg(info_handle, sms_state_change_cb);
    mbtk_radio_state_change_cb_reg(info_handle, radio_state_change_cb);
    mbtk_sim_state_change_cb_reg(info_handle, sim_state_change_cb);

    printf(">>>>>>>>>>>>>>>>>>>>>>>>Enter cmd:\n");
    char cmd[100];
    while(1)
    {
        memset(cmd, 0, 100);
        int err;
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
                err = mbtk_version_get(info_handle, version);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Version : %s\n", version);
                }
            } else if(!strncasecmp(cmd, "imei", 4)){
                char imei[50] = {0};
                err = mbtk_imei_get(info_handle, imei);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("IMEI : %s\n", imei);
                }
            } else if(!strncasecmp(cmd, "sn", 2)){
                char sn[50] = {0};
                err = mbtk_sn_get(info_handle, sn);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("SN : %s\n", sn);
                }
            } else if(!strncasecmp(cmd, "meid", 4)){
                char meid[50] = {0};
                err = mbtk_meid_get(info_handle, meid);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("MEID : %s\n", meid);
                }
            } else if(!strncasecmp(cmd, "volte", 5)){ // "volte" or "volte 0" or "volte 1"
                int volte;
                if(!strcasecmp(cmd, "volte")) { // Get
                    err = mbtk_volte_state_get(info_handle, &volte);
                    if(err) {
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
                    err = mbtk_volte_state_set(info_handle, volte);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "radio", 5)){ // "radio" or "radio 0" or "radio 1"
                int radio;
                if(!strcasecmp(cmd, "radio")) { // Get
                    err = mbtk_radio_state_get(info_handle, &radio);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Radio : %d\n", radio);
                    }
                } else { // Set
                    if(!strcasecmp(cmd, "radio 1")) { // Open radio
                        radio = 1;
                    } else { // Close radio
                        radio = 0;
                    }
                    err = mbtk_radio_state_set(info_handle, radio);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Radio set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "sim", 3)){
                mbtk_sim_state_enum sim;
                err = mbtk_sim_state_get(info_handle, &sim);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Sim : %d\n", sim);
                }
            } else if(!strncasecmp(cmd, "imsi", 4)){
                char imsi[50] = {0};
                err = mbtk_imsi_get(info_handle, imsi);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("IMSI : %s\n", imsi);
                }
            } else if(!strncasecmp(cmd, "iccid", 5)){
                char iccid[50] = {0};
                err = mbtk_iccid_get(info_handle, iccid);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("ICCID : %s\n", iccid);
                }
            } else if(!strncasecmp(cmd, "pn", 2)){
                char phone_number[50] = {0};
                err = mbtk_phone_number_get(info_handle, phone_number);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Phone Number : %s\n", phone_number);
                }
            } else if(!strncasecmp(cmd, "avail_net", 9)){
                list_node_t* net_list = NULL;
                err = mbtk_available_net_get(info_handle, &net_list);
                if(err || net_list == NULL) {
                    printf("Error : %d\n", err);
                } else {
                    mbtk_net_info_t* net = NULL;
                    list_first(net_list);
                    while ((net = (mbtk_net_info_t*) list_next(net_list)))
                    {
                        printf("Net : %d, %d, %d, %d\n", net->net_sel_mode, net->net_type, net->net_state, net->plmn);
                    }
                }
                list_free(net_list);
            } else if(!strncasecmp(cmd, "cell", 4)){
                char *ptr = strstr(cmd, ",");	 //CPMS,ME,ME,ME
				if(ptr == NULL)
				{
                    printf("ptr == NULL\n");
                    list_node_t* cell_list = NULL;
                    int type;
                    err = mbtk_cell_get(info_handle, &type, &cell_list);
                    if(err || cell_list == NULL) {
                        printf("Error : %d\n", err);
                    } else {
                        list_first(cell_list);
                        mbtk_cell_info_t* cell = (mbtk_cell_info_t*) list_next(cell_list);
                        if(cell) { // Current server cell.
                            switch(type)
                            {
                                case 0:
                                    printf("GSM : lac=%d, ci=%d, arfcn=%d, bsic=%d\n", cell->value1, cell->value2, cell->value3, cell->value4);
                                    break;
                                case 1:
                                    printf("UMTS : lac=%d, ci=%d, arfcn=%d\n", cell->value1, cell->value2, cell->value3);
                                    break;
                                case 2:
                                    printf("LTE : tac=%d, PCI=%d, dlEuarfcn=%d, ulEuarfcn=%d, band=%d\n", cell->value1, cell->value2, cell->value3, cell->value4, cell->value5);
                                    break;
                                default:
                                    break;
                            }
                        }
                        while ((cell = (mbtk_cell_info_t*) list_next(cell_list)))
                        {
                            switch(type)
                            {
                                case 0:
                                    printf("CELL : %d, %d, %d, %d, %d", cell->value1, cell->value2, cell->value3, cell->value4, cell->value5);
                                    break;
                                case 1:
                                    printf("CELL : lac=%d, ci=%d, arfcn=%d\n", cell->value1, cell->value2, cell->value3);
                                    break;
                                case 2:
                                    printf("CELL : phyCellId=%d, euArfcn=%d, rsrp=%d, rsrq=%d\n", cell->value1, cell->value2, cell->value3, cell->value4);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    list_free(cell_list);
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
					err = mbtk_cell_set(info_handle, mem, resp);
					if(err) {
						printf("Error : %d\n", err);
					} else {
						printf("cell set . resp:%s\n", resp);
					}

                }
            } else if(!strncasecmp(cmd, "sel_mode", 8)){ // "sel_mode" or "sel_mode 1 7 46000"
                mbtk_net_info_t net;
                memset(&net, 0, sizeof(mbtk_net_info_t));
                if(!strcasecmp(cmd, "sel_mode")) { // Get
                    err = mbtk_net_sel_mode_get(info_handle, &net);
                    if(err) {
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

                    err = mbtk_net_sel_mode_set(info_handle, &net);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Net select mode set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "band", 4)){ // "band" or "band support" or "band 0 79 147 482 524503"
                mbtk_band_info_t band;
                memset(&band, 0x0, sizeof(mbtk_band_info_t));
                if(!strcasecmp(cmd, "band")) { // Get
                    err = mbtk_current_band_get(info_handle, &band);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Band : %d, %d, %d, %d, %d\n", band.net_pref, band.gsm_band, band.umts_band, band.tdlte_band, band.fddlte_band);
                    }
                } else if(!strcasecmp(cmd, "band support")) { // Get
                    err = mbtk_support_band_get(info_handle, &band);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Band : %d, %d, %d, %d, %d\n", band.net_pref, band.gsm_band, band.umts_band, band.tdlte_band, band.fddlte_band);
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

                    err = mbtk_current_band_set(info_handle, &band);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Band set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "signal", 6)){
                mbtk_signal_info_t signal;
                err = mbtk_net_signal_get(info_handle, &signal);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Signal : %d, %d, %d, %d, %d, %d, %d, %d\n", signal.type, signal.rssi, signal.rxlev, signal.ber,
                        signal.rscp, signal.ecno, signal.rsrq, signal.rsrp);
                }
            } else if(!strncasecmp(cmd, "reg", 3)){
                mbtk_net_reg_info_t reg;
                err = mbtk_net_reg_get(info_handle, &reg);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("REG : call_state=%d, data_state=%d, ims_state=%d, net_type=%d, %04x, %08x\n", reg.call_state, reg.data_state, reg.ims_state, reg.type, reg.lac, reg.ci);
                }
            } else if(!strncasecmp(cmd, "call", 4)){
                char phone_number[12];
                char *ptr = strstr(cmd, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                memset(phone_number,0,strlen(phone_number));
                memcpy(phone_number,ptr,strlen(ptr));
                printf("phone number is: %s\n",phone_number);
                err = mbtk_call_start(info_handle, phone_number);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Call success.\n");
                }
            } else if(!strncasecmp(cmd, "answer", 6)){
                err = mbtk_call_answer(info_handle);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("Call success.\n");
                }
            } else if(!strncasecmp(cmd, "hangup", 6)){
                int phone_id;
                if(!strcasecmp(cmd, "hangup")) { // hang up all
                    err = mbtk_call_hang(info_handle);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Call hang up all.\n");
                    }
                } else if(!strcasecmp(cmd, "hangup 0")) {
                    err = mbtk_waiting_or_background_call_hang(info_handle);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("Call hang up waiting or background.\n");
                    }
                } else if(!strcasecmp(cmd, "hangup 3")) {
                    err = mbtk_foreground_resume_background_call_hang(info_handle);
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
                    err = mbtk_a_call_hang(info_handle, phone_id);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("A Call hang up.\n");
                    }
                }
            } else if(!strncasecmp(cmd, "waitin", 6)){
                mbtk_call_info_t reg;
                err = mbtk_call_reg_get(info_handle, &reg);
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
                    err = mbtk_mute_state_get(info_handle, &mute);
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
                    err = mbtk_mute_state_set(info_handle, mute);
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
                err = mbtk_dtmf_send(info_handle, &reg);
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("DTMF success.\n");
                }
            } else if(!strncasecmp(cmd, "shutdown", 8)){
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
            } else if(!strncasecmp(cmd, "temp", 4)){
                mbtk_thermal_info_t temp;
                memset(&temp, 0, sizeof(mbtk_thermal_info_t));
                if(!strcasecmp(cmd, "temp 0")) {
                    err = mbtk_temp_get(info_handle, 0, &temp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("SOC : %d\n", temp.ther);
                    }
                } else if(!strcasecmp(cmd, "temp 1")) {
                    err = mbtk_temp_get(info_handle, 1, &temp);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("RF : %d\n", temp.ther);
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
            } else if(!strncasecmp(cmd, "apn", 3)){
                if(!strcasecmp(cmd, "apn")) { // Get apn
                #define APN_MAX 10
                    mbtk_apn_info_t apns[APN_MAX];
                    int apn_num = APN_MAX;
                    err = mbtk_apn_get(info_handle, &apn_num, apns);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN Num:%d\n", apn_num);
                        int i = 0;
                        while(i < apn_num) {
                            printf("APN : %d, %s, %s\n", apns[i].cid, apn2str(apns[i].ip_type), apns[i].apn);
                            i++;
                        }
                    }
                } else { // apn <cid> <0/1/2/3> <apn>
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
                    mbtk_ip_type_enum ip_type = (mbtk_ip_type_enum)atoi(ptr);

                    ptr = strstr(ptr, " ");
                    if(ptr == NULL)
                        continue;
                    while(*ptr != '\0' && *ptr == ' ')
                        ptr++;
                    char *apn  = ptr;

                    err = mbtk_apn_set(info_handle, cid, ip_type, apn, NULL, NULL, NULL);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("APN set success\n");
                    }
                }
            } else if(!strncasecmp(cmd, "data_call", 9)){ // data_call <0/1/2> <cid> <timeout>
                // data_call <0/1/2> <cid> <timeout>
                char *ptr = strstr(cmd, " ");
                if(ptr == NULL)
                    continue;
                while(*ptr != '\0' && *ptr == ' ')
                    ptr++;
                mbtk_data_call_type_enum type = (mbtk_data_call_type_enum)atoi(ptr);

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

                if(type == MBTK_DATA_CALL_START) {
                    err = mbtk_data_call_start(info_handle, cid, 0, FALSE, timeout);
                } else if(type == MBTK_DATA_CALL_STOP) {
                    err = mbtk_data_call_stop(info_handle, cid, timeout);
                } else {
                    mbtk_ipv4_info_t ipv4;
                    mbtk_ipv6_info_t ipv6;
                    err = mbtk_data_call_state_get(info_handle, cid, &ipv4, &ipv6);
                    if(!err) {
                        if(ipv4.valid) {
                            // log_hex("IPv4", &ipv4, sizeof(mbtk_ipv4_info_t));
                            char ip_tmp[20];

                            memset(ip_tmp, 0, 20);
                            if(inet_ntop(AF_INET, &(ipv4.IPAddr), ip_tmp, 20) == NULL) {
                                printf("IP error.\n");
                            } else {
                                printf("IP : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 20);
                            if(inet_ntop(AF_INET, &(ipv4.PrimaryDNS), ip_tmp, 20) == NULL) {
                                printf("PrimaryDNS error.\n");
                            } else {
                                printf("PrimaryDNS : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 20);
                            if(inet_ntop(AF_INET, &(ipv4.SecondaryDNS), ip_tmp, 20) == NULL) {
                                printf("SecondaryDNS error.\n");
                            } else {
                                printf("SecondaryDNS : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 20);
                            if(inet_ntop(AF_INET, &(ipv4.GateWay), ip_tmp, 20) == NULL) {
                                printf("GateWay error.\n");
                            } else {
                                printf("GateWay : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 20);
                            if(inet_ntop(AF_INET, &(ipv4.NetMask), ip_tmp, 20) == NULL) {
                                printf("NetMask error.\n");
                            } else {
                                printf("NetMask : %s\n", ip_tmp);
                            }
                        }

                        if(ipv6.valid) {
                            // log_hex("IPv6", &ipv6, sizeof(mbtk_ipv6_info_t));
                            char ip_tmp[50];

                            memset(ip_tmp, 0, 50);
                            if(ipv6_2_str(&(ipv6.IPV6Addr), ip_tmp)) {
                                printf("IP error.\n");
                            } else {
                                printf("IP : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 50);
                            if(ipv6_2_str(&(ipv6.PrimaryDNS), ip_tmp)) {
                                printf("PrimaryDNS error.\n");
                            } else {
                                printf("PrimaryDNS : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 50);
                            if(ipv6_2_str(&(ipv6.SecondaryDNS), ip_tmp)) {
                                printf("SecondaryDNS error.\n");
                            } else {
                                printf("SecondaryDNS : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 50);
                            if(ipv6_2_str(&(ipv6.GateWay), ip_tmp)) {
                                printf("GateWay error.\n");
                            } else {
                                printf("GateWay : %s\n", ip_tmp);
                            }

                            memset(ip_tmp, 0, 50);
                            if(ipv6_2_str(&(ipv6.NetMask), ip_tmp)) {
                                printf("NetMask error.\n");
                            } else {
                                printf("NetMask : %s\n", ip_tmp);
                            }
                        }
                    }
                }
                if(err) {
                    printf("Error : %d\n", err);
                } else {
                    printf("DATA_CALL success\n");
                }
            }else if(!strncasecmp(cmd, "cmgf", 4)){ // set mode 0: pud, 1:text
                int mode;
                if(!strcasecmp(cmd, "cmgf")) { // Get
                    err = mbtk_sms_cmgf_get(info_handle, &mode);
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
                   err = mbtk_sms_cmgf_set(info_handle, mode);
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
                    err = mbtk_sms_cpms_get(info_handle, &mem);
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
                    err = mbtk_sms_cpms_set(info_handle, mem, resp);
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
                    err = mbtk_sms_cmgs_set(info_handle, cmgs, resp);
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


                    err = mbtk_sms_cmss_set(info_handle, cmss, resp);
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
                    err = mbtk_sms_cmgr_set(info_handle, index, resp);
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
                    err = mbtk_sms_cmgw_set(info_handle, cmgw, resp);
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


                    err = mbtk_sms_cmgd_set(info_handle, cmgd);
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
                    err = mbtk_sms_cmgl_set(info_handle, cmgl, resp);
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
                    err = mbtk_sms_csca_get(info_handle, csca);
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

                    err = mbtk_sms_csca_set(info_handle, csca);
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

                    err = mbtk_sms_csmp_set(info_handle, csmp);
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

                    err = mbtk_sms_cscb_set(info_handle, cscb);
                    if(err) {
                        printf("Error : %d\n", err);
                    } else {
                        printf("VoLTE set success\n");
                    }
                }
            }
            else if(!strncasecmp(cmd, "ims_en", 6)){ // ims_en 0/1
                char enable[10] = {0};
                if(1 == sscanf(cmd, "ims_en %s", enable)) {
                    err = mbtk_net_ims_set(info_handle, atoi(enable) ? 1 : 0);
                    if(err) {
                        printf("Set IMS fail : %d\n", err);
                    } else {
                        printf("Set IMS success\n");
                    }
                } else {
                    printf("ARG error\n");
                }
            }
            else if(!strncasecmp(cmd, "ims_state", 9)){ // ims_state
                int enable = 0;
                err = mbtk_net_ims_get(info_handle, &enable);
                if(err) {
                    printf("Get IMS state fail : %d\n", err);
                } else {
                    printf("IMS state : %d\n", enable);
                }
            }
            else if(!strncasecmp(cmd, "ims_reg", 7)){ // ims_reg
                int reg = 0;
                err = mbtk_net_ims_reg_state_get(info_handle, &reg);
                if(err) {
                    printf("Get IMS register state fail : %d\n", err);
                } else {
                    printf("IMS register state : %d\n", reg);
                }
            }
            else if(!strcasecmp(cmd, "h") || !strcasecmp(cmd, "help")) {
                help();
            } else if(!strcasecmp(cmd, "q")) {
                mbtk_info_handle_free(&info_handle);
                break;
            } else {
                printf("\n");
            }
        }
    }

    LOG("Client exec complete.");
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
