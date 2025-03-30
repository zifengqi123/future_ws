#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "mbtk_gnss.h"
#include "mbtk_log.h"

static void help()
{
    printf("------------------------------------\n");
    printf("open <0-15> timeout(s)\n");
    printf("close timeout(s)\n");
    printf("setting <cmd> timeout(s)\n");
    printf("dl <fw_path> timeout(s)\n");
    printf("ind_set <0-3> timeout(s)\n");
    printf("------------------------------------\n>>");
}

static void gnss_callback(uint32 ind_type, const void* data, uint32 data_len)
{
    if(ind_type == MBTK_GNSS_IND_LOCATION) {
		printf("data_len [%d] mopen_location_info_t[%d]\n", data_len, sizeof(mbtk_gnss_location_info_t));
		if(data_len != sizeof(mbtk_gnss_location_info_t))
		{
			printf("LOC : data size error\n");
			return;
		}
		if(data == NULL)
		{
			printf("LOC : data is NULL\n");
			return;
		}
		mbtk_gnss_location_info_t *locl_info = (mbtk_gnss_location_info_t *)data;
        printf("LOC : timestamp[%ld] bearing[%f] speed[%f] latitude[%f] longitude[%f] altitude[%f]\n", locl_info->timestamp, locl_info->bearing, locl_info->speed, locl_info->latitude, locl_info->longitude, locl_info->altitude);
    } else if(ind_type == MBTK_GNSS_IND_NMEA) {
        printf("NMEA : %s\n", data);
    } else {
        printf("Unknown IND : %d\n", ind_type);
    }
}

static void sig_process(int sig)
{
    LOGI("I got signal %d\n", sig);
    switch(sig)
    {
        case SIGINT: // Ctrl + C
        {
            LOGI("Exit by SIGINT.\n");
            mbtk_gnss_deinit();;
            exit(0);
        }
        case SIGQUIT: // Ctrl + \ (类似 SIGINT ，但要产生core文件)
        {
            LOGI("Exit by SIGQUIT.\n");
            mbtk_gnss_deinit();;
            exit(0);
        }
        case SIGTERM:// 默认kill   (同 SIGKILL ，但 SIGKILL 不可捕获)
        {
            LOGI("Exit by SIGTERM.\n");
            mbtk_gnss_deinit();;
            exit(0);
        }
        case SIGTSTP:// Ctrl + Z (同 SIGSTOP ，但 SIGSTOP 不可捕获)
        {
            LOGI("Exit by SIGTSTP.\n");
            mbtk_gnss_deinit();
            exit(0);
        }
        case SIGSEGV: // 如空指针
        {
            LOGI("Exit by SIGSEGV.\n");
            mbtk_gnss_deinit();
            exit(0);
        }
        default:
        {
            LOGI("Unknown sig:%d\n",sig);
            break;
        }
    }
}


int main(int argc, char *argv[])
{
    signal(SIGINT, sig_process);
    signal(SIGQUIT, sig_process);
    signal(SIGTERM, sig_process);

    mbtk_log_init("radio","GNSS_CLI");

    gnss_err_enum ret = mbtk_gnss_init(gnss_callback);
    if(ret != GNSS_ERR_OK) {
        printf("mbtk_gnss_init() fail : %d", ret);
        return -1;
    }

    char cmd[100];
    while(1)
    {
        help();
        memset(cmd, 0, 100);
        int err;
        if(fgets(cmd, 100, stdin) && strlen(cmd) > 1)
        {
            if(cmd[strlen(cmd) - 1] == '\n') {
                cmd[strlen(cmd) - 1] = '\0';
            }
            if(memcmp(cmd, "open", 4) == 0) {
                int type, timeout;
                if(2 == sscanf(cmd, "open %d %d", &type, &timeout)) {
                    ret = mbtk_gnss_open(type, timeout);
                    if(ret != GNSS_ERR_OK) {
                        printf("mbtk_gnss_open() fail : %d\n", ret);
                    } else {
                        printf("mbtk_gnss_open() success.\n");
                    }
                } else {
                    printf("CMD(%s) error.\n", cmd);
                }
            } else if(memcmp(cmd, "close", 5) == 0) {
                int timeout;
                if(1 == sscanf(cmd, "close %d", &timeout)) {
                    ret = mbtk_gnss_close(timeout);
                    if(ret != GNSS_ERR_OK) {
                        printf("mbtk_gnss_close() fail : %d\n", ret);
                    } else {
                        printf("mbtk_gnss_close() success.\n");
                    }
                } else {
                    printf("CMD(%s) error.\n", cmd);
                }
            } else if(memcmp(cmd, "setting", 7) == 0) {
                int timeout;
                char set_cmd[100] = {0};
                if(2 == sscanf(cmd, "%*s %s %d", set_cmd, &timeout)) {
                    ret = mbtk_gnss_setting(set_cmd, timeout);
                    if(ret != GNSS_ERR_OK) {
                        printf("mbtk_gnss_setting() fail : %d\n", ret);
                    } else {
                        printf("mbtk_gnss_setting() success.\n");
                    }
                } else {
                    printf("CMD(%s) error.\n", cmd);
                }
            } else if(memcmp(cmd, "dl", 2) == 0) {
                int timeout;
                char fw_path[256] = {0};
                if(2 == sscanf(cmd, "%*s %s %d", fw_path, &timeout)) {
                    ret = mbtk_gnss_dl(fw_path, timeout);
                    if(ret != GNSS_ERR_OK) {
                        printf("mbtk_gnss_dl() fail : %d\n", ret);
                    } else {
                        printf("mbtk_gnss_dl() success.\n");
                    }
                } else {
                    printf("CMD(%s) error.\n", cmd);
                }
            } else if(memcmp(cmd, "ind_set", 7) == 0) {
                int type, timeout;
                if(2 == sscanf(cmd, "ind_set %d %d", &type, &timeout)) {
                    ret = mbtk_gnss_ind_set(type, timeout);
                    if(ret != GNSS_ERR_OK) {
                        printf("mbtk_gnss_ind_set() fail : %d\n", ret);
                    } else {
                        printf("mbtk_gnss_ind_set() success.\n");
                    }
                } else {
                    printf("CMD(%s) error.\n", cmd);
                }
            } else if(strcmp(cmd, "q") == 0) {
                break;
            } else {
                printf("Unknown cmd : %s\n", cmd);
            }
        }
    }

    ret = mbtk_gnss_deinit();
    if(ret != GNSS_ERR_OK) {
        printf("mbtk_gnss_deinit() fail : %d", ret);
        return -1;
    }

    return 0;
}

