#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "lynq_deflog.h"

int main(int argc, char *argv[])
{
    int err, value;
    int cmdIdx = 0;
    int ret = 0;
    int i = 0;
    char operator[10];
    int opt = 0;
    const char * module_name = "MBTK_QL_TEST";
    log_level_enum *level = -1;

    lynq_log_configuration_init("MBTK_QL_TEST");

    while(1)
    {

        printf("=========log========\n"
            "\t 0 lynq_syslog_set_file_size\n"
            "\t 1 lynq_syslog_get_file_size\n"
            "\t 2 lynq_syslog_set_file_rotate\n"
            "\t 3 lynq_syslog_get_file_rotate\n"
            "\t 4 lynq_set_log_level\n"
            "\t 5 lynq_get_log_level\n"
            "\t 6 lynq_notify_recalc_log_level\n"
            "\t 7 test write log\n"
            "\t 8 EXIT \n"
            "=========================\n");
        
        fflush(stdin);
        fgets(operator, sizeof(operator), stdin);
        opt = atoi(operator);

        switch(opt)
        {
            case 0://"lynq_syslog_set_file_size"
            {
                //fgets(arg_string, MAX_INPUT, stdin);
                printf("please input size:\n");
                scanf("%d", &value);
                printf("Input value is %d\n",value);
                ret = lynq_syslog_set_file_size(value);
                if(ret < 0)
                {
                    printf("lynq_syslog_set_file_size failed, ret=%d\n", ret);
                }

                break;
            }
            case 1://"lynq_syslog_get_file_size"
            {
                //ret = demo_lynq_syslog_get_file_size();
                ret = lynq_syslog_get_file_size();
                printf("lynq_syslog_get_file_size ret:%d!\n", ret);

                break;
            }
            case 2://"lynq_syslog_set_file_rotate"
            {
                printf("please input count:\n");
                scanf("%d", &value);
                printf("Input value is %d\n",value);
                ret = lynq_syslog_set_file_rotate(value);
                if(ret < 0)
                {
                    printf("lynq_syslog_set_file_rotate failed, ret=%d\n", ret);
                }

                break;
            }
            case 3://"lynq_syslog_get_file_rotate"
            {
                ret = lynq_syslog_get_file_rotate();
                printf("lynq_syslog_get_file_rotate ret:%d!\n", ret);

                break;
            }
            case 4://"lynq_set_log_level"
            {
                printf("please input level:\n");
                scanf("%d", &value);
                ret = lynq_set_log_level(module_name, value);
                printf("lynq_set_log_level ret:%d!\n", ret);

                break;
            }
            case 5://"lynq_get_log_level"
            {
                ret = lynq_get_log_level(module_name, &level);
                printf("lynq_get_log_level ret:%d, level:%d!\n", ret, level);

                break;
            }
            case 6://test write log
            {
                ret = lynq_notify_recalc_log_level(0);
                printf("lynq_notify_recalc_log_level ret: %d!\n", ret);
                break;
            }
            case 7://test write log
            {
                puts(lynq_read_log_version());
                for(i=0;i<10;i++)
                {
                    LYVERBLOG("LYNQ: Test LYVERBLOG\n");
                    LYERRLOG("LYNQ: Test LYERRLOG\n");
                    LYWARNLOG("LYNQ: Test LYWARNLOG\n");
                    LYINFLOG("LYNQ: Test LYINFLOG\n");
                    LYDBGLOG("LYNQ: Test LYDBGLOG\n");
                    lynq_log_global_output(LOG_VERBOSE, "LOG_VERBOSE test");
                    lynq_log_global_output(LOG_ERROR, "LOG_ERROR test");
                    lynq_log_global_output(LOG_WARNING, "LOG_WARNING test");
                    lynq_log_global_output(LOG_INFO, "LOG_INFO test");
                    lynq_log_global_output(LOG_DEBUG, "LOG_DEBUG test");
                    lynq_log_global_output(LOG_UNSET, "LOG_UNSET test");
                    lynq_log_global_output(LOG_LEVEL_MAX, "LOG_LEVEL_MAX test");
                    lynq_log_debug("LYNQ: test lynq_log_debug");
                    lynq_log_verbose("LYNQ: test lynq_log_verbose");
                    lynq_log_error("LYNQ: test lynq_log_error");
                    lynq_log_info("LYNQ: test lynq_log_info");
                    lynq_log_warning("LYNQ: test lynq_log_warning");
                }

                printf("test write log end\n");

                break;
            }
            default://EXIT
            {
                printf("break\n");
                return 0;
            }
        }
    }

    return 0;
}

