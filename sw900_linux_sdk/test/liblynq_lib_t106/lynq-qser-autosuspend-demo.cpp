#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <lynq/lynq-qser-autosuspend.h>

#define FILE_LOCK_TABLE "/tmp/.lock_table"

#define MAX_LOCK_NUM 128

static void qser_lpm_handler(qser_lpm_edge_t edge_state)
{
    printf("this is qser_lpm_handler, edge_state=%d\n", edge_state);

}

static void printUsage(void)
{
    printf("-i                  --qser_lpm_init\n");
    printf("-d                  --qser_lpm_deinit\n");
    printf("-q                  --quit\n");
    printf("-e                  --qser_autosuspend_enable\n");
    printf("-cl [wakelock name] --qser_wakelock_create \n");
    printf("-al [wakelock num]  --qser_wakelock_lock \n");
    printf("-rl [wakelock num]  --qser_wakelock_unlock \n");
    printf("-dl [wakelock num]  --qser_wakelock_destroy\n");
    printf("-ws [whitelist num] --qser_whitelist_set\n");
    printf("-wg                 --qser_whitelist_get\n");
    printf("-ccl                --check created locks\n");
    printf("-cll                --check lockup locks\n");
    printf("-cws                --check wakeup sources\n");
    printf("Please input an cmd:");
}


int check_lock(void)
{
    int err;
    int file_fd;
    int i;
    int ret;
    LOCK_TABLE lock_status;
    file_fd = open(FILE_LOCK_TABLE,O_RDWR);
    if(file_fd < 0)
    {
        err = errno;
        printf("Error open lock_table file:%s\n", strerror(errno));
        return -2;
    }

    memset(&lock_status, 0, sizeof(lock_status));
    lseek(file_fd,0,SEEK_SET);
    ret = read(file_fd,(unsigned char *)&lock_status,sizeof(lock_status));
    if(ret <= 0)
    {
        close(file_fd);
        return -2;
    }
    for(i=0;i<MAX_LOCK_NUM;i++)
    {
        if(strlen(lock_status.lock_name[i]) != 0)
        {
            printf("fd: %d lock_name:%s strlen:%d, pid=%d\n", i, lock_status.lock_name[i], strlen(lock_status.lock_name[i]), lock_status.lock_pid[i]);
        }
    }

    close(file_fd);
    return 0;

}


void delete_enter(char *data)
{
    char *find = strchr(data, '\n');
    if(find)
        *find = '\0';
    return ;
}

int main(int argc,char** argv)
{

    int num;
    int ret;
    int len;
    FILE *fp;
    char buf[256];
    char cmd[64];
    char *cmd2;
    char *cmd1;
    char tmp[8];
    qser_pm_cfg_t *qser_lpm_cfg = NULL;
    qser_lpm_cfg = (qser_pm_cfg_t *)malloc(sizeof(qser_pm_cfg_t));
    qser_lpm_cfg->wakeupin.wakeupin_pin = 50;
    qser_lpm_cfg->wakeupin.wakeupin_edge = E_QL_LPM_FALLING;
    while(1)
    {
        printUsage();
        memset(cmd,0,sizeof(cmd));
        fgets(cmd, sizeof(cmd), stdin);
        cmd1 = strtok(cmd, " ");
        cmd2 = strtok(NULL, " ");
        delete_enter(cmd1);
        printf("cmd1:%s\n", cmd1);
        if(strcmp(cmd1,"-i") == 0)
        {
            ret = qser_lpm_init( qser_lpm_handler, qser_lpm_cfg);
            if(ret != 0)
            {
                printf("lpm init fail\n");
                break;
            }
            else
            {
                printf("lpm init success\n");
            }
        }
        else if(strcmp(cmd1,"-e") == 0)
        {
            char num='1';
            ret = qser_autosuspend_enable(num);
            if(ret != 0)
            {
                printf("qser_autosuspend_enable fail\n");
            }
            else
            {
                printf("qser_autosuspend_enable success\n");
            }
        }
        else if(strcmp(cmd1,"-cl") == 0)
        {
            if(cmd2 == NULL)
            {
                printf("name is null\n");
                continue;
            }
            len = strlen(cmd2);
            printf("len =%d\n", len);
            ret = qser_wakelock_create(cmd2, len);
            if(ret < 0)
            {
                printf("wakelock create fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("wakelock create success\n");
                printf("fd=%d\n", ret);
            }
        }
        else if (strcmp(cmd1,"-al") == 0)
        {
             if(cmd2 == NULL)
            {
                printf("fd is null\n");
                continue;
            }
            num = atoi(cmd2);
            ret = qser_wakelock_lock(num);
            if(ret != 0)
            {
                printf("wakelock lock fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("wakelock lock success\n");
            }
        }

        else if (strcmp(cmd1,"-rl") == 0)
        {
            if(cmd2 == NULL)
            {
                printf("fd is null\n");
                continue;
            }
            num = atoi(cmd2);
            ret = qser_wakelock_unlock(num);
            if(ret != 0)
            {
                printf("wakelock unlock fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("wakelock unlock success\n");
            }
        }
        else if(strcmp(cmd1,"-dl") == 0)
        {
            if(cmd2 == NULL)
            {
                printf("fd is null\n");
                continue;
            }
            num = atoi(cmd2);
            ret = qser_wakelock_destroy(num);
            if(ret != 0)
            {
                printf("wakelock destroy fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("wakelock destroy success\n");
            }
        }
        else if(strcmp(cmd1, "-ws") == 0)
        {
            if(cmd2 == NULL)
            {
                printf("num is null\n");
                continue;
            }
            delete_enter(cmd2);
            ret = qser_whitelist_set(cmd2);
            if(ret != 0)
            {
                printf("qser_whitlist_set fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("qser_whitlist_set success\n");
            }
        }
        else if(strcmp(cmd1, "-wg") == 0)
        {
            ret = qser_whitelist_get(tmp);
            if(ret != 0)
            {
                printf("qser_whitlist_set fail\n");
            }
            else
            {
                printf("qser_whitlist_set success\n");
                printf("ret=%d, whilelist state is %s\n", ret, tmp);
            }
        }
        else if(strcmp(cmd1,"-ccl") == 0)
        {
            int ret;
            ret = check_lock();
            if(ret != 0)
            {
                printf("check lock fail\n");
                printf("ret=%d\n", ret);
            }
            else
            {
                printf("check lock success\n");
            }
        }
        else if(strcmp(cmd1,"-cll") == 0)
        {
            int ret;
            ret = system("cat /sys/power/wake_lock");
            if(ret != 0)
            {
                printf("check created lock fail\n");
            }
        }
        else if(strcmp(cmd1,"-cws") == 0)
        {
            printf("start check active wakeup_sources  !!!\n");
            memset(buf,0,sizeof(buf));
            fp = popen("cat /sys/kernel/debug/wakeup_sources|sed -e 's/\"^ \"/\"unnamed\"/g' | awk '{print $6 \"\t\" $1}'| grep -v \"^0\" |sort -n \n","r");
            while(fgets(buf, 255, fp) != NULL)
            {
                printf("%s", buf);
            }
            pclose(fp);
        }
        else if(strcmp(cmd1, "-d") == 0)
        {
            ret = qser_lpm_deinit();
            if(ret != 0)
            {
                printf("lpm deinit fail\n");
            }
            else
            {
                printf("lpm deinit success\n");
            }
        }
        else if(strcmp(cmd1,"-q") == 0)
        {
            break;
        }
    }

    return 0;

}
