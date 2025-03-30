/*
*    gnss_ipc.c
*
*    MBTK GNSS IPC service source.
*
*/
/******************************************************************************

                          EDIT HISTORY FOR FILE

  WHEN        WHO       WHAT,WHERE,WHY
--------    --------    -------------------------------------------------------
2024/6/15     LiuBin      Initial version

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>

#include "mbtk_log.h"
#include "mbtk_type.h"
#include "mbtk_gnss.h"

#define GNSS_SOCK_PATH "/tmp/mbtk_gnss_sock"

static int sock_listen_fd = -1;

typedef enum {
    GNSS_CMD_INIT = 0,
    GNSS_CMD_DEINIT,
    GNSS_CMD_SETTING,
    GNSS_CMD_DL
} gnss_cmd_enum;

static void help()
{
    printf("gnss_cli gnss_init <0-15>\n");
    printf("gnss_cli gnss_deinit\n");
    printf("gnss_cli gnss_setting cmd\n");
    printf("gnss_cli gnss_dl fw_name\n");
}

static int cmd_process(gnss_cmd_enum cmd, void *arg)
{
    if(sock_listen_fd < 0) {
        sock_listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if(sock_listen_fd < 0)
        {
            printf("socket() fail[%d].\n", errno);
            return -1;
        }

        struct sockaddr_un cli_addr;
        memset(&cli_addr, 0, sizeof(cli_addr));
        cli_addr.sun_family = AF_LOCAL;
        strcpy(cli_addr.sun_path, GNSS_SOCK_PATH);
        if(connect(sock_listen_fd, (struct sockaddr *)&cli_addr, sizeof(cli_addr)))
        {
            printf("connect() fail[%d].\n", errno);
            close(sock_listen_fd);
            sock_listen_fd = -1;
            return -1;
        }
    }

    char buff[100] = {0};
    if(cmd == GNSS_CMD_INIT) {
        if(arg) {
            int type = atoi((char*)arg);
            sprintf(buff, "gnss_init:%d", type);
        } else {
            return -1;
        }
    } else if(cmd == GNSS_CMD_DEINIT) {
        sprintf(buff, "gnss_deinit");
    } else if(cmd == GNSS_CMD_SETTING) {
        sprintf(buff, "gnss_setting:%s", arg);
    } else if(cmd == GNSS_CMD_DL) {
        sprintf(buff, "gnss_dl:%s", arg);
    } else {
        printf("Unknown cmd.\n");
        return -1;
    }

    write(sock_listen_fd, buff, strlen(buff));

    int len = 0;
    char *rsp = NULL;
    while(1) {
        memset(buff, 0, sizeof(buff));
        len = read(sock_listen_fd, buff, sizeof(buff));
        if(len > 0) {
            rsp = buff;
            if(rsp[len - 1] == MBTK_IND_END_FLAG) {
                rsp[len - 1] = '\0';
            }
            if(rsp[0] == MBTK_IND_START_FLAG) {
                rsp++;
            }
            printf("RSP : %s\n", rsp);
            if(cmd == GNSS_CMD_INIT) {
                if(memcmp(rsp, "gnss_init", 9) == 0) {
                    return atoi(rsp + 10);
                } else {
                    printf("gnss_init response error.\n");
                    return -1;
                }
            } else if(cmd == GNSS_CMD_DEINIT) {
                if(memcmp(rsp, "gnss_deinit", 11) == 0) {
                    return atoi(rsp + 12);
                } else {
                    printf("gnss_deinit response error.\n");
                    return -1;
                }
            } else if(cmd == GNSS_CMD_SETTING) {
                if(memcmp(rsp, "gnss_setting", 12) == 0) {
                    return atoi(rsp + 13);
                } else {
                    printf("gnss_setting response error.\n");
                    return -1;
                }
            } else if(cmd == GNSS_CMD_DL) {
                if(memcmp(rsp, "gnss_dl", 7) == 0) {
                    return atoi(rsp + 8);
                } else {
                    printf("gnss_dl response error.\n");
                    return -1;
                }
            } else {
                printf("Unknown response.\n");
                return -1;
            }
        } else if(len == 0) {
            printf("RSP is null.\n");
            return -1;
        } else {
            printf("read = %d:errno = %d\n", len, errno);
        }
    }
}

int main(int argc, char *argv[])
{
    int ret = 0;
    if(argc == 2) {
        if(strcmp(argv[1], "gnss_deinit")) {
            help();
            return -1;
        }

        ret = cmd_process(GNSS_CMD_DEINIT, NULL);
    } else if(argc == 3) {
        if(strcmp(argv[1], "gnss_init") == 0) {
            ret = cmd_process(GNSS_CMD_INIT, argv[2]);
        } else if(strcmp(argv[1], "gnss_setting") == 0) {
            ret = cmd_process(GNSS_CMD_SETTING, argv[2]);
        } else if(strcmp(argv[1], "gnss_dl") == 0) {
            ret = cmd_process(GNSS_CMD_DL, argv[2]);
        } else {
            help();
            return -1;
        }
    } else {
        help();
        return -1;
    }

    // printf("Error:%s\n", strerror(EBADF));

    printf("Result : %d\n", ret);
    return 0;
}

