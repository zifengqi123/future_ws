#include "lynq_qser_network.h"
#include "mbtk_type.h"
#include "mbtk_info_api.h"

#include <stdio.h>
#include <stdlib.h>


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
                printf("ind is NULL\n");
                
                break;
            }
        default:
            break;
    }
}



int main(int argc, char *argv[])
{
    char operator[10];
    int opt;
    int lv_voll = 0;
    int ret;
    nw_client_handle_type handle = -1;
    mbtk_log_init("radio", "NW_TEST");

    printf("=========network main=========\n"
        "\t0 exit\n"
        "\t1 network init\n"
        "\t2 network add rx msg handle\n"
        "\t3 network band set config\n"
        "\t4 network get operator name\n"
        "\t5 network get reg status\n"
        "\t6 network get signal strength\n"
        "\t7 network set oos config\n"
        "\t8 network get oos config\n"
        "\t9 network set rf config\n"
        "\t10 network get rf config\n"
        "\t11 network set ims config\n"
        "\t12 network get ims config\n"
        "\t13 network deinit\n"
        "operator: >> ");

    while(1)
    {

        opt = -1;
        fgets(operator, sizeof(operator), stdin);
        fflush(stdin);
        opt = atoi(operator);
        switch (opt)
        {
        case 0:
            printf("main exit\n");
            return 0;
        case 1:
            ret = qser_nw_client_init(&handle);
            printf("qser_nw_client_init ret = %d\n", ret);
            break;
        case 2:
            ret = qser_nw_add_rx_msg_handler(handle, nw_event_ind_handler, NULL);
            printf("qser_nw_add_rx_msg_handler ret = %d\n", ret);
            break;
        case 3:
            {
                //漫游开关现在不支持，所以就不用配置
                printf("please input \n"
                        "\t1:GSM \n"
                        "\t2:WCDMA \n"
                        "\t4:CDMA \n"
                        "\t8:EVDO \n"
                        "\t16:LTE \n"
                        "\t32:TDSCDMA \n"
                );
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                opt = atoi(operator);
                QSER_NW_CONFIG_INFO_T t_info;
             
                t_info.preferred_nw_mode = opt;
                ret = qser_nw_set_config(handle, &t_info);
                printf("qser_nw_set_config ret = %d\n", ret);
            }
            break;
        case 4:
            {
                QSER_NW_OPERATOR_NAME_INFO_T t_info;
                int err = qser_nw_get_operator_name(handle, &t_info);
                if(err)
                {
                    printf("Error : %d\n", err);
                } else
                {
                    printf("Operator : %s, %s, %s, %s\n", t_info.long_eons, t_info.short_eons, t_info.mcc, t_info.mnc);
                }
            }
            break;
        case 5:
            {
                QSER_NW_REG_STATUS_INFO_T t_info;
                int err = qser_nw_get_reg_status(handle, &t_info);
                if(err)
                {
                    printf("Error : %d\n", err);
                }
                else
                {
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

                    //printf("Data Reg: radio_tech = %d, lac = %x, cid = %x\n", reg.data_registration_details_3gpp.radio_tech, reg.data_registration_details_3gpp.lac, reg.data_registration_details_3gpp.cid);
                    //printf("Voice Reg: radio_tech = %d, lac = %x, cid = %x\n", reg.voice_registration_details_3gpp.radio_tech, reg.voice_registration_details_3gpp.lac, reg.voice_registration_details_3gpp.cid);
                }
            }
            break;
        case 6:
            {
                QSER_NW_SIGNAL_STRENGTH_INFO_T t_info;
                int err = qser_nw_get_signal_strength(handle, &t_info);
                if(err)
                {
                    printf("Error : %d\n", err);
                }
                else
                {
                    printf("qser_nw_get_signal_strength, ret=%d\n", err);
                    if(err==0)
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

                }
            }
            break;
        case 7:
            {
                QSER_NW_OOS_CONFIG_INFO_T pt_info_s;
                memset(&pt_info_s, 0x00, sizeof(QSER_NW_OOS_CONFIG_INFO_T));
                /*取值范围0-255,设置时间自定义
                phase 1   5次       次数无法更改，这五次都是以10秒为一次的间隔搜网
                phase 2   5次      次数无法更改，这五次都是以15秒为一次的间隔搜网
                phase 3   不限制次数的秒数设置，第三阶段不限制次数以20秒为一次的间隔搜网
                如果要关闭OOS配置，则把min step max 全部配置为0即可
                */
                int p1,p2,p3;
                printf("input phase 1 number:\n");
                memset(operator, 0x0, 3);
                fgets(operator, 3, stdin);
                fflush(stdin);
                p1 = atoi(operator);
                printf("phase 1 number:%s\n", operator);

                printf("input phase 2 number:\n");
                memset(operator, 0x0, 3);
                fgets(operator, 3, stdin);
                fflush(stdin);
                p2 = atoi(operator);
                printf("phase 2 number:%s\n", operator);

                printf("input phase 3 number:\n");
                memset(operator, 0x0, 3);
                fgets(operator, 3, stdin);
                fflush(stdin);
                p3 = atoi(operator);
                printf("phase 3 number:%s\n", operator);

                pt_info_s.type = QSER_NW_OOS_CFG_TYPE_FULL_BAND_SCAN;//平台提供自定义配置搜网时间间隔
                {
                    pt_info_s.u.full_band_scan_info.t_min = p1;//phase 1
                    pt_info_s.u.full_band_scan_info.t_step = p2;//phase 2
                    pt_info_s.u.full_band_scan_info.t_num = 0;//可以不配置，并没有使用
                    pt_info_s.u.full_band_scan_info.t_max = p3;//phase 3
                }
                printf("set OOS %d %d %d \n",pt_info_s.u.full_band_scan_info.t_min, pt_info_s.u.full_band_scan_info.t_step, pt_info_s.u.full_band_scan_info.t_max);
                //pt_info_s.type = QSER_NW_OOS_CFG_TYPE_FAST_SCAN;平台本身有历史频点优先处理的逻辑（无接口不需要我们进行处理）
                ret = qser_nw_set_oos_config(handle, &pt_info_s);
                printf("qser_nw_set_oos_config ret = %d\n", ret);
            }
            break;
        case 8:
            {
                QSER_NW_OOS_CONFIG_INFO_T pt_info_g;
                qser_nw_get_oos_config(handle, &pt_info_g);
                if (pt_info_g.type == QSER_NW_OOS_CFG_TYPE_FULL_BAND_SCAN)
                    printf("TYPE: %d min: %d setp: %d max: %d\n", pt_info_g.type, pt_info_g.u.full_band_scan_info.t_min, pt_info_g.u.full_band_scan_info.t_step, pt_info_g.u.full_band_scan_info.t_max);
                else
                    printf("type not supported \n");
            }
            break;
        case 9:
            {
                printf("please input 0 or 1 or 4: \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                opt = atoi(operator);
                printf("rf_mode: [%d]\n",opt);
                ret = qser_nw_set_rf_mode(handle, opt);
                printf("qser_nw_set_rf_mode ret = %d\n", ret);
            }
            break;
        case 10:
            {
                int rf_t;
                qser_nw_get_rf_mode(handle, &rf_t);
                printf(">>>rf_mode = [%d]\n",rf_t);
            }
            break;
        case 11:
            {
                E_QSER_NW_IMS_MODE_TYPE_T  ims_mode;
                printf("please input you want set ims mode (0: off;  1: volte enable \n");
                scanf("%d", &ims_mode);
                ret = qser_nw_set_ims_enable(handle, ims_mode);
                printf("qser_nw_set_ims_enable %d ret = %d\n",ims_mode, ret);
            }
            break;
        case 12:
            {
                QSER_NW_IMS_REG_STATUS_INFO_T         t_info;
                memset(&t_info, 0, sizeof(QSER_NW_IMS_REG_STATUS_INFO_T));
                ret = qser_nw_get_ims_reg_status(handle, &t_info);
                printf("qser_nw_get_ims_reg_status ret = %d, detail info:\n", ret);
                if(ret == 0)
                {
                    printf("ims_registration: registration_state=%d\n", t_info.registration_state);
                }
            }
            break;
        case 13:
            qser_nw_client_deinit(handle);
            break;
        default:
            break;
        }

    }

    return 0;
}

