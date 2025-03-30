#include "lynq_systime.h"

#include <stdio.h>
#include <stdlib.h>



int main()
{
    char operator[10];
    int opt;
    time_src_status_s time_src ={0};
    unsigned long ulsec =0;
    int en;

    while(1)
    {
        printf("=========time main=========\n"
            "\t0 exit\n"
            "\t1 ntp_sync_time\n"
            "\t2 modem_time_enable\n"
            "\t3 gnss_time_enable\n"
            "\t4 user_set_time\n"
            "\t5 lynq_sync_time_from_rtc(don't support)\n"
            "\t6 lynq_get_time_src_status\n"
            "\t7 lynq_set_rtc_time(don't support)\n"
            "\t8 lynq_get_rtc_time(don't support)\n"
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
                {
                    printf(">>>>>Input 0 or 1<<<<<\n");
                    memset(operator, 0x00, sizeof(operator));
                    fgets(operator, sizeof(operator)-1, stdin);
                    fflush(stdin);
                    en = atoi(operator);
                    //enable set time from ntp
                    if (en == 0 || en == 1)
                        ntp_sync_time(en);
                    else
                        printf(">>>>>RE Input 0 or 1<<<<<\n");
                }
                break;
            case 2:
                 {
                    printf(">>>>>Input 0 or 1<<<<<\n");
                    memset(operator, 0x00, sizeof(operator));
                    fgets(operator, sizeof(operator)-1, stdin);
                    fflush(stdin);
                    en = atoi(operator);
                    //enable set time from nitz
                    if (en == 0 || en == 1)
                        modem_time_enable(en);
                    else
                        printf(">>>>>RE Input 0 or 1<<<<<\n");
                }
                break;
            case 3:
                 {
                    printf(">>>>>Input 0 or 1<<<<<\n");
                    memset(operator, 0x00, sizeof(operator));
                    fgets(operator, sizeof(operator)-1, stdin);
                    fflush(stdin);
                    en = atoi(operator);
                    //enable set time from gnss
                    if (en == 0 || en == 1)
                        gnss_time_enable(en);
                    else
                        printf(">>>>>RE Input 0 or 1<<<<<\n");
                }
                break;
            case 4:
                //enable set time from user
                user_set_time("2024-01-09", "11:30");
                break;
            case 5:
                // RTC TIME set to system
                lynq_sync_time_from_rtc();
                break;
            case 6:
                //check sysytem type
                lynq_get_time_src_status (&time_src);
                printf("time source status are: \nntp=%d\nnitz=%d\ngnss=%d\n",time_src.ntp,time_src.nitz, time_src.gnss);
                break;
            case 7:
                // system time set to RTC
                lynq_set_rtc_time();
                break;
            case 8:
                // get RTC time
                lynq_get_rtc_time(&ulsec);
                break;
            default:
                break;
        }

        sleep(1);
    }

    return 0;
}




