/**
 *   \file dtmf_test.c
 *   \brief A Documented file.
 *
 *  Detailed description
 *   \Author:  jinLuo
 *   \Version: 1.0.0
 *   \Date: 2022-12-1
 */

/******************************************************************************\
 *   Include files
\******************************************************************************/
#include <pthread.h>
#include <time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lynq_qser_sms.h"
#define MAX_LEN 15


#if 0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>
#include <ctype.h>

//#include"lynq-qser-sms-demo.h"


#define SMS_BUF  64
#define MSG_BUF  100

int flag_init = 0;

typedef uint32_t sim_client_handle_type;
sms_client_handle_type  ph_sms = 2022;
sms_client_handle_type  h_sms = 2022;


int (*qser_sms_client_init)(sms_client_handle_type  *ph_sms);
int (*qser_sms_client_deinit)(sms_client_handle_type h_sms);
int (*qser_sms_send_sms)(sms_client_handle_type h_sms, QSER_sms_info_t *pt_sms_info);
int (*qser_sms_addrxmsghandler)(QSER_SMS_RxMsgHandlerFunc_t handlerPtr, void* contextPtr);
int (*qser_sms_deletefromstorage)(sms_client_handle_type  h_sms, QSER_sms_storage_info_t  *pt_sms_storage);
int (*qser_sms_getsmscenteraddress)(sms_client_handle_type h_sms, QSER_sms_service_center_cfg_t *set_sca_cfg);
int (*qser_sms_setsmscenteraddress)(sms_client_handle_type h_sms, QSER_sms_service_center_cfg_t *get_sca_cfg);

void qser_sms_handler(QSER_SMS_MsgRef msgRef, void* contextPtr) {
    printf("[%s-%d] sms handler, msgRef->sms_data = %s\n", __FUNCTION__, __LINE__, msgRef->sms_data);
}

bool isNumeric(char *phnum) {
    int length = strlen(phnum);
    
    if (length == 0) {
        return false;
    }
    
    if (phnum[0] != '+' && (phnum[0] < '0' || phnum[0] > '9')) {
        return false;
    }
    
    if (phnum[0] == '+' && length < 3) {
        return false;
    }
    
    for (int i = 1; i < length; i++) {
        if (phnum[i] < '0' || phnum[i] > '9') {
            return false;
        }
    }
    
    return true;
}

//以下示例默认为所有API 执行都成功的情况下，依次执行后续API
int main(int argc, char *argv[])
{
    int ret    = 0;
    //char phnum[SMS_BUF]={0};

    //const char *lynq_libpath_sms = "/lib/liblynq-qser-sms.so";
    const char *lynq_libpath_sms = "/lib/liblynq_lib.so";
    void *dlHandle_sms = dlopen(lynq_libpath_sms, RTLD_NOW);
    
    if (dlHandle_sms == NULL) 
    {
        printf("dlopen dlHandle_sms failed: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    if(argc != 2 || !isNumeric(argv[1]))
    {
        printf("[%s][%d] please input correct phnum\n",__FUNCTION__,__LINE__);
        exit(1);
    }
    
    qser_sms_client_init = (int(*)(sms_client_handle_type  *ph_sms))dlsym(dlHandle_sms,"qser_sms_client_init");
    if(NULL != qser_sms_client_init)
    {
        ret = qser_sms_client_init(&ph_sms);
        if(ret == 0)
        {
            printf("Run qser_sms_client_init\n");
        }else{
            printf("qser_sim_client_init error\n");
        }
     }else{
            printf("qser_sim_client_init dlsym error\n");
     }

    sleep(2);

    qser_sms_addrxmsghandler = (int(*)(QSER_SMS_RxMsgHandlerFunc_t handlerPtr, void* contextPtr))dlsym(dlHandle_sms,"qser_sms_addrxmsghandler");

    if(NULL != qser_sms_addrxmsghandler)
    {
        ret = qser_sms_addrxmsghandler(qser_sms_handler, NULL);
        if(ret == 0)
    {
        printf("qser_sms_addrxmsghandler success");
    }else{
        printf("qser_sms_addrxmsghandler error, ret = %d\n", ret);
    }
    }else{
        printf("qser_sms_addrxmsghandler dlsym error\n");
    }
    
    QSER_sms_service_center_cfg_t get_sca_cfg;
    qser_sms_getsmscenteraddress = (int (*)(sms_client_handle_type h_sms, QSER_sms_service_center_cfg_t *get_sca_cfg))dlsym(dlHandle_sms,"qser_sms_getsmscenteraddress");
    if(NULL != qser_sms_getsmscenteraddress)
    {
        ret = qser_sms_getsmscenteraddress(h_sms, &get_sca_cfg);
        if(ret == 0)
        {
            printf("get smsc success,service_center_addr = %s\n",get_sca_cfg.service_center_addr);
        }else{
            printf("get smsc error, ret = %d\n", ret);
        }
    }else{
        printf("qser_sms_getsmscenteraddress dlsym error\n");
    }
    sleep(1);
    QSER_sms_service_center_cfg_t set_sca_cfg;
    strncpy(set_sca_cfg.service_center_addr, "+8613800280500", 14);//smsc +8613800280500成都号码      +8613800230500黑龙江
    qser_sms_setsmscenteraddress = (int (*)(sms_client_handle_type h_sms, QSER_sms_service_center_cfg_t *set_sca_cfg))dlsym(dlHandle_sms,"qser_sms_setsmscenteraddress");
    if(NULL != qser_sms_setsmscenteraddress)
    {
        ret = qser_sms_setsmscenteraddress(h_sms, &set_sca_cfg);
        if(ret == 0)
        {
            printf("set smsc success");
        }else{
            printf("set smsc error, ret = %d\n", ret);
        }
    }else{
        printf("qser_sms_setsmscenteraddress dlsym error\n");
    }

    //再次查询是否设置成功
    sleep(1);
    qser_sms_getsmscenteraddress = (int (*)(sms_client_handle_type h_sms, QSER_sms_service_center_cfg_t *get_sca_cfg))dlsym(dlHandle_sms,"qser_sms_getsmscenteraddress");
    if(NULL != qser_sms_getsmscenteraddress)
    {
        ret = qser_sms_getsmscenteraddress(h_sms, &get_sca_cfg);
        if(ret == 0)
        {
            printf("get smsc success,service_center_addr = %s\n",get_sca_cfg.service_center_addr);
        }else{
            printf("get smsc error, ret = %d\n", ret);
        }
    }else{
        printf("qser_sms_getsmscenteraddress dlsym error\n");
    }


    //test 7bit
    //char telephony_num[SMS_BUF] = {};
    char msg[MSG_BUF] = {};
    QSER_sms_info_t  pt_sms_info;
    //sprintf(telephony_num,"18981904623");//请用户务必替换此号码，随意向运营商发短消息极有可能被封号。
    sprintf(msg,"hello");
    strncpy(pt_sms_info.src_addr, argv[1], SMS_BUF);
    strncpy(pt_sms_info.sms_data, msg, MSG_BUF);
    pt_sms_info.format = QSER_SMS_7BIT;
  //  printf("[%s,%d]  src_addr=%s, telephony_num = %s\n",__FUNCTION__,__LINE__, pt_sms_info.src_addr, telephony_num);
    printf("[%s,%d]  sms_data=%s, msg = %s\n",__FUNCTION__,__LINE__, pt_sms_info.sms_data, msg);

    sleep(120);

    //test ucs2
    //char telephony_num[SMS_BUF] = {};
    char msg[MSG_BUF] = {};
    QSER_sms_info_t  pt_sms_info;
    //sprintf(telephony_num,"18981904623");//请用户务必替换此号码，随意向运营商发短消息极有可能被封号。
    sprintf(msg,"你好吗");
    strncpy(pt_sms_info.src_addr, argv[1], SMS_BUF);
    strncpy(pt_sms_info.sms_data, msg, MSG_BUF);
    pt_sms_info.format = QSER_SMS_UCS2;
    //  printf("[%s,%d]  src_addr=%s, telephony_num = %s\n",__FUNCTION__,__LINE__, pt_sms_info.src_addr, telephony_num);
    printf("[%s,%d]  sms_data=%s, msg = %s\n",__FUNCTION__,__LINE__, pt_sms_info.sms_data, msg);

    qser_sms_send_sms = (int (*)(sms_client_handle_type h_sms, QSER_sms_info_t *pt_sms_info))dlsym(dlHandle_sms,"qser_sms_send_sms");
    if(NULL != qser_sms_send_sms)
    {
        ret = qser_sms_send_sms(h_sms, &pt_sms_info);
        if(ret == 0)
        {
            printf("send sms success");
        }else{
            printf("send sms error, ret = %d\n", ret);
        }
    }else{
        printf("qser_sms_send_sms dlsym error\n");
    }

    sleep(120);//在此期间向模组发送短消息，即可触发“接受新短消息事件”。
    qser_sms_client_deinit = (int (*)(sms_client_handle_type h_sms))dlsym(dlHandle_sms,"qser_sms_client_deinit");
    if(NULL != qser_sms_client_deinit)
    {
        ret = qser_sms_client_deinit(h_sms);
        if(ret == 0)
        {
            printf("sms deinit success");
        }else{
            printf("sms deinit error, ret = %d\n", ret);
        }
    }else{
        printf("qser_sms_client_deinit dlsym error\n");
    }
    
}

#else

void qser_sms_handler(QSER_SMS_MsgRef msgRef, void* contextPtr) {
    printf("[%s-%d] sms handler, msgRef->sms_data = %s\n", __FUNCTION__, __LINE__, msgRef->sms_data);
}


int main(int argc, char *argv[])
{
    char operator[10];
    char serNum[50] = {0};
    char phone_number[MAX_LEN] ={0};
    int opt;
    int ret, uToken; 
    sms_client_handle_type handle = -1;
    QSER_sms_info_t test_sms_info;
    QSER_sms_service_center_cfg_t test_sca_cfg;
    QSER_sms_storage_info_t test_sms_storage;
    mbtk_log_init("radio", "SMS_TEST");

    printf("=========sms main=========\n"
        "\t0 exit\n"
        "\t1 sms init\n"
        "\t2 send text sms\n"
        "\t3 wait receive new sms\n"
        "\t4 send PDU sms\n"
        "\t5 //delete sms list\n"
        "\t6 delete sms\n"
        "\t7 query service number\n"
        "\t8 set service number\n"
        "\t9 deinit sms\n"
        "operator: >> \n");

    while(1)
    {
        fgets(operator, sizeof(operator), stdin);
        fflush(stdin);
        opt = atoi(operator);
        switch (opt)
        {
        case 0:
            printf("main exit\n");
            return 0;
        case 1:
            {
                ret = qser_sms_client_init(&handle);
                if(ret == 0)
                    printf("Run qser_sms_client_init success\n");
                else
                    printf("qser_sim_client_init error\n");
            }
            break;
        case 2:
            {
                memset(&test_sms_info, 0x00, sizeof(QSER_sms_info_t));
                char *tmp = "all man hello world";
                printf("input phone number:\n");
                memset(phone_number, 0x0, MAX_LEN);
                fgets(phone_number, MAX_LEN, stdin);
                fflush(stdin);
                int len = strlen(tmp);
                int num_len = strlen(phone_number);
                test_sms_info.format = 0;
                memcpy(test_sms_info.sms_data, tmp, len);
                memcpy(test_sms_info.src_addr, phone_number, num_len);
                ret = qser_sms_send_sms(handle, &test_sms_info);
                if(ret == 0)
                    printf("Run qser_sms_send_sms success\n");
                else
                    printf("qser_sms_send_sms error\n");
            }
            break;
        case 3:
            {
                ret = qser_sms_addrxmsghandler(qser_sms_handler, NULL);
                if(ret == 0)
                    printf("Run qser_sms_addrxmsghandler success\n");
                else
                    printf("qser_sms_addrxmsghandler error\n");
            }
            break;
        case 4:
            {
                memset(&test_sms_info, 0x00, sizeof(QSER_sms_info_t));
                char *tmp = "你好"; 
                printf("input phone number:\n");
                memset(phone_number, 0x0, MAX_LEN);
                fgets(phone_number, MAX_LEN, stdin);
                fflush(stdin);
                int len = strlen(tmp);
                int num_len = strlen(phone_number);
                test_sms_info.format = 2;
                memcpy(test_sms_info.sms_data, tmp, len);
                memcpy(test_sms_info.src_addr, phone_number, num_len);
                ret = qser_sms_send_sms(handle, &test_sms_info);
                if(ret == 0)
                    printf("Run qser_sms_send_sms success\n");
                else
                    printf("qser_sms_send_sms error\n");
            }
            break;
        case 5:
            {
                /*
                char list_del[1024] = {0};

                if (strcmp(list_del, "") == 0)
                    printf("list_del = NULL\n");
                else
                    printf("list_del = [%s]\n",list_del);
                 */
            }
            break;
        case 6:
            {
                memset(&test_sms_storage, 0x00, sizeof(QSER_sms_storage_info_t));
                memset(operator,0x00, sizeof(operator));
                printf("please input index \n");
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                opt = atoi(operator);
                int t_index = opt;
                test_sms_storage.storage_idx = t_index;
                ret = qser_sms_deletefromstorage(handle, &test_sms_storage);
                if(ret == 0)
                    printf("Run qser_sms_deletefromstorage success\n");
                else
                    printf("qser_sms_deletefromstorage error\n");
            }
            break;
        case 7:
            {
                memset(&test_sca_cfg, 0x00, sizeof(QSER_sms_service_center_cfg_t));
                ret = qser_sms_getsmscenteraddress(handle,&test_sca_cfg);
                if(!ret)
                    printf("get_smsc:%s success\n", test_sca_cfg.service_center_addr);
                else
                    printf("qser_sms_getsmscenteraddress error\n");
            }
            break;
        case 8:
            {
                memset(&test_sca_cfg, 0x00, sizeof(QSER_sms_service_center_cfg_t));
                printf("input phone number:\n");
                memset(phone_number, 0x0, MAX_LEN);
                fgets(phone_number, MAX_LEN, stdin);
                fflush(stdin);
                int len = strlen(phone_number);
                memcpy(test_sca_cfg.service_center_addr, phone_number, len);
                ret= qser_sms_setsmscenteraddress(handle, &test_sca_cfg);
                if(ret == 0)
                    printf("Run qser_sms_setsmscenteraddress success\n");
                else
                    printf("qser_sms_setsmscenteraddress error\n");
            }
            break;
        case 9:
            {
                ret = qser_sms_client_deinit(handle); 
                if(ret == 0)
                    printf("Run qser_sms_client_deinit success\n");
                else
                    printf("qser_sms_client_deinit error\n");
            }
            break;
        default:
            break;
        }
    }

    return 0;
}

#endif



