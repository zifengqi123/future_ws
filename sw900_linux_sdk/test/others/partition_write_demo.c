#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{

#if 1
    // 100K
    char buffer[102400];
    long len = -1;
    long count = 0;
    int fd = open("/etc/file.temp", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if(fd < 0) {
        printf("Open file error:%d\n", errno);
        return -1;
    }

    while((len = write(fd, buffer, sizeof(buffer))) > 0){
        count += len;
        printf("write : %d\n", count);
        //usleep(1000);
    }

    printf("Write complete,len = %d, errno = %d\n", len, errno);

    close(fd);
#else
    // 100K
    int buffer = 1;
    long len = -1;
    long count = 0;

    FILE *file = fopen("/etc/file.temp", "w");
    if(file == NULL) {
        printf("Open file error:%d\n", errno);
        return -1;
    }

    while((len = fwrite(&buffer, sizeof(int), 1,file)) > 0){
        buffer++;

        // printf("write : %d\n", buffer);
        //usleep(1000);
    }

    printf("Write complete,len = %d, errno = %d\n", len, errno);

    fclose(file);
#endif
    return 0;
}

