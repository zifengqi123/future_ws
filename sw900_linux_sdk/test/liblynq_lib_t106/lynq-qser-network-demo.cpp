#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
//#include <pthread.h>
#include <unistd.h>
#include "lynq_qser_network.h"

#ifndef LOG_TAG
#define LOG_TAG "QSER_NETWORK_DEMO"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void *handle_network;

int (*qser_nw_client_init_p)(nw_client_handle_type  * h_nw);
int (*qser_nw_client_deinit_p)(nw_client_handle_type h_nw);
int (*qser_nw_set_config_p)(nw_client_handle_type  h_nw, QSER_NW_CONFIG_INFO_T *pt_info);
int (*qser_nw_get_operator_name_p)(nw_client_handle_type    h_nw, QSER_NW_OPERATOR_NAME_INFO_T    *pt_info );
int (*qser_nw_get_reg_status_p)(nw_client_handle_type  h_nw, QSER_NW_REG_STATUS_INFO_T *pt_info);
int (*qser_nw_add_rx_msg_handler_p)(nw_client_handle_type  h_nw, QSER_NW_RxMsgHandlerFunc_t handlerPtr,void* contextPtr);
int (*qser_nw_get_signal_strength_p)(nw_client_handle_type h_nw,QSER_NW_SIGNAL_STRENGTH_INFO_T *pt_info);
int (*qser_nw_set_oos_config_p)(nw_client_handle_type   h_nw, QSER_NW_OOS_CONFIG_INFO_T *pt_info);
int (*qser_nw_get_oos_config_p)(nw_client_handle_type   h_nw, QSER_NW_OOS_CONFIG_INFO_T *pt_info);
int (*qser_nw_set_rf_mode_p) (nw_client_handle_type   h_nw,E_QSER_NW_RF_MODE_TYPE_T rf_mode);
int (*qser_nw_get_rf_mode_p) (nw_client_handle_type   h_nw,E_QSER_NW_RF_MODE_TYPE_T* rf_mode);
int (*qser_nw_set_ims_enable_p) (nw_client_handle_type h_nw,E_QSER_NW_IMS_MODE_TYPE_T ims_mode);
int (*qser_nw_get_ims_reg_status_p) (nw_client_handle_type h_nw, QSER_NW_IMS_REG_STATUS_INFO_T *pt_info);



int getFunc()
{
    const char *lynq_libpath_network = "/lib/liblynq-qser-network.so";

    handle_network = dlopen(lynq_libpath_network,RTLD_NOW);
    if(NULL == handle_network)
    {
        printf("dlopen lynq_libpath_network fail:%s",dlerror());
        exit(EXIT_FAILURE);
    }

    qser_nw_client_init_p = (int (*)(nw_client_handle_type  * h_nw))dlsym(handle_network,"qser_nw_client_init");    
    qser_nw_client_deinit_p = (int (*)(nw_client_handle_type h_nw))dlsym(handle_network,"qser_nw_client_deinit");
    qser_nw_set_config_p = (int (*)(nw_client_handle_type  h_nw, QSER_NW_CONFIG_INFO_T *pt_info))dlsym(handle_network,"qser_nw_set_config");
    qser_nw_get_operator_name_p = (int (*)(nw_client_handle_type  h_nw, QSER_NW_OPERATOR_NAME_INFO_T  *pt_info ))dlsym(handle_network,"qser_nw_get_operator_name");
    qser_nw_get_reg_status_p = (int (*)(nw_client_handle_type  h_nw, QSER_NW_REG_STATUS_INFO_T  *pt_info))dlsym(handle_network,"qser_nw_get_reg_status");
    qser_nw_get_signal_strength_p = (int (*)(nw_client_handle_type h_nw, QSER_NW_SIGNAL_STRENGTH_INFO_T *pt_info))dlsym(handle_network,"qser_nw_get_signal_strength");
    qser_nw_add_rx_msg_handler_p = (int (*)(nw_client_handle_type  h_nw, QSER_NW_RxMsgHandlerFunc_t handlerPtr,void* contextPtr))dlsym(handle_network,"qser_nw_add_rx_msg_handler");
    qser_nw_get_oos_config_p = (int (*)(nw_client_handle_type   h_nw, QSER_NW_OOS_CONFIG_INFO_T *pt_info))dlsym(handle_network,"qser_nw_get_oos_config");
    qser_nw_set_oos_config_p = (int (*)(nw_client_handle_type   h_nw, QSER_NW_OOS_CONFIG_INFO_T *pt_info))dlsym(handle_network,"qser_nw_set_oos_config");
    qser_nw_set_rf_mode_p = (int (*)(nw_client_handle_type   h_nw, E_QSER_NW_RF_MODE_TYPE_T rf_mode))dlsym(handle_network,"qser_nw_set_rf_mode");
    qser_nw_get_rf_mode_p = (int (*)(nw_client_handle_type   h_nw, E_QSER_NW_RF_MODE_TYPE_T* rf_mode))dlsym(handle_network,"qser_nw_get_rf_mode");
    qser_nw_set_ims_enable_p = (int (*)(nw_client_handle_type  h_nw, E_QSER_NW_IMS_MODE_TYPE_T ims_mode))dlsym(handle_network,"qser_nw_set_ims_enable");
    qser_nw_get_ims_reg_status_p = (int (*)(nw_client_handle_type  h_nw, QSER_NW_IMS_REG_STATUS_INFO_T  *pt_info))dlsym(handle_network,"qser_nw_get_ims_reg_status");
        
    if(qser_nw_client_deinit_p==NULL || qser_nw_client_init_p==NULL || qser_nw_set_config_p ==NULL ||
       qser_nw_get_operator_name_p == NULL || qser_nw_get_reg_status_p ==NULL || qser_nw_add_rx_msg_handler_p==NULL ||
       qser_nw_set_rf_mode_p == NULL || qser_nw_get_rf_mode_p == NULL || qser_nw_get_oos_config_p == NULL || qser_nw_set_oos_config_p == NULL || 
       qser_nw_set_ims_enable_p == NULL || qser_nw_get_ims_reg_status_p == NULL)
    {
        printf("get func pointer null");
        exit(EXIT_FAILURE);
    }    
    return  0;    
}

static int test_nw(void);

int main(int argc, char const *argv[])
{
    printf("--------->[%s,%d] start \n",__FUNCTION__,__LINE__);   
    
    if(getFunc()==0)
    {
        test_nw();
    }    

    return 0;
}

typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;

st_api_test_case at_nw_testlist[] = 
{
    {0,     "qser_nw_client_init"},
    {1,     "qser_nw_set_config"},      
    {2,     "qser_nw_get_operator_name"},
    {3,     "qser_nw_get_reg_status"},        
    {4,     "qser_nw_add_rx_msg_handler"},     
    {5,     "qser_nw_get_signal_strength"},
    {6,     "qser_nw_set_oos_config"},
    {7,     "qser_nw_get_oos_config"},
    {8,     "qser_nw_set_rf_mode"},
    {9,     "qser_nw_get_rf_mode"},
    {10,     "qser_nw_set_ims_enable"},
    {11,     "qser_nw_get_ims_reg_status"},
    {12,     "qser_nw_client_deinit"},      
    {-1,    "quit"}
};

typedef int (*TEST)(void);

typedef struct
{
    char                *group_name;
    st_api_test_case    *test_cases;
    TEST                pf_test;
} func_api_test_t;

func_api_test_t t_nw_test = {"nw", at_nw_testlist, test_nw};

void show_group_help(func_api_test_t *pt_test)
{
    int i;

    printf("Group Name:%s, Supported test cases:\n", pt_test->group_name);
    for(i = 0; ; i++)
    {
        if(pt_test->test_cases[i].cmdIdx == -1)
        {
            break;
        }
        printf("%d:\t%s\n", pt_test->test_cases[i].cmdIdx, pt_test->test_cases[i].funcName);
    }
}

static nw_client_handle_type h_nw = 0;

char *tech_domain[] = {"NONE", "3GPP", "3GPP2"};
char *radio_tech[] = {"unknown", 
    "TD_SCDMA", "GSM",      "HSPAP",    "LTE",      "EHRPD",    "EVDO_B", 
    "HSPA",     "HSUPA",    "HSDPA",    "EVDO_A",   "EVDO_0",   "1xRTT", 
    "IS95B",    "IS95A",    "UMTS",     "EDGE",     "GPRS",     "NONE"};

void nw_event_ind_handler (
    nw_client_handle_type h_nw, 
    u_int32_t ind_flag, 
    void                  *ind_msg_buf, 
    u_int32_t              ind_msg_len, 
    void                  *contextPtr)
{
    switch(ind_flag) {
        case NW_IND_VOICE_REG_EVENT_IND_FLAG:
            {
                QSER_NW_VOICE_REG_EVENT_IND_T *ind = (QSER_NW_VOICE_REG_EVENT_IND_T*)ind_msg_buf;
                printf("Recv event indication : VOICE REG EVENT\n");
                
                if(ind==NULL)
                {
                     printf("ind is NULL\n");
                     break;
                }

                if(ind->registration_valid)
                {
                    printf("voice_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                            tech_domain[ind->registration.tech_domain], 
                            radio_tech[ind->registration.radio_tech],
                            ind->registration.roaming,
                            ind->registration.registration_state);
                }
                if(ind->registration_details_3gpp_valid)
                {
                    printf("voice_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                            tech_domain[ind->registration_details_3gpp.tech_domain], 
                            radio_tech[ind->registration_details_3gpp.radio_tech],
                            ind->registration_details_3gpp.mcc,
                            ind->registration_details_3gpp.mnc,
                            ind->registration_details_3gpp.roaming,
                            ind->registration_details_3gpp.forbidden,                    
                            ind->registration_details_3gpp.cid,
                            ind->registration_details_3gpp.lac,
                            ind->registration_details_3gpp.psc,
                            ind->registration_details_3gpp.tac);
                }

                if(ind->registration_details_3gpp2_valid)
                {
                    printf("voice_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, sid=%d, nid=%d, bsid=%d\n", 
                            tech_domain[ind->registration_details_3gpp2.tech_domain], 
                            radio_tech[ind->registration_details_3gpp2.radio_tech],
                            ind->registration_details_3gpp2.mcc,
                            ind->registration_details_3gpp2.mnc,
                            ind->registration_details_3gpp2.roaming,
                            ind->registration_details_3gpp2.forbidden,                    
                            ind->registration_details_3gpp2.sid,
                            ind->registration_details_3gpp2.nid,
                            ind->registration_details_3gpp2.bsid);
                }

                break;
            }
        case NW_IND_DATA_REG_EVENT_IND_FLAG:
            {
                QSER_NW_DATA_REG_EVENT_IND_T *ind = (QSER_NW_DATA_REG_EVENT_IND_T*)ind_msg_buf;

                printf("Recv event indication : DATA REG EVENT\n");
                
                if(ind==NULL)
                {
                     printf("ind is NULL\n");
                     break;
                }
              

                if(ind->registration_valid)
                {
                    printf("data_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                            tech_domain[ind->registration.tech_domain], 
                            radio_tech[ind->registration.radio_tech],
                            ind->registration.roaming,
                            ind->registration.registration_state);
                }
                if(ind->registration_details_3gpp_valid)
                {
                    printf("data_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                            tech_domain[ind->registration_details_3gpp.tech_domain], 
                            radio_tech[ind->registration_details_3gpp.radio_tech],
                            ind->registration_details_3gpp.mcc,
                            ind->registration_details_3gpp.mnc,
                            ind->registration_details_3gpp.roaming,
                            ind->registration_details_3gpp.forbidden,                    
                            ind->registration_details_3gpp.cid,
                            ind->registration_details_3gpp.lac,
                            ind->registration_details_3gpp.psc,
                            ind->registration_details_3gpp.tac);
                }

                if(ind->registration_details_3gpp2_valid)
                {
                    printf("data_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, prl=%d, css=%d, sid=%d, nid=%d, bsid=%d\n", 
                            tech_domain[ind->registration_details_3gpp2.tech_domain], 
                            radio_tech[ind->registration_details_3gpp2.radio_tech],
                            ind->registration_details_3gpp2.mcc,
                            ind->registration_details_3gpp2.mnc,
                            ind->registration_details_3gpp2.roaming,
                            ind->registration_details_3gpp2.forbidden,   
                            ind->registration_details_3gpp2.inPRL,
                            ind->registration_details_3gpp2.css,
                            ind->registration_details_3gpp2.sid,
                            ind->registration_details_3gpp2.nid,
                            ind->registration_details_3gpp2.bsid);
                }

                break;
            }
        case NW_IND_SIGNAL_STRENGTH_EVENT_IND_FLAG:
            {
                QSER_NW_SINGNAL_EVENT_IND_T *ind = (QSER_NW_SINGNAL_EVENT_IND_T*)ind_msg_buf;

                printf("Recv event indication : SIGNAL STRENGTH EVENT\n");
                
                if(ind==NULL)
                {
                     printf("ind is NULL\n");
                     break;
                }

                if(ind->gsm_sig_info_valid)
                {
                    printf("gsm_sig_info: rssi=%d\n", 
                            ind->gsm_sig_info.rssi);
                } 

                if(ind->wcdma_sig_info_valid)
                {
                    printf("wcdma_sig_info: rssi=%d, ecio=%d\n", 
                            ind->wcdma_sig_info.rssi, 
                            ind->wcdma_sig_info.ecio);
                } 
                if(ind->tdscdma_sig_info_valid)
                {
                    printf("tdscdma_sig_info: rssi=%d, rscp=%d, ecio=%d, sinr=%d\n", 
                            ind->tdscdma_sig_info.rssi, 
                            ind->tdscdma_sig_info.rscp,
                            ind->tdscdma_sig_info.ecio,
                            ind->tdscdma_sig_info.sinr);
                } 
                if(ind->lte_sig_info_valid)
                {
                    printf("lte_sig_info: rssi=%d, rsrq=%d, rsrp=%d, snr=%d\n", 
                            ind->lte_sig_info.rssi, 
                            ind->lte_sig_info.rsrq,
                            ind->lte_sig_info.rsrp,
                            ind->lte_sig_info.snr);
                } 
                if(ind->cdma_sig_info_valid)
                {
                    printf("cdma_sig_info: rssi=%d, ecio=%d\n", 
                            ind->cdma_sig_info.rssi, 
                            ind->cdma_sig_info.ecio);
                } 
                if(ind->hdr_sig_info_valid)
                {
                    printf("hdr_sig_info: rssi=%d, ecio=%d, sinr=%d, io=%d\n", 
                            ind->hdr_sig_info.rssi, 
                            ind->hdr_sig_info.ecio,
                            ind->hdr_sig_info.sinr,
                            ind->hdr_sig_info.io);
                }
                break;
            }
        case NW_IND_IMS_REG_EVENT_IND_FLAG:
            {
                printf("Recv event indication : IMS REG EVENT\n");               
                
                break;
            }
        default:
            break;
    }
}

static int test_nw(void)
{
    int    cmdIdx  = 0;
    int    ret     = 0;       
        
    while(1)
    {
        show_group_help(&t_nw_test);
        printf("please input cmd index(-1 exit): ");
        ret = scanf("%d", &cmdIdx);
        if(ret != 1)
        {
            char c;
            while(((c=getchar()) != '\n') && (c != EOF))
            {
                ;
            }
            continue;
        }
        if(cmdIdx == -1)
        {
            break;
        }
        switch(cmdIdx)
        {
             case 0://"qser_nw_client_init"
             {
                 
                 ret = qser_nw_client_init(&h_nw);
                 printf("qser_nw_client_init ret = %d\n", ret);
                 break;
             }
             case 1://"qser_nw_set_config"
             {
                 QSER_NW_CONFIG_INFO_T     t_info = {0};
                 
                 int mask = 0;
                 printf("please input decimal format number,  whose hex format is (TDSCDMA | LTE | EVDO | CDMA | WCDMA | GSM) : \n");
                 scanf("%d", &mask);
                 t_info.preferred_nw_mode = mask;
                 
                 ret = qser_nw_set_config(h_nw, &t_info);
                 printf("qser_nw_set_config ret = %d\n", ret);
                 break;
             }            
             case 2://"qser_nw_get_operator_name"
             {
                 QSER_NW_OPERATOR_NAME_INFO_T  t_info;
                 ret = qser_nw_get_operator_name(h_nw, &t_info);
                 printf("qser_nw_get_operator_name ret = %d, long_eons=%s, short_eons=%s, mcc=%s, mnc=%s\n", ret,
                         t_info.long_eons, t_info.short_eons, t_info.mcc, t_info.mnc);
                 break;
             }       
             case 3://"qser_nw_get_reg_status"
             {
                 QSER_NW_REG_STATUS_INFO_T         t_info;
     
                 memset(&t_info, 0, sizeof(QSER_NW_REG_STATUS_INFO_T));
                 ret = qser_nw_get_reg_status(h_nw, &t_info);
                 printf("qser_nw_get_reg_status ret = %d, detail info:\n", ret);
                 if(t_info.voice_registration_valid)
                 {
                     printf("voice_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                         tech_domain[t_info.voice_registration.tech_domain], 
                         radio_tech[t_info.voice_registration.radio_tech],
                         t_info.voice_registration.roaming,
                         t_info.voice_registration.registration_state);
                 }
                 if(t_info.data_registration_valid)
                 {
                     printf("data_registration: \ntech_domain=%s, radio_tech=%s, roaming=%d, registration_state=%d\n", 
                         tech_domain[t_info.data_registration.tech_domain], 
                         radio_tech[t_info.data_registration.radio_tech],
                         t_info.data_registration.roaming,
                         t_info.data_registration.registration_state);
                 }
                 if(t_info.voice_registration_details_3gpp_valid)
                 {
                     printf("voice_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                         tech_domain[t_info.voice_registration_details_3gpp.tech_domain], 
                         radio_tech[t_info.voice_registration_details_3gpp.radio_tech],
                         t_info.voice_registration_details_3gpp.mcc,
                         t_info.voice_registration_details_3gpp.mnc,
                         t_info.voice_registration_details_3gpp.roaming,
                         t_info.voice_registration_details_3gpp.forbidden,                    
                         t_info.voice_registration_details_3gpp.cid,
                         t_info.voice_registration_details_3gpp.lac,
                         t_info.voice_registration_details_3gpp.psc,
                         t_info.voice_registration_details_3gpp.tac);
                 }
                 if(t_info.data_registration_details_3gpp_valid)
                 {
                     printf("data_registration_details_3gpp: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, cid=0x%X, lac=%d, psc=%d, tac=%d\n", 
                         tech_domain[t_info.data_registration_details_3gpp.tech_domain], 
                         radio_tech[t_info.data_registration_details_3gpp.radio_tech],
                         t_info.data_registration_details_3gpp.mcc,
                         t_info.data_registration_details_3gpp.mnc,
                         t_info.data_registration_details_3gpp.roaming,
                         t_info.data_registration_details_3gpp.forbidden,                    
                         t_info.data_registration_details_3gpp.cid,
                         t_info.data_registration_details_3gpp.lac,
                         t_info.data_registration_details_3gpp.psc,
                         t_info.data_registration_details_3gpp.tac);
                 }
                 
                 if(t_info.voice_registration_details_3gpp2_valid)
                 {
                     printf("voice_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, sid=%d, nid=%d, bsid=%d\n", 
                         tech_domain[t_info.voice_registration_details_3gpp2.tech_domain], 
                         radio_tech[t_info.voice_registration_details_3gpp2.radio_tech],
                         t_info.voice_registration_details_3gpp2.mcc,
                         t_info.voice_registration_details_3gpp2.mnc,
                         t_info.voice_registration_details_3gpp2.roaming,
                         t_info.voice_registration_details_3gpp2.forbidden,                    
                         t_info.voice_registration_details_3gpp2.sid,
                         t_info.voice_registration_details_3gpp2.nid,
                         t_info.voice_registration_details_3gpp2.bsid);
                 }
                 
                 if(t_info.data_registration_details_3gpp2_valid)
                 {
                     printf("data_registration_details_3gpp2: \ntech_domain=%s, radio_tech=%s, mcc=%s, mnc=%s, roaming=%d, forbidden=%d, sid=%d, nid=%d, bsid=%d\n", 
                         tech_domain[t_info.data_registration_details_3gpp2.tech_domain], 
                         radio_tech[t_info.data_registration_details_3gpp2.radio_tech],
                         t_info.data_registration_details_3gpp2.mcc,
                         t_info.data_registration_details_3gpp2.mnc,
                         t_info.data_registration_details_3gpp2.roaming,
                         t_info.data_registration_details_3gpp2.forbidden,                    
                         t_info.data_registration_details_3gpp2.sid,
                         t_info.data_registration_details_3gpp2.nid,
                         t_info.data_registration_details_3gpp2.bsid);
                 }
                             
                 break;
             }                 
             case 12://"qser_nw_client_deinit"
             {
                 ret = qser_nw_client_deinit(h_nw);
                 printf("qser_nw_client_deinit ret = %d\n", ret);
                 break;
             }       
             case 4 :
             {
                ret = qser_nw_add_rx_msg_handler(h_nw, nw_event_ind_handler, NULL);  
                printf("qser_nw_add_rx_msg_handler, ret=%d\n", ret);
                break;
             }
              case 5 :
             {
                QSER_NW_SIGNAL_STRENGTH_INFO_T t_info;
                ret = qser_nw_get_signal_strength(h_nw, &t_info);  
                printf("qser_nw_get_signal_strength, ret=%d\n", ret);
                if(ret==0)
                {                       
                    printf("qser_solicited_signal_strength gsm_sig_info_valid = %d\n"
                               "                               gsm_sig_info.rssi = %d\n"
                               "                               wcdma     _sig_info_valid = %d\n" 
                               "                               wcdma_sig_info.rssi = %d\n"
                               "                               wcdma_sig_info.ecio = %d\n"
                               "                               tdscdma_sig_info_valid = %d\n"
                               "                               lte_sig_info_valid = %d\n"
                               "                               lte_sig_info.rssi = %d\n"
                               "                               lte_sig_info.rsrp = %d\n"
                               "                               lte_sig_info.rsrq = %d\n"
                               "                               lte_sig_info.snr = %d\n"
                               "                               cdma_sig_info_valid = %d\n"
                               "                               hdr_sig_info_valid = %d\n"
                               "                               nr_sig_info_valid = %d\n"
                               "                               nr_sig_info.ssRsrp = %d\n"
                               "                               nr_sig_info.ssRsrq = %d\n"
                               "                               nr_sig_info.ssSinr = %d\n"
                               "                               nr_sig_info.csiRsrp = %d\n"
                               "                               nr_sig_info.csiRsrq = %d\n"
                               "                               nr_sig_info.csiSinr = %d\n",
                              t_info.gsm_sig_info_valid, t_info.gsm_sig_info.rssi,
                              t_info.wcdma_sig_info_valid, t_info.wcdma_sig_info.rssi,  t_info.wcdma_sig_info.ecio,
                              t_info.tdscdma_sig_info_valid,
                              t_info.lte_sig_info_valid, t_info.lte_sig_info.rssi, t_info.lte_sig_info.rsrp, t_info.lte_sig_info.rsrq, t_info.lte_sig_info.snr,
                              t_info.cdma_sig_info_valid, 
                              t_info.hdr_sig_info_valid, 
                              t_info.nr_sig_info_valid, t_info.nr_sig_info.ssRsrp, t_info.nr_sig_info.ssRsrq, t_info.nr_sig_info.ssSinr,
                              t_info.nr_sig_info.csiRsrp, t_info.nr_sig_info.csiRsrq, t_info.nr_sig_info.csiSinr);
                        
                }
                break;
             }        
             case 7 :
             {
                QSER_NW_OOS_CONFIG_INFO_T t_info;
                int type = 0;
                printf("please input you want query oos config's type (0: fast network scan config; 1: full band network scan config ) : \n");
                scanf("%d", &type);
                t_info.type = type;
                ret = qser_nw_get_oos_config(h_nw, &t_info);  
                printf("qser_nw_get_oos_config, ret=%d\n", ret);
                if(ret==0)
                {         
                    if(t_info.type == QSER_NW_OOS_CFG_TYPE_FULL_BAND_SCAN)
                    {
                        printf("qser_nw_get_oos_config         t_min = %d\n"
                               "                               t_step = %d\n"
                               "                               t_num = %d\n"
                               "                               t_max = %d\n",
                              t_info.u.full_band_scan_info.t_min, t_info.u.full_band_scan_info.t_step,                             
                              t_info.u.full_band_scan_info.t_num, t_info.u.full_band_scan_info.t_max);
                    }     
                    else if(t_info.type == QSER_NW_OOS_CFG_TYPE_FAST_SCAN)
                    {
                        printf("qser_nw_get_oos_config         enable = %d\n"
                               "                               time_interval = %d\n",
                              t_info.u.fast_can_info.enable, t_info.u.fast_can_info.time_interval); 
                    }
                    else
                    {
                        printf("qser_nw_get_oos_config tyep is %d, ret is ok",t_info.type);
                    }
    
                }
                break;
             }         
             case 6 :
             {
                QSER_NW_OOS_CONFIG_INFO_T t_info;
                int type = 0;
                printf("please input you want set oos config's type (0: fast network scan config; 1: full band network scan config ) : \n");
                scanf("%d", &type);
                t_info.type = type;
                if(t_info.type == QSER_NW_OOS_CFG_TYPE_FULL_BAND_SCAN)
                {
                      printf("please input  t_min: \n");
                      scanf("%d", &(t_info.u.full_band_scan_info.t_min));
                      printf("please input t_step: \n");
                      scanf("%d", &(t_info.u.full_band_scan_info.t_step));
                      printf("please input  t_num: \n");
                      scanf("%d", &(t_info.u.full_band_scan_info.t_num));
                      printf("please input t_max: \n");
                      scanf("%d", &(t_info.u.full_band_scan_info.t_max));
                      ret = qser_nw_set_oos_config(h_nw, &t_info);  
                      printf("qser_nw_get_oos_config, ret=%d\n", ret);                      
                }
                else if(t_info.type==QSER_NW_OOS_CFG_TYPE_FAST_SCAN)
                {
                      printf("please input  enable: \n");
                      scanf("%d", &(t_info.u.fast_can_info.enable));
                      printf("please input time_interval: \n");
                      scanf("%d", &(t_info.u.fast_can_info.time_interval));                      
                      ret = qser_nw_set_oos_config(h_nw, &t_info);  
                      printf("qser_nw_get_oos_config, ret=%d\n", ret);                    
                }
                else 
                {
                       ret = qser_nw_set_oos_config(h_nw, &t_info);  
                       printf("qser_nw_get_oos_config, ret=%d\n", ret);            
                }                
                break;
             }         
             case 9://"qser_nw_get_rf_mode"
             {
                 E_QSER_NW_RF_MODE_TYPE_T  rf_mode;
                 ret = qser_nw_get_rf_mode(h_nw, &rf_mode);
                 printf("qser_nw_get_rf_mode ret = %d, rf mode=%d\n", ret, rf_mode);
                 break;
             }       
             case 8://"qser_nw_set_rf_mode"
             {
                 E_QSER_NW_RF_MODE_TYPE_T  rf_mode;
                 printf("please input you want set rf mode (0: cfun 0;  1: cfun 1; 4: flight mode \n");
                 scanf("%d", &rf_mode);
                 ret = qser_nw_set_rf_mode(h_nw, rf_mode);
                 printf("qser_nw_set_rf_mode %d ret = %dn",rf_mode, ret);
                 break;
             }       
             case 10://"qser_nw_set_ims_enable"
             {
                 E_QSER_NW_IMS_MODE_TYPE_T  ims_mode;
                 printf("please input you want set ims mode (0: off;  1: volte enable \n");
                 scanf("%d", &ims_mode);
                 ret = qser_nw_set_ims_enable(h_nw, ims_mode);
                 printf("qser_nw_set_ims_enable %d ret = %dn",ims_mode, ret);
                 break;
             }       
             case 11://"qser_nw_get_ims_reg_status"
             {
                 QSER_NW_IMS_REG_STATUS_INFO_T         t_info;
                 memset(&t_info, 0, sizeof(QSER_NW_IMS_REG_STATUS_INFO_T));
                 ret = qser_nw_get_ims_reg_status(h_nw, &t_info);
                 printf("qser_nw_get_ims_reg_status ret = %d, detail info:\n", ret);
                 if(ret == 0)
                 {
                     printf("ims_registration: registration_state=%d\n",                          
                         t_info.registration_state);
                 }                
                 break;
             }                 
             default:
             {
                 show_group_help(&t_nw_test);
             }
        }
    }
    return 0;
}

#ifdef __cplusplus
}
#endif




