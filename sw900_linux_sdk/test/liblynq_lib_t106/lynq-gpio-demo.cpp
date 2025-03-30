#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <lynq/lynq-gpio.h>

static void printUsage(void)
{
    printf("-ds  [direction]                    --lynq_gpio_direction_set\n");
    printf("-vs  [value]                        --lynq_gpio_value_set\n");
    printf("-ps  [pullsel]                      --lynq_gpio_pullsel_set\n");
    printf("-vg                                --lynq_gpio_value_get\n");
    printf("-pg                                 --lynq_gpio_pullsel_get\n");
    printf("-quit\n");
    printf("Please input an cmd:");
}




int main(int argc,char** argv)
{
    int ret;
    int gpio;
    int direction;
    int value;
    int pullsel;
    char cmd[16];
    char *cmd2;
    char *cmd1;

    if(argc != 2)
    {
        printf("wrong input format, please inout lynq-irq-demo <gpio>\n");
        return -1;
    }
    gpio = atoi(argv[1]);
    printf("gpio is %d\n", gpio);
    ret = lynq_gpio_init(gpio, 0, 0, 0);
    if(ret != 0)
    {
        printf("lynq_gpio_init fail\n");
        return -1;
    }
    else
    {
        printf("lynq_gpio_init success\n");
    }

    while(1)
    {
        printUsage();
        memset(cmd,0,sizeof(cmd));
        fgets(cmd, sizeof(cmd), stdin);
        printf("cmd:%s\n",cmd);
        cmd1 = strtok(cmd, " ");
        cmd2 = strtok(NULL, " ");
        if(strcmp(cmd1,"-ds") == 0)
        {
            if(cmd2 == NULL)
            {
                printf("direction is NULL\n");
                continue;
            }
            direction = atoi(cmd2);
            ret = lynq_gpio_direction_set(gpio, direction);
            if(ret != 0)
            {
                printf("lynq_gpio_direction_set fail\n");
            }
            else
            {
                printf("lynq_gpio_direction_set success\n");
            }
        }

        else if(strcmp(cmd1,"-vs") == 0)
        {
            if(cmd2 == NULL)
            {
                printf("wake_state is NULL\n");
                continue;
            }
            value = atoi(cmd2);
            ret = lynq_gpio_value_set(gpio, value);
            if(ret < 0)
            {
                printf("lynq_gpio_value_set fail\n");
            }
            else
            {
                printf("lynq_gpio_value_set success\n");
            }
        }

        else if (strcmp(cmd1,"-ps") == 0)
        {
            if(cmd2 == NULL)
            {
                printf("pullsel is NULL\n");
                continue;
            }
            pullsel = atoi(cmd2);
            ret = lynq_gpio_pullsel_set(gpio, pullsel);
            if(ret != 0)
            {
                printf("lynq_gpio_pullsel_set fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("lynq_gpio_pullsel_set success\n");
            }
        }
        else if (strcmp(cmd1,"-vg\n") == 0)
        {
            ret = lynq_gpio_value_get(gpio);
            if(ret < 0)
            {
                printf("lynq_gpio_value_get fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("lynq_gpio_value_get success\n");
                printf("ret=%d\n", ret);
            }
        }
        else if(strcmp(cmd1,"-pg\n") == 0)
        {
            ret = lynq_gpio_pullsel_get(gpio);
            if(ret < 0)
            {
                printf("lynq_gpio_pullsel_get fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("lynq_gpio_pullsel_get success\n");
                printf("ret=%d\n", ret);
            }
        }
        else if(strcmp(cmd1,"-quit\n") == 0)
        {
            break;
        }
        else
        {
            printf("wrong input format\n");
        }


    }


    ret = lynq_gpio_deinit(gpio);
    if(ret != 0)
    {
        printf("lynq_gpio_deinit fail\n");
        printf("ret=%d\n", ret);
    }
    else
    {
        printf("lynq_gpio_deinit success\n");
    }

    return 0;

}
