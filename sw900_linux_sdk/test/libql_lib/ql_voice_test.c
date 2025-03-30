#include "mbtk_type.h"
//#include "ql/ql_vcall.h"
//#include "ql/ql_mcm_call.h"
#include "ql/ql_voice.h"




int main(int argc, char *argv[])
{
    char operator[10] = {0};
    int opt;
    int    ret     = 0;
    char   phoneNum[32] = {0};
    int    h_voice     = 0;

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
            ret = ql_voice_call_init();
            if(ret < 0)
            {
                printf("ql_voice_call_init FAIL.	ret:%d\n",ret);
                return -1;
            }
            printf("ql_voice_call_init ret = %d, with h_voice=%d\n", ret, h_voice);

            break;
        case 2:
            ret = ql_voice_call_event_register(NULL, NULL);
            if(ret < 0)
            {
                printf("ql_voice_call_event_register FAIL.		ret:%d\n",ret);
                return -1;
            }
            break;
        case 3:
            ql_voice_call_start("15775690697");
            break;
        case 4:
            ql_voice_call_end();
            break;
        case 5:
            ql_voice_call_answer();
            break;
        case 6:
            ql_voice_auto_answer(6000);
            break;
        case 7:
            ql_voice_call_hold();
            break;
        case 8:
            ql_voice_call_unhold();
            break;
        case 9:
            ql_voice_call_release();
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

