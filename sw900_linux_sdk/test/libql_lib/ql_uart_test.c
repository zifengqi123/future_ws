#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include "ql/ql_uart.h"

int fd;
pthread_mutex_t mutexsum;	//创建mutex锁

typedef struct {
    char *node;
    char *rate;
} thread_arg;

void *read_function(void *arg) {
	int i=0;
    thread_arg *args = (thread_arg *)arg;
    printf("read_function() node = %s rate = %s\n", args->node, args->rate);

    ST_UARTDCB dcb;
    memset(&dcb, 0x0, sizeof(ST_UARTDCB));
    dcb.databit = DB_CS8;
    dcb.parity = PB_NONE;
    dcb.flowctrl = FC_NONE;

    if(Ql_UART_SetDCB(fd, &dcb)) {
        printf("Ql_UART_SetDCB() fail.\n");
        return -1;
    }

    char buff[1024];
    int len;
    while(1) {
        // pthread_mutex_lock(&mutexsum);
        memset(buff, 0x0 ,1024);
        len = Ql_UART_Read(fd, buff, 1024);
        if(len > 0) {
            if(memcmp(buff, "exit", 4) == 0) {
                Ql_UART_Write(fd, "exit\r\n", 6);
                break;
            } else {
                //printf("<%s\n", buff);
                for(i=0;i<len;i++)
					printf("<<%x<<\n", buff[i]);

                //Ql_UART_Write(fd, "Module received data!\r\n", 23);
            }
        }
        // pthread_mutex_unlock(&mutexsum);
    }
    Ql_UART_Close(fd);
    return NULL;
}

void *write_function(void *arg) {
    //char str[20] = "write success!\r\n";
	char str[20];

	str[0]=0x02; 
	str[1]=0x17;
	str[2]=0x2f;
	str[3]=0x00;
	str[4]=0x01;
	str[5]=0x00;
	str[6]=0x03;
	str[7]=0x39;


    while (1)
    {
    	printf("sending datat to 1903...\n");
        sleep(5);
        // pthread_mutex_lock(&mutexsum);
        Ql_UART_Write(fd, str,8);
        // pthread_mutex_unlock(&mutexsum);
    }
    Ql_UART_Close(fd);
    return NULL;
}

int main(int argc, char *argv[])
{
    printf("main() start \n");
    mbtk_log_init("radio", "MBTK_UART");
    if(argc != 3) {
        printf("./uart_test <dev> <baudrate>\n");
        return -1;
    }

    printf("main() start  node = %s rate = %s\n", argv[1], argv[2]);
    fd = Ql_UART_Open(argv[1], (Enum_BaudRate)atoi(argv[2]), FC_NONE);
    if(fd < 0) {
        printf("Ql_UART_Open() fail.\n");
        return -1;
    }

    // pthread_mutex_init(&mutexsum, NULL);
    pthread_t read_thread, write_thread;

    thread_arg args = {argv[1], argv[2]};
    printf("start read_thread node = %s rate = %s\n", args.node, args.rate);
    pthread_create(&read_thread, NULL, read_function, &args);

    printf("start write_thread \n");
    pthread_create(&write_thread, NULL, write_function, NULL);

    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    printf("main() end \n");
    return 0;
}