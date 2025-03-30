#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#include "mbtk_tcpip.h"
#include "mbtk_log.h"

/*
    int link_id;
    char ser_addr[256];
    int ser_port;
    mbtk_sock_type prot_type; // TCP/UDP
    mbtk_tcpip_type_enum tcpip_type; // Only support client.
    int local_port;
    bool ack_support;
    bool ssl_support;
    bool ignore_cert;
    uint32 heartbeat_time;
    uint32 delay_time;

    mbtk_tcpip_read_callback_func read_cb;

*/
static void help()
{
    printf("\n************************************************************************\n");
    printf("net_open: Open network.\n");
    printf("net_close: Close network.\n");
    printf("link_open <link_id> <ser_addr> <ser_port> <loc_port> <TCP/UDP> <CLI/SER> <ack> <ssl> <ignore_cert> <heartbeat_time> <delay_time> <read_cb>: Open link.\n");
    printf("link_close <link_id>: Close link.\n");
    printf("send <link_id> <data>:Send data.\n");
    printf("recv <link_id> <len>:Recv data.\n");
    printf("traffic_reset <link_id>:Traffic reset.\n");
    printf("traffic_get <link_id>:Traffic get.\n");
    printf("state_get <link_id>:Link state get.\n");
    printf("tcp_info <link_id>:TCP information state get.\n");
    printf("\n************************************************************************\n");
}

static void sig_process(int sig)
{
    LOGI("I got signal %d\n", sig);
    switch(sig)
    {
        case SIGINT: // Ctrl + C
        {
            LOGI("Exit by SIGINT.\n");
            mbtk_tcpip_err_enum err = mbtk_tcpip_net_close();
            if(err == MBTK_TCPIP_ERR_SUCCESS) {
                printf("Net close success.\n");
            } else {
                printf("Net close fail:%d\n", err);
            }
            exit(0);
        }
        case SIGQUIT: // Ctrl + \ (类似 SIGINT ，但要产生core文件)
        {
            LOGI("Exit by SIGQUIT.\n");
            mbtk_tcpip_err_enum err = mbtk_tcpip_net_close();
            if(err == MBTK_TCPIP_ERR_SUCCESS) {
                printf("Net close success.\n");
            } else {
                printf("Net close fail:%d\n", err);
            }
            exit(0);
        }
        case SIGTERM:// 默认kill   (同 SIGKILL ，但 SIGKILL 不可捕获)
        {
            LOGI("Exit by SIGTERM.\n");
            mbtk_tcpip_err_enum err = mbtk_tcpip_net_close();
            if(err == MBTK_TCPIP_ERR_SUCCESS) {
                printf("Net close success.\n");
            } else {
                printf("Net close fail:%d\n", err);
            }
            exit(0);
        }
        case SIGTSTP:// Ctrl + Z (同 SIGSTOP ，但 SIGSTOP 不可捕获)
        {
            LOGI("Exit by SIGTSTP.\n");
            exit(0);
        }
        case SIGSEGV: // 如空指针
        {
            LOGI("Exit by SIGSEGV.\n");
            exit(0);
        }
        default:
        {
            LOGI("Unknown sig:%d\n",sig);
            break;
        }
    }
}

void tcpip_print_tcp_info(mbtk_tcpip_tcp_state_info_s *info)
{
    printf("Link - %d\n", info->link_id);
    printf("fd - %d\n", info->sock_fd);
    printf("State - %d\n", info->state);
    printf("Recv data length - %d\n", info->recv_data_len);
}

void tcpip_read_cb(int link_id, const char* data, int data_len)
{
    printf("\nRECV(%d-%d):%s\n", link_id, data_len, data);
}

void tcpip_net_callback_func(int state, const char* addr)
{
    if(state) {
        printf("Net conncect, IP : %s\n", addr);
    } else {
        printf("Net disconnect.\n");
    }
}

void tcpip_sock_callback_func(int link_id, int state)
{
    if(state == 0) {
        printf("Link[%d] disconnected.\n", link_id);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_process);
    signal(SIGQUIT, sig_process);
    signal(SIGTERM, sig_process);
    //signal(SIGTSTP, sig_process);
    //signal(SIGSEGV, sig_process);

    mbtk_log_init("radio","MBTK_TCPIP");

    printf(">>>>>>>>>>>>>>>>>>>>>>>>Enter cmd:\n");
    char cmd[100];
    while(1)
    {
        memset(cmd, 0, 100);
        mbtk_tcpip_err_enum err;
        if(fgets(cmd, 100, stdin))
        {
            char *ptr = cmd + strlen(cmd) - 1;
            while(ptr >= cmd && (*ptr == '\r' || *ptr == '\n'))
            {
                *ptr-- = '\0';
            }
            // net_open
            if(!strncasecmp(cmd, "net_open", 8)){
                err = mbtk_tcpip_net_open(tcpip_net_callback_func, tcpip_sock_callback_func);
                if(err == MBTK_TCPIP_ERR_SUCCESS) {
                    printf("Net open success.\n");
                } else {
                    printf("Net open fail:%d\n", err);
                }
            } else if(!strncasecmp(cmd, "net_close", 9)){ // net_close
                err = mbtk_tcpip_net_close();
                if(err == MBTK_TCPIP_ERR_SUCCESS) {
                    printf("Net close success.\n");
                } else {
                    printf("Net close fail:%d\n", err);
                }
            }
            // link_open <link_id> <ser_addr> <ser_port> <loc_port> <TCP/UDP> <CLI/SER> <ack> <ssl> <ignore_cert> <heartbeat_time> <delay_time> <read_cb>
            else if(!strncasecmp(cmd, "link_open", 9)){
                mbtk_tcpip_info_t info;
                char prot_type[10] = {0};
                char type[10] = {0};
                int read_cb_set;
                memset(&info, 0x0, sizeof(mbtk_tcpip_info_t));
                int count = sscanf(cmd, "link_open %d %s %d %d %s %s %d %d %d %d %d %d", &(info.link_id),
                                info.ser_addr, &(info.ser_port), &(info.local_port), prot_type, type,
                                &(info.ack_support), &(info.ssl_support), &(info.ignore_cert), &(info.heartbeat_time),
                                &(info.delay_time), &read_cb_set);
                if(count == 12) {
                    if(!strncasecmp(prot_type, "UDP", 3)) {
                        info.prot_type = MBTK_SOCK_UDP;
                    } else {
                        info.prot_type = MBTK_SOCK_TCP;
                    }

                    if(!strncasecmp(type, "SER", 3)) {
                        info.tcpip_type = MBTK_TCPIP_TYPE_SERVER;
                    } else {
                        info.tcpip_type = MBTK_TCPIP_TYPE_CLIENT;
                    }

                    if(read_cb_set) {
                        info.read_cb = tcpip_read_cb;
                    } else {
                        info.read_cb = NULL;
                    }

                    err = mbtk_tcpip_sock_open((const mbtk_tcpip_info_t*)(&info));
                    if(err == MBTK_TCPIP_ERR_SUCCESS) {
                        printf("Link open success.\n");
                    } else {
                        printf("Link open fail:%d\n", err);
                    }
                } else {
                    printf("ARG error.\n");
                }
            } else if(!strncasecmp(cmd, "link_close", 10)){ // link_close <link_id>
                int link_id;
                int count = sscanf(cmd, "link_close %d", &link_id);
                if(count == 1) {
                    err = mbtk_tcpip_sock_close(link_id);
                    if(err == MBTK_TCPIP_ERR_SUCCESS) {
                        printf("Link close success.\n");
                    } else {
                        printf("Link close fail:%d\n", err);
                    }
                } else {
                    printf("ARG error.\n");
                }
            } else if(!strncasecmp(cmd, "send", 4)){ // send <link_id> <data>
                int link_id;
                char data[100] = {0};
                int count = sscanf(cmd, "send %d %s", &link_id, data);
                if(count == 2) {
                    int len = mbtk_tcpip_send(link_id, data, strlen(data), NULL, 0);
                    if(len == strlen(data)) {
                        printf("Send success:%d.\n", len);
                    } else {
                        printf("Send fail:%d/%d\n", len, strlen(data));
                    }
                } else {
                    printf("ARG error.\n");
                }
            } else if(!strncasecmp(cmd, "recv", 4)){ // recv <link_id> <len>
                int link_id;
                int read_size;
                int count = sscanf(cmd, "recv %d %d", &link_id, &read_size);
                if(count == 2 && read_size > 0 && read_size <= 2048) {
                    char buff[2048] = {0};
                    int len = mbtk_tcpip_read(link_id, buff, read_size);
                    if(len > 0) {
                        printf("RECV[%d]:%s\n", len, buff);
                    } else {
                        printf("RECV fail:%d\n", len);
                    }
                } else {
                    printf("ARG error.\n");
                }
            } else if(!strncasecmp(cmd, "traffic_reset", 13)){ // traffic_reset <link_id>
                int link_id;
                int count = sscanf(cmd, "traffic_reset %d", &link_id);
                if(count == 1) {
                    err = mbtk_tcpip_data_traffic_reset(link_id);
                    if(err == MBTK_TCPIP_ERR_SUCCESS) {
                        printf("Traffic reset success.\n");
                    } else {
                        printf("Traffic reset fail:%d\n", err);
                    }
                } else {
                    printf("ARG error.\n");
                }
            } else if(!strncasecmp(cmd, "traffic_get", 11)){ // traffic_get <link_id>
                int link_id;
                int count = sscanf(cmd, "traffic_get %d", &link_id);
                if(count == 1) {
                    int traffic = mbtk_tcpip_data_traffic_get(link_id);
                    if(traffic >= 0) {
                        printf("Traffic : %d\n", traffic);
                    } else {
                        printf("Get raffic fail:%d\n", traffic);
                    }
                } else {
                    printf("ARG error.\n");
                }
            } else if(!strncasecmp(cmd, "state_get", 9)){ // state_get <link_id>
                int link_id;
                int count = sscanf(cmd, "state_get %d", &link_id);
                if(count == 1) {
                    int state = mbtk_tcpip_link_state_get(link_id);
                    if(state == 1) {
                        printf("Link %d connected.\n", link_id);
                    } else if(state == 0) {
                        printf("Link %d disconnected.\n", link_id);
                    } else {
                        printf("Get link state fail:%d\n", state);
                    }
                } else {
                    printf("ARG error.\n");
                }
            }
            else if(!strncasecmp(cmd, "tcp_info", 8)){ // tcp_info <link_id>
                int link_id;
                int count = sscanf(cmd, "tcp_info %d", &link_id);
                if(count == 1) {
                    mbtk_tcpip_tcp_state_info_s tcp_info;
                    if(mbtk_tcpip_info_get(link_id, &tcp_info)) {
                        printf("mbtk_tcpip_info_get() fail.\n");
                    } else {
                        tcpip_print_tcp_info(&tcp_info);
                    }
                } else {
                    printf("ARG error.\n");
                }
            }
            else if(!strcasecmp(cmd, "h") || !strcasecmp(cmd, "help")) {
                help();
            } else if(!strcasecmp(cmd, "q")) {
                err = mbtk_tcpip_net_close();
                if(err == MBTK_TCPIP_ERR_SUCCESS) {
                    printf("Net close success.\n");
                } else {
                    printf("Net close fail:%d\n", err);
                }
                break;
            } else {
                printf("\n");
            }
        }
    }

    LOGD("Client exec complete.");
#if 1
    while(1)
    {
        sleep(1000 * 365 * 24 * 60 * 60);
    }
#else
    sleep(1);
#endif
    return 0;
}

