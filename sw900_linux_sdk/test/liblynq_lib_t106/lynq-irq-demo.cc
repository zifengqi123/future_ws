#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <lynq/lynq-irq.h>


#define CMD_SET_WAKE    "set_wake"
#define CMD_GET_WAKE    "get_wake\n"
#define CMD_SET_TYPE    "set_type"
#define CMD_GET_TYPE    "get_type\n"
#define CMD_UNINSTALL   "uninstall\n"
#define CMD_QUIT_TEST   "quit\n"

int line;


static void printUsage(void)
{
    printf("Usage:\n");
    printf("-set_wake <en>\n");
    printf("-set_type <en>\n");
    printf("-get_type\n");
    printf("-get_wake\n");
    printf("-uninstall\n");
    printf("-quit\n");
    printf("Please input an cmd:");
}


static void irq_test_handler(void)
{
    int trig_type;
    trig_type = lynq_irq_get_type(line);
    printf("this is irq_test_handler\nthis irq is gpio %d,trig_type is %d\n", line_gpio[line], trig_type);
//    return NULL;
}


int main(int argc, char** argv)
{
    int ret;
    int irq;
    int trig_type;
    int en;
    char cmd[16];
    char *cmd2;
    char *cmd1;

    if(argc != 3)
    {
        printf("wrong input format, please inout lynq-irq-demo <irq_line> <trig_type/wake_state> \n");
        return -1;
    }
    irq = atoi(argv[1]);
    trig_type = atoi(argv[2]);
    ret = lynq_irq_install(irq, irq_test_handler, trig_type);
    if(ret != 0)
    {
        printf("lynq_irq_install fail\n");
        return 0;
    }
    else
    {
        printf("lynq_irq_install success\n");
        line = irq;
    }

    while(1)
    {
        printUsage();
        memset(cmd,0,sizeof(cmd));
        fgets(cmd, sizeof(cmd), stdin);
        printf("cmd:%s\n",cmd);
        cmd1 = strtok(cmd, " ");
        cmd2 = strtok(NULL, " ");
        if(strcmp(cmd1, CMD_SET_WAKE) == 0)
        {
            if(cmd2 == NULL)
            {
                printf("wake_state is NULL\n");
                break;
            }
            en = atoi(cmd2);
            ret = lynq_irq_set_wake(irq, en);
            if(ret < 0)
            {
                printf("lynq_irq_set_wake fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("lynq_irq_set_wake success\n");
                printf("ret=%d\n", ret);
            }
        }
        else if(strcmp(cmd1, CMD_GET_WAKE) == 0)
        {
            ret = lynq_irq_get_wake(irq);
            printf("lynq_irq_get_wake ret %d\n", ret);
        }
        else if(strcmp(cmd1, CMD_SET_TYPE) == 0)
        {
            if(cmd2 == NULL)
            {
                printf("trig_type is NULL\n");
                break;
            }
            trig_type = atoi(cmd2);
            ret = lynq_irq_set_type(line, trig_type);
            if(ret < 0)
            {
                printf("lynq_irq_set_type fail\n");
            }
            else
            {
                printf("lynq_irq_set_type success\n");
            }
        }
        else if(strcmp(cmd1, CMD_GET_TYPE) == 0)
        {
            ret = lynq_irq_get_type(irq);
            printf("lynq_irq_get_type ret %d\n", ret);
        }
        else if(strcmp(cmd1, CMD_UNINSTALL) == 0)
        {
            ret = lynq_irq_uninstall(irq);
            if(ret != 0)
            {
                printf("lynq_irq_uninstall fail\n");
            }
            else
            {
                printf("lynq_irq_uninstall success\n");
            }
        }
        else if(strcmp(cmd1, CMD_QUIT_TEST) == 0)
        {
            ret = lynq_irq_uninstall(irq);
            if(ret != 0)
            {
                printf("lynq_irq_uninstall fail\n");
            }
            else
            {
                printf("lynq_irq_uninstall success\n");
            }
            break;
        }
        else
        {
            printf("wrong input format\n");
        }
    }

    return 0;
}


