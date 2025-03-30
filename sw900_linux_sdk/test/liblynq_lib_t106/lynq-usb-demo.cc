#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <linux/netlink.h>

#include <lynq/lynq-qser-usb.h>

sem_t usbplug_sem;
void do_get_adb_state(int nargs, char **argv)
{

    int ret = 0;

    ret = qser_get_usb_usermode_adb_state();
    if (ret < 0)
    {
        printf("get adb state fail,ret:%d \n", ret);
        return;
    }

    printf("adb state:%d, %s\n", ret, ((ret == 0) ? "off" : "on"));
}

void do_get_net_state(int nargs, char **argv)
{

    int ret = 0;

    ret = qser_get_usb_usermode_net_state();
    if (ret < 0)
    {
        printf("get net state fail,ret:%d \n", ret);
        return;
    }

    printf("net state:%d, %s\n", ret, ((ret == 0) ? "off" : "on"));
}

int main(int argc, char *argv[])
{
    sem_init(&usbplug_sem, 0, 0);
    do_get_adb_state(argc, argv);
    do_get_net_state(argc, argv);
    sem_post(&usbplug_sem);
    return 0;
}