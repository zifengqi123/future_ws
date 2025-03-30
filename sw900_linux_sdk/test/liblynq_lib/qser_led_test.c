#include <stdio.h>
#include <strings.h>
#include<stdlib.h>

#include "lynq_led.h"

void user_help(void)
{
    printf("\t-1 exit\n"
           "\t1 open net led\n"
           "\t2 close net led \n"
           "\t3 open status led\n"
           "\t4 close status led\n"
           "please input operator: >> \n");
}


int main(int argc, const char *argv[])
{
    int ret;
    int opt = 0;
    while(1)
    {
        printf("=========gnss main=========\n");
        user_help();
        scanf("%d", &opt);
        switch (opt)
        {
            case -1:
            {
                printf("main exit\n");
                return 0;
            }
            case 1:
            {
                ret = lynq_set_netled_on(1);
                if(ret)
                {
                    printf("[%s-%d] lynq_set_netled_on error\n", __FUNCTION__, __LINE__);
                    return -1;
                }
                else
                {
                    printf("open net led success.\n");
                }
                break;
            }
            case 2:
            {
                ret = lynq_set_netled_on(0);
                if(ret)
                {
                    printf("[%s-%d] lynq_set_netled_on error\n", __FUNCTION__, __LINE__);
                    return -1;
                }
                else
                {
                    printf("close net led success.\n");
                }
                break;
            }
            case 3:
            {
                ret = lynq_set_statusled_on(1);
                if(ret)
                {
                    printf("[%s-%d] lynq_set_statusled_on error\n", __FUNCTION__, __LINE__);
                    return -1;
                }
                else
                {
                    printf("open status led success.\n");
                }
                break;
            }
            case 4:
            {
                ret = lynq_set_statusled_on(0);
                if(ret)
                {
                    printf("[%s-%d] lynq_set_statusled_on error\n", __FUNCTION__, __LINE__);
                    return -1;
                }
                else
                {
                    printf("close status led success.\n");
                }
                break;
            }
            default:
            {
                printf("opt unkuown\n");
                break;
            }
        }
    }
    
    return 0;
}

