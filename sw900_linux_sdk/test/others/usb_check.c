#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

#include "mbtk_utils.h"

static int running = 0;

static void sig_handler(int sig)
{
    printf("Signal : %d\n", sig);
    running = 0;
}


int main(int argc, char *argv[])
{
    int fd = open("/tmp/usb.info", O_CREAT | O_WRONLY, 0666);
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    if(fd > 0) {
        char buff[1024];
        running = 1;
        while(running){
            memset(buff, 0, 1024);
            if(mbtk_cmd_line("cat /sys/class/android_usb/android0/state", buff, 1024)) {
                write(fd, buff, strlen(buff));
                write(fd, "\n", 1);
            }
            sleep(1);
        }
        close(fd);
    }
    return 0;
}

