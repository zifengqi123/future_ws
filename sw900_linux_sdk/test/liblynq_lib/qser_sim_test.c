#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>

#include"lynq_qser_sim.h"


#define BUF_SIZE 32
#define BUF_PIN 16
#define VER_SIZE 128

typedef uint32_t sim_client_handle_type;

sim_client_handle_type  ph_sim = 2023;
sim_client_handle_type  h_sim = 2023;


int main(int argc, char const *argv[])
{
    printf("=========sim main=========\n"
        "\t0 qser_sim_init\n"
        "\t1 qser_get_imsi\n"
        "\t2 qser_get_iccid\n"
        "\t3 qser_get_phonenumber\n"
        "\t4 qser_verify_pin\n"
        "\t5 qser_change_pin\n"
        "\t6 qser_unlock_pin\n"
        "\t7 qser_enable_pin\n"
        "\t8 qser_disable_pin\n"
        "\t9 qser_get_sim_status\n"
        "\t10 qser_get_imei\n"
        "\t11 qser_get_imei_and_sv\n"
        "\t12 qser_reset_modem\n"
        "\t13 qser_get_version\n"
        "\t14 qser_reset_sim\n"
        "\t15 qser_deinit_sim\n"
        "\t16 exit\n");

    while(1)
    {
        int cmdIdx = -1; 
        int res = -1;

        printf("operator: >> \n");

        scanf("%d", &cmdIdx);
        fflush(stdin);

        switch(cmdIdx)
        {
            //"qser_sim_init"
            case 0:
            {
                //int token;
                //printf("input token\n");
                //scanf("%d", &token);
                res = qser_sim_client_init(&h_sim);
                if(res == 0)
                {
                    printf("Run qser_sim_client_init\n");
                }else{
                    printf("qser_sim_client_init error\n");
                }
            }
            break;
            //"qser_sim_getimsi"
            case 1:
            {
                char imsi[BUF_SIZE] = {0};
                QSER_SIM_APP_ID_INFO_T pt_info;
                res = qser_sim_getimsi(h_sim, &pt_info, imsi, BUF_SIZE);
                if(res == 0)
                {
                    printf("imsi is %s!!!\n",imsi);
                }else{
                    printf("get imsi error, res = %d\n", res);
                }
            }
            break;
            //"qser_get_iccid"
            case 2:
            {
                char iccid[BUF_SIZE] = {0};
                res = qser_sim_geticcid(h_sim, QSER_SIM_SLOT_ID_1, iccid, BUF_SIZE);
                if(res == 0)
                {
                    printf("get iccid success!!! iccid is %s\n",iccid);
                }else{
                    printf("get iccid error, res = %d\n", res);
                }
            }
            break;
            //qser_get_phonenumber
            case 3:
            {
                char phonenumber[BUF_SIZE] = "";
                QSER_SIM_APP_ID_INFO_T pt_info;
                res = qser_sim_getphonenumber(h_sim, &pt_info, phonenumber, BUF_SIZE);
                if(res == 0)
                {
                    printf("get phonenumber success!!! phonenumber is %s\n",phonenumber);
                }else{
                    printf("get phonenumber error, res = %d\n", res);
                }
			}
			break;
			//qser_verify_pin
            case 4:
            {
                char pin[BUF_PIN] = {0};
                QSER_SIM_VERIFY_PIN_INFO_T pt_info;
                printf("input pin\n");
                scanf("%s", pin);
                strncpy(pt_info.pin_value, pin, BUF_PIN);
                printf("pin_value = %s , pin = %s\n", pt_info.pin_value, pin);
                pt_info.pin_value_len = strlen(pt_info.pin_value);

                res = qser_sim_verifypin(h_sim, &pt_info);
                if(res == 0)
                {
                    printf("verify pin success!!!\n");
                }else{
                    printf("verify pin error, res = %d\n", res);
                }
            }
            break;
            //qser_change_pin
            case 5:
            {
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
                pt_info.old_pin_value_len = strlen(pt_info.old_pin_value);
                pt_info.new_pin_value_len = strlen(pt_info.new_pin_value);

                res = qser_sim_changepin(h_sim, &pt_info);
                if(res == 0)
                {
                    printf("change pin success!!!\n");
                }else{
                    printf("change pin error, res = %d\n", res);
                }
            }
            break;
            //qser_unlock_pin
            case 6:
            {
                char puk[BUF_PIN] = {0};
                QSER_SIM_UNBLOCK_PIN_INFO_T pt_info;
                printf("input  puk\n");
                scanf("%s", puk);
                char new_pin[BUF_PIN] = {0};
                printf("input new pin\n");
                scanf("%s", new_pin);

                strncpy(pt_info.puk_value, puk, BUF_PIN);
                strncpy(pt_info.new_pin_value, new_pin, BUF_PIN);
                printf("pt_info.puk_value = %s, puk = %s\n", pt_info.puk_value, puk);
                printf("pt_info.new_pin_value = %s, new_pin = %s\n", pt_info.new_pin_value, new_pin);
                pt_info.new_pin_value_len = strlen(pt_info.new_pin_value);
                pt_info.puk_value_len = strlen(pt_info.puk_value);
                    
                res = qser_sim_unblockpin(h_sim, &pt_info);
                if(res == 0)
                {
                    printf("unlock pin success!!!\n");
                }else{
                    printf("unlock pin error, res = %d\n", res);
                }
            }
            break;
            //qser_enable_pin
            case 7:
			{
                char pin[BUF_PIN] = {0};
                QSER_SIM_ENABLE_PIN_INFO_T pt_info;
                printf("input pin\n");
                scanf("%s", pin);
                strncpy(pt_info.pin_value, pin, BUF_PIN);
                pt_info.pin_value_len = strlen(pt_info.pin_value);

                res = qser_sim_enablepin(h_sim, &pt_info);
                if(res == 0)
                {
                    printf("pin enabled!!!\n");
                }else{
                    printf("pin enable error, res =%d\n", res);
                }
            }
            break;
            //qser_disable_pin
            case 8:
            {
                char pin[BUF_PIN] = {0};
                QSER_SIM_ENABLE_PIN_INFO_T pt_info;
                printf("input pin\n");
                scanf("%s", pin);
                strncpy(pt_info.pin_value, pin, BUF_PIN);
                pt_info.pin_value_len = strlen(pt_info.pin_value);
                
                res = qser_sim_disablepin(h_sim, &pt_info);
                if(res == 0)
                {
                    printf("pin disnabled!!!\n");
                }else{
                    printf("pin disable error,res = %d\n", res);
                }
            }
            break;
            //qser_get_sim_status
            case 9:
            {
                QSER_SIM_CARD_STATUS_INFO_T pt_info;

                res = qser_sim_getcardstatus(h_sim, QSER_SIM_SLOT_ID_1, &pt_info);
                if(res == 0)
                {
                    printf("state is %d !!!\n",pt_info.e_card_state);
                }else{
                    printf("get imsi error,res = %d\n", res);
                }
            }
            break;
            //qser_get_imei
            case 10:
            {
                char imei[BUF_SIZE]="";
                res = qser_sim_getimei(h_sim, imei);
                if(res == 0)
                {
                    printf("get imei success!!! imei = %s\n", imei);
                }else{
                    printf("get imei error, res = %d\n", res);
                }
            }
            break;
            //qser_get_imei_and_sv
            case 11:
            {
                char imei[BUF_SIZE]="";
                char sv[BUF_SIZE]="";

                res = qser_get_imei_and_sv(h_sim, imei, sv);
                if(res == 0)
                {
                    printf("get imei and sv success!!!imei = %s sv = %s\n", imei, sv);
                }else{
                    printf("get imei and sv error, res = %d\n", res);
                }
            }
			break;
            //qser_reset_modem
            case 12:
            {
                res = qser_reset_modem(h_sim);
                if(res == 0)
                {
                    printf("reset modem success!!!\n");
                }else{
                    printf("reset modem error, res = %d\n", res);
                }
            }
            break;
            //qser_get_version
            case 13:
            {
                char buf[VER_SIZE]="";

                res = qser_get_version(h_sim, buf);
                if(res == 0)
                {
                    printf("get version success!!! ver = %s\n", buf);
                }else{
                    printf("get version error, res = %d\n", res);
                }
            }
            break;
            //qser_reset_sim
            case 14:
            {
                res = qser_reset_sim(h_sim);
                if(res == 0)
                {
                    printf("reset sim success!!!\n");
                }else{
                    printf("reset sim error, res = %d\n", res);
                }
            }
            break;
            //qser_deinit_sim
            case 15:
            {
                res = qser_sim_client_deinit(h_sim);
                if(res == 0)
                {
                    printf("sim deinit success is!!!\n");
                }else{
                    printf("sim deint errors, res = %d\n", res);
                }
            }
            break;
            case 16:
                printf("main exit\n");
                return 0;
            default :
                break;
        }
            
    }
    
   return 0;
}
