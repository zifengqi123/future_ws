#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>

#include"lynq-qser-sim-demo.h"
#include "mbtk_log.h"


#define BUF_SIZE 32
#define BUF_PIN 8
#define VER_SIZE 128

typedef uint32_t sim_client_handle_type;

//sim_client_handle_type  ph_sim = 2023;
sim_client_handle_type  h_sim = 2023;
int flag_init = 0;

int (*qser_sim_client_init)(sim_client_handle_type  *ph_sim);
int (*qser_sim_client_deinit)(sim_client_handle_type h_sim);
int (*qser_sim_getimsi)(
    sim_client_handle_type          h_sim,
    QSER_SIM_APP_ID_INFO_T            *pt_info,   ///< [IN] The SIM identifier info.
    char                            *imsi,      ///< [OUT] IMSI buffer
    size_t                          imsiLen     ///< [IN] IMSI buffer length
);

int (*qser_sim_geticcid)
(
    sim_client_handle_type          h_sim,
    QSER_SIM_SLOT_ID_TYPE_T     simId,     ///< [IN] The SIM identifier.
    char                            *iccid,    ///< [OUT] ICCID
    size_t                          iccidLen   ///< [IN] ICCID buffer length
);

int (*qser_sim_getphonenumber)
(
    sim_client_handle_type          h_sim,
    QSER_SIM_APP_ID_INFO_T            *pt_info,   ///< [IN] The SIM identifier.
    char                            *phone_num, ///< [OUT] phone number
    size_t                          phoneLen    ///< [IN] phone number buffer length
);
int (*qser_sim_verifypin)(sim_client_handle_type h_sim, QSER_SIM_VERIFY_PIN_INFO_T *pt_info);
int (*qser_sim_changepin)(sim_client_handle_type h_sim, QSER_SIM_CHANGE_PIN_INFO_T *pt_info);
int (*qser_sim_unblockpin)(sim_client_handle_type h_sim, QSER_SIM_UNBLOCK_PIN_INFO_T *pt_info);
int (*qser_sim_enablepin)(sim_client_handle_type h_sim, QSER_SIM_ENABLE_PIN_INFO_T *pt_info);
int (*qser_sim_disablepin)(sim_client_handle_type h_sim, QSER_SIM_DISABLE_PIN_INFO_T *pt_info);
int (*qser_sim_getcardstatus)(sim_client_handle_type h_sim, QSER_SIM_SLOT_ID_TYPE_T simId, QSER_SIM_CARD_STATUS_INFO_T *pt_info);
int (*qser_sim_getimei)(sim_client_handle_type h_sim, char *imei);
int (*qser_get_imei_and_sv)(sim_client_handle_type h_sim,char *imei, char*sv);
int (*qser_reset_modem)(sim_client_handle_type h_sim);
int (*qser_get_version)(sim_client_handle_type h_sim, char *buf);
int (*qser_reset_sim)(sim_client_handle_type h_sim);
int (*qser_sim_addrxmsghandler)(QSER_SIM_RxMsgHandlerFunc_t handlerPtr);


typedef struct
{
    int  cmdIdx;
    const char *funcName;
} st_api_test_case;

//for server test
st_api_test_case at_api_testcases[] =
{
    {0,   "qser_sim_init"},
    {1,   "qser_get_imsi"},
    {2,   "qser_get_iccid"},
    {3,   "qser_get_phonenumber"},
    {4,   "qser_verify_pin"},
    {5,   "qser_change_pin"},
    {6,   "qser_unlock_pin"},
    {7,   "qser_enable_pin"},
    {8,   "qser_disable_pin"},
    {9,   "qser_get_sim_status"},
    {10,   "qser_get_imei"},
    {11,   "qser_get_imei_and_sv"},
    {12,   "qser_reset_modem"},
    {13,   "qser_get_version"},
    {14,   "qser_reset_sim"},
    {15,   "qser_sim_addrxmsghandler"},
    {16,   "qser_deinit_sim"},
    {-1,    NULL}
};

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

void qser_sim_handler(QSER_SIM_MsgRef msgRef) {
    printf("[%s-%d] sim, state = 0x%x========\n", __FUNCTION__, __LINE__, msgRef->e_card_state);
}

int main(int argc, char const *argv[])
{
    int cmdIdx = 0;
    int res    = 0;

    mbtk_log_init("radio", "MBTK");

    const char *lynq_libpath_sim = "/lib/liblynq-qser-sim.so";
    void *dlHandle_sim = dlopen(lynq_libpath_sim, RTLD_NOW);
    if (dlHandle_sim == NULL)
    {
        printf("dlopen dlHandle_sim failed: %s\n", dlerror());
        exit(EXIT_FAILURE);
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
            //"qser_sim_init"
            case 0:
            {
                if(flag_init == 1)
                {
                   printf("init is already\n");
                   break;
                }
                else{
                    //int token;
                    //printf("input token\n");
                    //scanf("%d", &token);
                    qser_sim_client_init = (int(*)(sim_client_handle_type  *h_sim))dlsym(dlHandle_sim,"qser_sim_client_init");
                    if(NULL != qser_sim_client_init)
                    {
                        res = qser_sim_client_init(&h_sim);
                        if(res == 0)
                        {
                            printf("Run qser_sim_client_init\n");
                            flag_init = 1;
                        }else{
                            printf("qser_sim_client_init error\n");
                        }
                    }else{
                        printf("qser_sim_client_init dlsym error\n");
                    }
                    break;
                }
            }

            //"qser_sim_getimsi"
            case 1:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char imsi[BUF_SIZE] = {0};
                    QSER_SIM_APP_ID_INFO_T pt_info;
                    qser_sim_getimsi = (int(*)(sim_client_handle_type h_sim, QSER_SIM_APP_ID_INFO_T *pt_info, char *imsi, size_t imsiLen))dlsym(dlHandle_sim,"qser_sim_getimsi");
                    if(NULL != qser_sim_getimsi)
                    {
                        res = qser_sim_getimsi(h_sim, &pt_info, imsi, BUF_SIZE);
                        if(res == 0)
                        {
                            printf("imsi is %s!!!\n",imsi);
                        }else{
                            printf("get imsi error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_getimsi dlsym error\n");
                    }
                }
                break;
            }

            //"qser_get_iccid"
            case 2:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char iccid[BUF_SIZE] = {0};
                     qser_sim_geticcid = (int(*)(sim_client_handle_type h_sim, QSER_SIM_SLOT_ID_TYPE_T simId, char *iccid, size_t iccidLen))dlsym(dlHandle_sim,"qser_sim_geticcid");
                    if(NULL != qser_sim_geticcid)
                    {
                        res = qser_sim_geticcid(h_sim, QSER_SIM_SLOT_ID_1, iccid, BUF_SIZE);
                        if(res == 0)
                        {
                            printf("get iccid success!!! iccid is %s\n",iccid);
                        }else{
                            printf("get iccid error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_geticcid dlsym error\n");
                    }
                }
                break;
            }
            //qser_get_phonenumber
            case 3:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char phonenumber[BUF_SIZE] = "";
                    QSER_SIM_APP_ID_INFO_T pt_info;
                    qser_sim_getphonenumber = (int(*)(sim_client_handle_type h_sim, QSER_SIM_APP_ID_INFO_T *pt_info, char *phone_num, size_t phoneLen))dlsym(dlHandle_sim,"qser_sim_getphonenumber");
                    if(NULL != qser_sim_getphonenumber)
                    {
                        res = qser_sim_getphonenumber(h_sim, &pt_info, phonenumber, BUF_SIZE);
                        if(res == 0)
                        {
                            printf("get phonenumber success!!! phonenumber is %s\n",phonenumber);
                        }else{
                            printf("get phonenumber error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_getphonenumber dlsym error\n");
                    }
                }
                break;
           }
            //qser_verify_pin
            case 4:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char pin[BUF_PIN] = {0};
                    QSER_SIM_VERIFY_PIN_INFO_T pt_info;
                    printf("input pin\n");
                    scanf("%s", pin);
                    strncpy(pt_info.pin_value, pin, BUF_PIN);
                    printf("pin_value = %s , pin = %s\n", pt_info.pin_value, pin);

                    qser_sim_verifypin = (int(*)(sim_client_handle_type h_sim, QSER_SIM_VERIFY_PIN_INFO_T *pt_info))dlsym(dlHandle_sim,"qser_sim_verifypin");
                    if(NULL != qser_sim_verifypin)
                    {
                        res = qser_sim_verifypin(h_sim, &pt_info);
                        if(res == 0)
                        {
                            printf("verify pin success!!!\n");
                        }else{
                            printf("verify pin error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_verifypin dlsym error\n");
                    }
                }
                break;
            }
            //qser_change_pin
            case 5:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char old_pin[BUF_PIN] = {0};
                    QSER_SIM_CHANGE_PIN_INFO_T pt_info;
                    printf("input old pin\n");
                    scanf("%s", old_pin);
                    char new_pin[BUF_PIN] = {0};
                    printf("input new pin\n");
                    scanf("%s", new_pin);
                    strncpy(pt_info.old_pin_value, old_pin, BUF_PIN);
                    strncpy(pt_info.new_pin_value, new_pin, BUF_PIN);
                    printf("pt_info.old_pin_value = %s, old_pin = %s\n", pt_info.old_pin_value, old_pin);
                    printf("pt_info.new_pin_value = %s, new_pin = %s\n", pt_info.new_pin_value, new_pin);

                    qser_sim_changepin = (int(*)(sim_client_handle_type h_sim, QSER_SIM_CHANGE_PIN_INFO_T *pt_info))dlsym(dlHandle_sim,"qser_sim_changepin");
                    if(NULL != qser_sim_changepin)
                    {
                        res = qser_sim_changepin(h_sim, &pt_info);
                        if(res == 0)
                        {
                            printf("change pin success!!!\n");
                        }else{
                            printf("change pin error, res = %d\n", res);
                        }
                    }else{
                        printf("lynq_change_pin dlsym error\n");
                    }
                }
                break;
            }
            //qser_unlock_pin
            case 6:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char puk[BUF_PIN] = {0};
                    QSER_SIM_UNBLOCK_PIN_INFO_T pt_info;
                    printf("input  puk\n");
                    scanf("%s", puk);
                    char new_pin[BUF_PIN] = {0};
                    printf("input new pin\n");
                    scanf("%s", new_pin);
                    memset(pt_info.puk_value, 0, sizeof(pt_info.puk_value));
                    memset(pt_info.new_pin_value, 0, sizeof(pt_info.new_pin_value));
                    strncpy(pt_info.puk_value, puk, BUF_PIN);
                    strncpy(pt_info.new_pin_value, new_pin, BUF_PIN);
                    printf("pt_info.puk_value = %s, puk = %s\n", pt_info.puk_value, puk);
                    printf("pt_info.new_pin_value = %s, new_pin = %s\n", pt_info.new_pin_value, new_pin);

                    qser_sim_unblockpin = (int(*)(sim_client_handle_type h_sim, QSER_SIM_UNBLOCK_PIN_INFO_T *pt_info))dlsym(dlHandle_sim,"qser_sim_unblockpin");
                    if(NULL != qser_sim_unblockpin)
                    {
                        res = qser_sim_unblockpin(h_sim, &pt_info);
                        if(res == 0)
                        {
                            printf("unlock pin success!!!\n");
                        }else{
                            printf("unlock pin error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_unblockpin dlsym error\n");
                    }
                }
                break;
            }
            //qser_enable_pin
            case 7:
           {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char pin[BUF_PIN] = {0};
                    QSER_SIM_ENABLE_PIN_INFO_T pt_info;
                    printf("input pin\n");
                    scanf("%s", pin);
                    strncpy(pt_info.pin_value, pin, BUF_PIN);

                    qser_sim_enablepin = (int(*)(sim_client_handle_type h_sim, QSER_SIM_ENABLE_PIN_INFO_T *pt_info))dlsym(dlHandle_sim,"qser_sim_enablepin");
                    if(NULL != qser_sim_enablepin)
                    {
                        res = qser_sim_enablepin(h_sim, &pt_info);
                        if(res == 0)
                        {
                            printf("pin enabled!!!\n");
                        }else{
                            printf("pin enable error, res =%d\n", res);
                        }
                    }else{
                        printf("qser_sim_enablepin dlsym error\n");
                    }
                }
                break;
            }
            //qser_disable_pin
            case 8:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char pin[BUF_PIN] = {0};
                    QSER_SIM_ENABLE_PIN_INFO_T pt_info;
                    printf("input pin\n");
                    scanf("%s", pin);
                    strncpy(pt_info.pin_value, pin, BUF_PIN);

                    qser_sim_disablepin = (int(*)(sim_client_handle_type h_sim, QSER_SIM_DISABLE_PIN_INFO_T *pt_info))dlsym(dlHandle_sim,"qser_sim_disablepin");
                    if(NULL != qser_sim_disablepin)
                    {
                        res = qser_sim_disablepin(h_sim, &pt_info);
                        if(res == 0)
                        {
                            printf("pin disnabled!!!\n");
                        }else{
                            printf("pin disable error,res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_disablepin dlsym error\n");
                    }
                }
                break;
            }
            //qser_get_sim_status
            case 9:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    QSER_SIM_CARD_STATUS_INFO_T pt_info;

                    qser_sim_getcardstatus = (int(*)(sim_client_handle_type h_sim, QSER_SIM_SLOT_ID_TYPE_T simId, QSER_SIM_CARD_STATUS_INFO_T *pt_info))dlsym(dlHandle_sim,"qser_sim_getcardstatus");
                    if(NULL != qser_sim_getcardstatus)
                    {
                        res = qser_sim_getcardstatus(h_sim, QSER_SIM_SLOT_ID_1, &pt_info);
                        if(res == 0)
                        {
                            printf("state is 0x%x !!!\n",pt_info.e_card_state);
                        }else{
                            printf("get imsi error,res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_getcardstatus dlsym error\n");
                    }
                }
                break;
            }
            //qser_deinit_sim
            case 10:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char imei[BUF_SIZE]="";
                    qser_sim_getimei = (int(*)(sim_client_handle_type h_sim, char *imei))dlsym(dlHandle_sim,"qser_sim_getimei");
                    if(NULL != qser_sim_getimei)
                    {
                        res = qser_sim_getimei(h_sim, imei);
                        if(res == 0)
                        {
                            printf("get imei success!!!\n");
                        }else{
                            printf("get imei error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_getimei dlsym error\n");
                    }
                 }
                    //flag_init = 0;
                    break;
            }
            //qser_get_imei_and_sv
            case 11:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char imei[BUF_SIZE]="";
                    char sv[BUF_SIZE]="";
                    qser_get_imei_and_sv = (int(*)(sim_client_handle_type h_sim, char *imei, char *sv))dlsym(dlHandle_sim,"qser_get_imei_and_sv");
                    if(NULL != qser_get_imei_and_sv)
                    {
                        res = qser_get_imei_and_sv(h_sim, imei, sv);
                        if(res == 0)
                        {
                            printf("get imei and sv success!!!\n");
                        }else{
                            printf("get imei and sv error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_get_imei_and_sv dlsym error\n");
                    }
                }
                //flag_init = 0;
                break;
            }
            //qser_reset_modem
            case 12:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    qser_reset_modem = (int(*)(sim_client_handle_type h_sim))dlsym(dlHandle_sim,"qser_reset_modem");
                    if(NULL != qser_reset_modem)
                    {
                        res = qser_reset_modem(h_sim);
                        if(res == 0)
                        {
                            printf("reset modem success!!!\n");
                        }else{
                            printf("reset modem error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_reset_modem dlsym error\n");
                    }
                }
                //flag_init = 0;
                break;
            }
            //qser_get_version
            case 13:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    char buf[VER_SIZE]="";
                    qser_get_version = (int(*)(sim_client_handle_type h_sim, char *buf))dlsym(dlHandle_sim,"qser_get_version");
                    if(NULL != qser_get_version)
                    {
                        res = qser_get_version(h_sim, buf);
                        if(res == 0)
                        {
                            printf("get version success!!!\n");
                        }else{
                            printf("get version error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_get_version dlsym error\n");
                    }
                }
                //flag_init = 0;
                break;
            }
            //qser_reset_sim
            case 14:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    qser_reset_sim = (int(*)(sim_client_handle_type h_sim))dlsym(dlHandle_sim,"qser_reset_sim");
                    if(NULL != qser_reset_sim)
                    {
                        res = qser_reset_sim(h_sim);
                        if(res == 0)
                        {
                            printf("reset sim success!!!\n");
                        }else{
                            printf("reset sim error, res = %d\n", res);
                        }
                    }else{
                        printf("qser_reset_sim dlsym error\n");
                    }
                }
                //flag_init = 0;
                break;
            }
            case 15://qser_sim_addrxmsghandler
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    qser_sim_addrxmsghandler = (int(*)(QSER_SIM_RxMsgHandlerFunc_t handlerPtr))dlsym(dlHandle_sim,"qser_sim_addrxmsghandler");
                    if(NULL != qser_sim_addrxmsghandler)
                    {
                        res = qser_sim_addrxmsghandler(qser_sim_handler);
                        if(res == 0)
                        {
                            printf("sim addrxmsghandler success is!!!\n");
                        }else{
                            printf("sim addrxmsghandler errors, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_addrxmsghandler dlsym error\n");
                    }
                 }
                //flag_init = 0;
                break;
            }
            //qser_deinit_sim
            case 16:
            {
                if(flag_init == 0){
                    printf("must init first\n");
                }
                else{
                    qser_sim_client_deinit = (int(*)(sim_client_handle_type h_sim))dlsym(dlHandle_sim,"qser_sim_client_deinit");
                    if(NULL != qser_sim_client_deinit)
                    {
                        res = qser_sim_client_deinit(h_sim);
                        if(res == 0)
                        {
                            printf("sim deinit success is!!!\n");
                        }else{
                            printf("sim deint errors, res = %d\n", res);
                        }
                    }else{
                        printf("qser_sim_client_deinit dlsym error\n");
                    }
                 }
                flag_init = 0;
                break;
            }
            default:
                print_help();
                break;
        }

    }

   return 0;


}


