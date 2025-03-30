#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include "mbtk_type.h"
#include "mbtk_log.h"

#define DATABITS    CS8
#define STOPBITS    0
#define PARITYON    0
#define PARITY      0

int uart_baud_get(int baud)
{
    int rate = 0;
    switch(baud)
    {
        case 300:
            rate = B300;
            break;
        case 600:
            rate = B600;
            break;
        case 1200:
            rate = B1200;
            break;
        case 2400:
            rate = B2400;
            break;
        case 4800:
            rate = B4800;
            break;
        case 9600:
            rate = B9600;
            break;
        case 19200:
            rate = B19200;
            break;
        case 38400:
            rate = B38400;
            break;
        case 57600:
            rate = B57600;
            break;
        case 115200:
            rate = B115200;
            break;
        case 230400:
            rate = B230400;
            break;
        case 460800:
            rate = B460800;
            break;
        case 921600:
            rate = B921600;
            break;
        case 1500000:
            rate = B1500000;
            break;
        case 2000000:
            rate = B2000000;
            break;
        case 3000000:
            rate = B3000000;
            break;
        case 4000000:
            rate = B4000000;
            break;
        default:
            rate = B115200;
            break;
    }

    return rate;
}

int gnss_port_open(const char *dev, int flag, int baud, bool tty)
{

    int fd = -1;
    if((fd = open(dev, flag)) < 0)
    {
        printf("Open %s fail errno = [%d].\n", dev, errno);
        return -1;
    }

    printf("Open %s success.\n", dev);
    if (tty)
    {
        int rate = uart_baud_get(baud);
        /* set newtio */
        struct termios newtio;
        memset(&newtio, 0, sizeof(newtio));
        //(void)fcntl(fd, F_SETFL, 0);
        /* no flow control for uart by default */
        newtio.c_cflag = rate | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR;
        //newtio.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;    /* disable ECHO, ICANON, etc... */

        newtio.c_cc[VERASE]   = 0x8;      /* del */
        newtio.c_cc[VEOF]     = 4;      /* Ctrl-d */
        newtio.c_cc[VMIN]     = 1;      /* blocking read until 1 character arrives */
        newtio.c_cc[VEOL]     = 0xD;      /* '\0' */

        tcflush(fd, TCIOFLUSH);
        tcsetattr(fd, TCSANOW, &newtio);
    }

    return fd;
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("%s <dev>\n", argv[1]);
        return -1;
    }

    int fd = gnss_port_open(argv[1], O_RDWR | O_NONBLOCK | O_NOCTTY, 115200, TRUE);
    if(fd < 0) {
        printf("gnss_port_open(%s) fail:%d\n", argv[1], errno);
        return -1;
    }

    char buff[1024];
    int len, ret;
    fd_set fdr;
    FD_ZERO(&fdr);
    FD_SET(fd, &fdr);

    while(1) {
        ret = select(fd + 1, &fdr, NULL, 0, NULL);
        if (ret < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            printf("select error, errno = %d (%s)\n", errno, strerror(errno));
            break;
        }
        else if (ret == 0)
        {
            printf("select ret == 0\n");
            break;
        }

        if (FD_ISSET(fd, &fdr))
        {
            memset(buff, 0, sizeof(buff));
            len = read(fd, buff, sizeof(buff));
            if(len > 0) {


            } else if(len ==0 ){
                printf("Read end : len = 0\n");
                break;
            } else {
                if(EAGAIN == errno) {
                    usleep(50000);
                    continue;
                } else {
                    printf("Read ret = -1 ,errno = %d\n", errno);
                    break;
                }
            }
        }
        else
        {
            printf("Unknown select event.\n");
            continue;
        }
    }

    printf("exit.\n");
    return 0;
}


