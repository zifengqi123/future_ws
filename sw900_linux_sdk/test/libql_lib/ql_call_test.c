#include "mbtk_type.h"
#include "ql/ql_at.h"
#include "ql/ql_vcall.h"
#include "ql/ql_mcm_call.h"

typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;

//for server test
st_api_test_case at_api_testcases[] =
{
    {0,   "print_help"},
    {1,   "QL_Voice_Call_Ecall"},
    {2,   "QL_Voice_Call_Ecall_HangUp"},

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

static void ql_voice_call_ind_func(unsigned int ind_id,
                                   void* ind_data,
                                   uint32_t ind_data_len)
{
    if(NULL == ind_data)
    {
        return;
    }

    switch(ind_id)
    {
        case E_QL_MCM_VOICE_CALL_IND:
        {
            if(ind_data_len != sizeof(ql_mcm_voice_call_ind))
            {
                break;
            }

            ql_mcm_voice_call_ind *pVoiceCallInd = (ql_mcm_voice_call_ind*)ind_data;

            char *call_state[] = {"INCOMING", "DIALING", "ALERTING", "ACTIVE", "HOLDING", "END", "WAITING"};

            int i = 0;
            for(i = 0; i < pVoiceCallInd->calls_len; i++)
            {
                printf("######### Call id=%d, PhoneNum:%s, event=%s!  ######\n",
                            pVoiceCallInd->calls[i].call_id, pVoiceCallInd->calls[i].number, call_state[pVoiceCallInd->calls[i].state]);
            }

            break;
        }

        case E_QL_MCM_VOICE_ECALL_STATUE_IND:
        {
            if(ind_data_len != sizeof(ql_mcm_voice_ecall_status_ind))
            {
                break;
            }

            ql_mcm_voice_ecall_status_ind *pEcallStatusInd
                                           = (ql_mcm_voice_ecall_status_ind*)ind_data;

            if (pEcallStatusInd->ecall_msd_tx_status_valid)
            {
                if (pEcallStatusInd->ecall_msd_tx_status == E_QL_MCM_VOICE_ECALL_MSD_TRANSMISSION_STATUS_SUCCESS)
                {
                    printf("========== Ecall status  call_id =%d ,   ecall msd tx success.\r\n", pEcallStatusInd->call_id);
                }
                else
                {
                    printf("========== Ecall status  call_id =%d ,   ecall msd tx failure.\r\n",  pEcallStatusInd->call_id);
                }
            }
            else
            {
                printf("========== Ecall status  call_id =%d  \r\n", pEcallStatusInd->call_id);
            }

            break;
        }

        case E_QL_MCM_VOICE_UNKOWN_IND:
        default:
            break;
    }
}

#if 0
int main(int argc, char *argv[])
{
    int    cmdIdx  = 0;
    int    ret     = E_QL_OK;
    char   phoneNum[32] = {0};
    voice_client_handle_type    h_voice     = 0;
    int                         voice_call_id = 0;

    printf("QL_Voice_Call_Client_Init .....\n");
    ret = QL_Voice_Call_Client_Init(&h_voice);
    if(ret < 0)
    {
        printf("QL_Voice_Call_Client_Init FAIL.	ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_Client_Init ret = %d, with h_voice=%d\n", ret, h_voice);

    ret = QL_Voice_Call_AddCommonStateHandler(h_voice, (QL_VoiceCall_CommonStateHandlerFunc_t)ql_voice_call_ind_func);
    if(ret < 0)
    {
        printf("QL_Voice_Call_AddCommonStateHandler FAIL.		ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_AddCommonStateHandler ret = %d\n", ret);

    print_help();

    while(1)
    {
        printf("please input cmd index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            break;
        }

        switch(cmdIdx)
        {
        case 0://"print_help"
        {
            print_help();
            break;
        }
       	case 1://"QL_Voice_Call_Ecall"
        {
            char    PhoneNum[32]                  = {0};
            printf("please input dest phone number: \n");
            scanf("%s", PhoneNum);

			char    msd[140+1]                  = {0};
            printf("please input msd content: \n");
            scanf("%s", msd);

            E_QL_MCM_ECALL_VARIANT_T ecall_mode;
            printf("please input ecall mode(1:test 2:emergency): \n");
            scanf("%d", &ecall_mode);
            QL_Voice_Call_Start(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1, PhoneNum, NULL);
            // ret = QL_Voice_Call_Ecall(h_voice, E_QL_VCALL_EXTERNAL_SLOT_1, PhoneNum,
            //                           ecall_mode, &voice_call_id);
			printf(" voice_call_id = %d\n", voice_call_id);
            printf(" ret = %d\n", ret);
            break;
        }
        case 2://QL_Voice_Call_Ecall_HangUp
        {
            ret = QL_Voice_Call_Ecall_HangUp(h_voice);
            printf(" ret = %d\n", ret);
            break;
        }

       default:
            print_help();
            break;
        }
    }

    ret = QL_Voice_Call_RemoveCommonStateHandler(h_voice);
    if(ret < 0)
    {
        printf("QL_Voice_Call_RemoveCommonStateHandler.	ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_RemoveCommonStateHandler ret = %d\n", ret);

    ret = QL_Voice_Call_Client_Deinit(h_voice);
    if(ret < 0)
    {
        printf("QL_Voice_Call_Client_Deinit FAIL.	ret:%d\n",ret);
        return -1;
    }
    printf("QL_Voice_Call_Client_Deinit ret = %d, with h_voice=%d\n", ret, h_voice);

    return 0;
}
#else

int main(int argc, char *argv[])
{
    char operator[10];
    int opt;
    int    ret     = E_QL_OK;
    char   phoneNum[32] = {0};
    voice_client_handle_type    h_voice     = 0;

    while(1)
    {
        printf("=========call main=========\n"
            "\t0 exit\n"
            "\t1 call init\n"
            "\t2 call register handle\n"
            "\t3 call start\n"
            "\t4 call end\n"
            "\t5 call answer\n"
            "\t6 call set auto answer\n"
            "\t7 call hold\n"
            "\t8 call unhold\n"
            "\t9 call deinit\n"
            "operator: >> ");

        fgets(operator, sizeof(operator), stdin);
        fflush(stdin);
        opt = atoi(operator);
        switch (opt)
        {
        case 0:
            printf("main exit\n");
            return 0;
        case 1:
            ret = QL_Voice_Call_Client_Init(&h_voice);
            if(ret < 0)
            {
                printf("QL_Voice_Call_Client_Init FAIL.	ret:%d\n",ret);
                return -1;
            }
            printf("QL_Voice_Call_Client_Init ret = %d, with h_voice=%d\n", ret, h_voice);

            break;
        case 2:
            ret = QL_Voice_Call_AddCommonStateHandler(h_voice, (QL_VoiceCall_CommonStateHandlerFunc_t)ql_voice_call_ind_func);
            if(ret < 0)
            {
                printf("QL_Voice_Call_AddCommonStateHandler FAIL.		ret:%d\n",ret);
                return -1;
            }
            break;
        case 3:
            QL_Voice_Call_Start(h_voice, 0, "15982066434", NULL);
            break;
        case 4:
            QL_Voice_Call_End(h_voice, 0);
            break;
        case 5:
            QL_Voice_Call_Answer(h_voice, 0);
            break;
        case 6:
            QL_Voice_Call_SetAutoAnswer(h_voice, E_QL_MCM_VOICE_AUTO_ANSWER_ENABLE, 6000);
            break;
        case 7:
            QL_Voice_Call_Hold(h_voice);
            break;
        case 8:
            QL_Voice_Call_UnHold(h_voice);
            break;
        case 9:
            QL_Voice_Call_Client_Deinit(h_voice);
            break;
        case 10:
            // QL_Voice_Call_UnHold;
            break;
        default:
            break;
        }

        sleep(1);
    }

    return 0;
}
#endif
