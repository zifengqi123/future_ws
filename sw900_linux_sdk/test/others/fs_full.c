#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFF_SIZE 4096

int main(int argc, char *argv[])
{
    int fd = open("/test.data", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if(fd < 0) {
        printf("open() fail:%d", errno);
        return -1;
    }

    char buff[BUFF_SIZE];
    while(1) {
        if(write(fd, buff, BUFF_SIZE) < 0) {
            printf("write() fail:%d", errno);
            break;
        }
    }

    close(fd);
    return 0;
}

