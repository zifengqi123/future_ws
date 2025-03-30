#include "lynq-irq.h"

#include "mbtk_type.h"
#include "mbtk_info_api.h"

int line;

static void irq_test_handler(void)
{
    int trig_type;
    trig_type = lynq_irq_get_type(line);
    printf("this is irq_test_handler\nthis irq is gpio %d,trig_type is %d\n", line_gpio[line-117],trig_type);
    return NULL;
}

int main(int argc, char *argv[])
{
    char operator[10];
    int opt;
    int irq, trig_type, en;
    int ret;
    irq_handler handler;
    mbtk_log_init("radio", "IRQ_TEST");

    printf("=========irq main=========\n"
    "\t0 exit\n"
    "\t1 irq install\n"
    "\t2 irq set_type\n"
    "\t3 irq get_type\n"
    "\t4 irq set_wake\n"
    "\t5 irq get_wake\n"
    "\t6 irq uninstall\n"
    "operator: >> \n");

    while(1)
    {

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
                printf("input>> irq \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                irq = atoi(operator);
                printf("input>> trig_type \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                trig_type = atoi(operator);
                ret = lynq_irq_install(irq, irq_test_handler, trig_type);
                if (ret != 0)
                {
                    printf("lynq_irq_install fail\n");
                }
                else
                {
                    printf("lynq_irq_install success\n");
                    line = irq;
                }
            }
            break;
        case 2:
            {
                printf("input>> trig_type \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                trig_type = atoi(operator);
                ret = lynq_irq_set_type(line, trig_type);
                if (ret != 0)
                {
                    printf("lynq_irq_set_type fail\n");
                }
                else
                {
                    printf("lynq_irq_set_type success\n");
                }
            }
            break;
        case 3:
            {
                printf("input>> irq \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                irq = atoi(operator);
                ret = lynq_irq_get_type(irq);
                if (ret < 0)
                {
                    printf("lynq_irq_get_type fail\n");
                }
                else
                {
                    printf("lynq_irq_get_type success  trig_type =%d\n",ret);
                }
             }
            break;
        case 4:
            {
                printf("input>> irq \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                irq = atoi(operator);
                printf("input>> en \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                en = atoi(operator);
                ret = lynq_irq_set_wake(irq, en);
                if (ret != 0)
                {
                    printf("lynq_irq_set_wake fail\n");
                }
                else
                {
                    printf("lynq_irq_set_wake success\n");
                }
            }
            break;
        case 5:
            {
                printf("input>> irq \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                irq = atoi(operator);
                ret = lynq_irq_get_wake(irq);
                if (ret < 0)
                {
                    printf("lynq_irq_get_wake fail\n");
                }
                else
                {
                    printf("lynq_irq_get_wake success  en = %d\n", ret);
                }
            }
            break;
        case 6:
            {
                printf("input>> irq \n");
                memset(operator,0x00, sizeof(operator));
                fgets(operator, sizeof(operator)-1, stdin);
                fflush(stdin);
                irq = atoi(operator);
                ret = lynq_irq_uninstall(irq);
                if (ret != 0)
                {
                    printf("lynq_irq_uninstall fail\n");
                }
                else
                {
                    printf("lynq_irq_uninstall success\n");
                }
            }
            break;
        default:
            break;
        }

    }

    return 0;
}


