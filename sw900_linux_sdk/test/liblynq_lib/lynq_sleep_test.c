#include "lynq-qser-autosuspend.h"

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include "mbtk_type.h"
#include "mbtk_log.h"

static void qser_lpm_handler(qser_lpm_edge_t edge_state)
{
    printf("this is qser_lpm_handler, edge_state=%d\n", edge_state);
}


int main(int argc, char *argv[])
{
    char operator[10];
    int opt;
    int lv_voll = 0;
    int fd_t;
    mbtk_log_init("radio", "SLEEP_TEST");

    printf("=========sleep main=========\n"
        "\t0 exit\n"
        "\t1 autosuspend enable\n"
        "\t2 wakelock create\n"
        "\t3 wakelock lock\n"
        "\t4 wakelock unlock\n"
        "\t5 wakelock destroy\n"
        "\t6 whitelist set\n"
        "\t7 whitelist get\n"
        "\t8 lpm init\n"
        "\t9 lpm destroy\n"
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
                printf(">>>>>Input 0 or 1<<<<<\n");
                char tmp_en[4]={0};
                memset(tmp_en, 0x00, sizeof(tmp_en));
                fgets(tmp_en, sizeof(tmp_en)-1, stdin);
                fflush(stdin);

                if (tmp_en[0] == '0' || tmp_en[0] == '1')
                    qser_autosuspend_enable(tmp_en[0]);
                else
                    printf(">>>>>re Input 0 or 1 error<<<<<\n");
            }
            break;
        case 2:
            {
                printf(">>>>>Input name<<<<<\n");
                char t_name[64]={0};
                int len_name;
                char tmp_c[64]={0};
                memset(tmp_c, 0x00, sizeof(tmp_c));
                fgets(tmp_c, sizeof(tmp_c)-1, stdin);
                fflush(stdin);
                len_name = strlen(tmp_c)-1;
                strncpy(t_name, tmp_c, len_name);
                printf(">>>>>name=[%s] name_len=[%d]<<<<<\n",t_name, len_name);

                fd_t = qser_wakelock_create(t_name, len_name);
                if (fd_t == -1)
                {
                    printf(">>>>>Output error =[%d]<<<<<\n",fd_t);
                }
                else
                {
                    printf(">>>>>Output ID NUM =[%d]<<<<<\n",fd_t);
                }
            }
            break;
        case 3:
            {
                printf(">>>>>Input ID NUM<<<<<\n");
                char tmp_l[127]={0};
                fgets(tmp_l, sizeof(tmp_l), stdin);
                fflush(stdin);
                fd_t = atoi(tmp_l);
                fd_t = qser_wakelock_lock(fd_t);
                if (fd_t == -1)
                {
                    printf(">>>>>Output error =[%d]<<<<<\n",fd_t);
                }
                else
                {
                    printf(">>>>>Output succuess<<<<<\n");
                }
            }
            break;
        case 4:
            {
                printf(">>>>>Input ID NUM<<<<<\n");
                char tmp_ul[127]={0};
                memset(tmp_ul, 0x00, sizeof(tmp_ul));
                fgets(tmp_ul, sizeof(tmp_ul)-1, stdin);
                fflush(stdin);
                fd_t = atoi(tmp_ul);
                fd_t = qser_wakelock_unlock(fd_t);
                if (fd_t == -1)
                {
                    printf(">>>>>Output error =[%d]<<<<<\n",fd_t);
                }
                else
                {
                    printf(">>>>>Output succuess<<<<<\n");
                }
            }
            break;
        case 5:
            {
                printf(">>>>>Input ID NUM<<<<<\n");
                char tmp_d[127]={0};
                memset(tmp_d, 0x00, sizeof(tmp_d));
                fgets(tmp_d, sizeof(tmp_d)-1, stdin);
                fflush(stdin);
                fd_t = atoi(tmp_d);
                fd_t = qser_wakelock_destroy(fd_t);
                if (fd_t == -1)
                {
                    printf(">>>>>Output error =[%d]<<<<<\n",fd_t);
                }
                else
                {
                    printf(">>>>>Output succuess<<<<<\n");
                }
            }
            break;
        case 6:
            {
                printf(">>>>>Input whitelish NUM  4位(eg:1010) <<<<<\n");
                char whitelish[16]={0};
                int len_whitelish;
                char tmp[16]={0};
                memset(tmp, 0x00, sizeof(tmp));
                fgets(tmp, sizeof(tmp)-1, stdin);
                fflush(stdin);
                len_whitelish = strlen(tmp)-1;

                strncpy(whitelish, tmp, len_whitelish);
                printf(">>>set tmp =[%s] list = [%s] len_whitelish =[%d]\n",tmp, whitelish,len_whitelish);

                qser_whitelist_set(whitelish);
            }
            break;
        case 7:
            {
                char whitelish_get[10]={0};

                qser_whitelist_get(whitelish_get);
                printf(">>>get list = [%s]\n", whitelish_get);
            }
            break;
        case 8:
            {
                qser_pm_cfg_t *qser_lpm_cfg = NULL;
                qser_lpm_cfg = (qser_pm_cfg_t *)malloc(sizeof(qser_pm_cfg_t));
                qser_lpm_cfg->wakeupin.wakeupin_pin = 50;
                qser_lpm_cfg->wakeupin.wakeupin_edge = E_QL_LPM_FALLING;
                qser_lpm_init(qser_lpm_handler, qser_lpm_cfg);
                free(qser_lpm_cfg);
            }
            break;
        case 9:
            qser_lpm_deinit();
            break;

        default:
            break;
        }

    }

    return 0;
}

