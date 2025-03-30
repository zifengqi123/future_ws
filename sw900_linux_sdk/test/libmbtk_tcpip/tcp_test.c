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


int main(int argc, char *argv[])
{


    printf(">>>>>>>>>>>START TCP TEST>>>>>>>>>>>>>\r\n");
    mbtk_tcpip_err_enum err;
        
    mbtk_tcpip_info_t info;
    memset(&info, 0x0, sizeof(mbtk_tcpip_info_t));
    char *ip = "www.chaixiongfeng.group";
    strcpy(&info.ser_addr,ip);
    info.link_id=1;
    info.ser_port =45002;
    info.prot_type = MBTK_SOCK_TCP;
    info.tcpip_type = MBTK_TCPIP_TYPE_CLIENT;
    info.read_cb = NULL;
        

    err = mbtk_tcpip_sock_open((const mbtk_tcpip_info_t*)(&info));
    if(err == MBTK_TCPIP_ERR_SUCCESS) {
        printf("Link open success.\n");
    } else {
        printf("Link open fail:%d\n", err);
        return 0;
    }
    
    while(1)
    {
        char *data="stargcharge connect\r\n";
        int len = mbtk_tcpip_send(1, data, strlen(data), NULL, 0);
        if(len == strlen(data)) 
        {
            printf("Send success:%d.\n", len);
        } 
        else 
        {
            printf("Send fail:%d/%d\n", len, strlen(data));
        }

        char buff[128];
        len = mbtk_tcpip_read(1, buff, 128);
        if(len > 0) 
        {
            printf("RECV[%d]:%s\n", len, buff);
        }
        else 
        {
            printf("RECV fail:%d\n", len);
        }

        err = mbtk_tcpip_sock_close(1);
        if(err == MBTK_TCPIP_ERR_SUCCESS) 
        {
            printf("Link close success.\n");
        } 
        else 
        {
            printf("Link close fail:%d\n", err);
        }
        sleep(5);
    }      
}


