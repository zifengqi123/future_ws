#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mbtk_utils.h"
#include "mbtk_log.h"

#define KMSG "/dev/kmsg"

static int fd = -1;

void timer_alrm_cb(int signo)
{
    char *str = "----------------------Timeout---------------------------\n";
    if(fd > 0) {
        write(fd, str, strlen(str));
    }

    printf("%s", str);
    LOGI("%s", str);
}


int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("mbtk_timer_test <time>\n");
        return -1;
    }
    mbtk_log_init("radio", "MBTK");

    int time = atoi(argv[1]);
    if(time > 0) {
        mbtk_timer_set(timer_alrm_cb, time * 1000);
    }

    fd = open(KMSG, O_WRONLY);
    if(fd > 0) {
        printf("Open kmsg success.");
    }

    while(1) {
        sleep(24 * 60 * 60);
    }

    return 0;
}

