#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>
#include <ctype.h>

#include"lynq-qser-sms-demo.h"
#include "mbtk_log.h"


#define SMS_BUF  64
#define MSG_BUF  100

int flag_init = 0;

typedef uint32_t sim_client_handle_type;
//sms_client_handle_type  ph_sms = 2022;
sms_client_handle_type  h_sms = 2022;


int (*qser_sms_client_init)(sms_client_handle_type  *h_sms);
int (*qser_sms_client_deinit)(sms_client_handle_type h_sms);
int (*qser_sms_send_sms)(sms_client_handle_type h_sms, QSER_sms_info_t *pt_sms_info);
int (*qser_sms_addrxmsghandler)(QSER_SMS_RxMsgHandlerFunc_t handlerPtr, void* contextPtr);
int (*qser_sms_deletefromstorage)(sms_client_handle_type  h_sms, QSER_sms_storage_info_t  *pt_sms_storage);
int (*qser_sms_getsmscenteraddress)(sms_client_handle_type h_sms, QSER_sms_service_center_cfg_t *set_sca_cfg);
//int (*qser_sms_setsmscenteraddress)(sms_client_handle_type h_sms, QSER_sms_service_center_cfg_t *get_sca_cfg);

void qser_sms_handler(QSER_SMS_MsgRef msgRef, void* contextPtr) {
    printf("[%s-%d] sms handler, msgRef->sms_data = %s,addr =%s\n", __FUNCTION__, __LINE__, msgRef->sms_data, msgRef->src_addr);
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

//ä»¥ä¸‹ç¤ºä¾‹é»˜è®¤ä¸ºæ‰€æœ‰API æ‰§è¡Œéƒ½æˆåŠŸçš„æƒ…å†µä¸‹ï¼Œä¾æ¬¡æ‰§è¡ŒåŽç»­API
int main(int argc, char *argv[])
{
    int ret    = 0;
    //char phnum[SMS_BUF]={0};
    mbtk_log_init("radio", "MBTK");

    const char *lynq_libpath_sms = "/lib/liblynq-qser-sms.so";
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
    
    qser_sms_client_init = (int(*)(sms_client_handle_type  *h_sms))dlsym(dlHandle_sms,"qser_sms_client_init");
    if(NULL != qser_sms_client_init)
    {
        ret = qser_sms_client_init(&h_sms);
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
        printf("qser_sms_addrxmsghandler success\n");
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
#if 0
    QSER_sms_service_center_cfg_t set_sca_cfg;
    memset(set_sca_cfg.service_center_addr, 0, sizeof(set_sca_cfg.service_center_addr));
    strncpy(set_sca_cfg.service_center_addr, "+8613800230500", 14);//smsc need userchange
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
#endif
   // char telephony_num[SMS_BUF] = {};
    char msg[MSG_BUF] = {};
    QSER_sms_info_t  pt_sms_info;
   // sprintf(telephony_num,"10086");//è¯·ç”¨æˆ·åŠ¡å¿…æ›¿æ¢æ­¤å·ç ï¼Œéšæ„å‘è¿è¥å•†å‘çŸ­æ¶ˆæ¯æžæœ‰å¯èƒ½è¢«å°å·ã€‚
    sprintf(msg,"Chello worrld-7bit-mr!.");
    strncpy(pt_sms_info.src_addr, argv[1], SMS_BUF);
    strncpy(pt_sms_info.sms_data, msg, MSG_BUF);
    pt_sms_info.format = QSER_SMS_7BIT;
  //  printf("[%s,%d]  src_addr=%s, telephony_num = %s\n",__FUNCTION__,__LINE__, pt_sms_info.src_addr, telephony_num);
    printf("[%s,%d]  sms_data=%s, msg = %s\n",__FUNCTION__,__LINE__, pt_sms_info.sms_data, msg);

    qser_sms_send_sms = (int (*)(sms_client_handle_type h_sms, QSER_sms_info_t *pt_sms_info))dlsym(dlHandle_sms,"qser_sms_send_sms");
    if(NULL != qser_sms_send_sms)
    {
        ret = qser_sms_send_sms(h_sms, &pt_sms_info);
        if(ret == 0)
        {
            printf("send sms success\n");
        }else{
            printf("send sms error, ret = %d\n", ret);
        }
    }else{
        printf("qser_sms_send_sms dlsym error\n");
    }

    sleep(120);//åœ¨æ­¤æœŸé—´å‘æ¨¡ç»„å‘é€çŸ­æ¶ˆæ¯ï¼Œå³å¯è§¦å‘â€œæŽ¥å—æ–°çŸ­æ¶ˆæ¯äº‹ä»¶â€ã€‚
    qser_sms_client_deinit = (int (*)(sms_client_handle_type h_sms))dlsym(dlHandle_sms,"qser_sms_client_deinit");
    if(NULL != qser_sms_client_deinit)
    {
        ret = qser_sms_client_deinit(h_sms);
        if(ret == 0)
        {
            printf("sms deinit success\n");
        }else{
            printf("sms deinit error, ret = %d\n", ret);
        }
    }else{
        printf("qser_sms_client_deinit dlsym error\n");
    }
    
}

