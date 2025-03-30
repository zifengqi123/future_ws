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
#include "ql/ql_sms.h"


int main(int argc, char *argv[])
{
    char operator[10];
    char serNum[50] = {0};
    char phonenumter[20] ={0};
    int opt;
    int ret, uToken; 

    while(1)
    {
        printf("=========audio main=========\n"
            "\t0 exit\n"
            "\t1 sms init\n"
            "\t2 send sms\n"
            "\t3 wait receive new sms\n"
            "\t4 delete sms(int index);\n"
            "\t5 list sms\n"
            "\t6 query sms storage status\n"
            "\t7 query service number\n"
            "\t8 set service number\n"
            "\t9 deinit sms\n"
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
            ql_sms_init();
            break;
        case 2:
            ql_sms_send_text_msg("+8615775690697", "hello world", 1);
            break;
        case 3:
            ql_sms_add_event_handler(NULL, NULL);
            break;
		case 4:
            ql_sms_send_pdu_msg("+8615775690697","你好",1);
            break;
       //     printf("please input volume (0~100): \n");
       //     fgets(operator, sizeof(operator), stdin);
       //     fflush(stdin);
       //     opt = atoi(operator);
		//	lynq_delete_sms(opt);
            break;
		case 5:
			printf("please input index (0~50): \n");
            fgets(operator, sizeof(operator), stdin);
            fflush(stdin);
            opt = atoi(operator);
       //     lynq_list_sms(1, opt, "ALL" );
			break;
		case 6:
        //    lynq_query_sms_storage_status();
			break;
        case 7:
            ret = ql_sms_get_sms_center_address(serNum);
            if(!ret)
                printf("get_smsc:%s\n", serNum);
            break;
        case 8:
        //    printf("please input service num: \n");
        //    fgets(phonenumter, sizeof(phonenumter), stdin);
        //    fflush(stdin);
        //    memcpy(phonenumter, "+8613800280500", "+8613800280500");
            
        //    ret= ql_sms_set_sms_center_address(phonenumter);
            ret= ql_sms_set_sms_center_address("+8613800280500");
            break;
        case 9:
            ql_sms_release();
            break;
        default:
            break;
        }
    }

    return 0;
}
