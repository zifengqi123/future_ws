#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include "ql/ql_uart.h"

#define READ_SIZE 1024
#define  NMEA_MAX_SIZE  127

typedef struct{
        int     pos;
        int     overflow;
        char    in[ NMEA_MAX_SIZE+1 ];
}NmeaReader;

int fd = -1;
static NmeaReader r = {0};

static const char CHARSET[] = {",.*$GRABMCDVLONSWTKPVUXYZEF-="};
static int CharSetValid(const char *ptr, int len)
{
    unsigned int temp, i, j = 0;
    const char *p = ptr;
    for (; len > 0; len--, j++) {
        temp = *ptr++;
        if (temp >= '0' && temp <= '9') {
            continue;
        }
        i = 0;
        for (;;) {
            if (temp == CHARSET[i]) {
                break;
            }
            if (++i >= strlen(CHARSET)) {
                printf("[CharSetValid err:%.2x %d %d ]\n", temp, j, len);
                printf("[ERR1_NMEA]:%s", p);
                return -1;
            }
        }
    }

    return 0;
}

static unsigned char nmea_checksum(const char *nmea)
{
    const char *p = nmea;
    unsigned char chs = 0;

    while (*p == '$')   // skip '$'
        p++;
    while (*p != '*' && *p != 0)
        chs ^= *p++;

    return chs;
}

static int str_count(const char *data, int data_len, char ch)
{
    int count = 0;
    int i = 0;
    while(i < data_len && data[i]){
        if(data[i] == ch){
            count++;
        }
        i++;
    }

    return count;
}

static int nmea_check(const char *nmea, int len)
{
    // No found '*'
    if(str_count(nmea, len, '*') == 0)
    {
        return -1;
    }

    char *checksum_str = strstr(nmea, "*");
    checksum_str++; // Jump '*'
    char checksum_buf[3] = {0};
    snprintf(checksum_buf, 3, "%02x", nmea_checksum(nmea));
    if(strncasecmp(checksum_buf, checksum_str, 2))
    {
        printf("[nmea_checksum error : %s]\n", checksum_buf);
        printf("[ERR2_NMEA]:%s", nmea);
        return -1;
    }

    return 0;
}

static void nmea_reader_addc(NmeaReader* r, int c)
{
    if (r->overflow)
    {
        r->overflow = (c != '\n');
        return;
    }

    if (r->pos >= (int) sizeof(r->in)-1 )
    {
        r->overflow = 1;
        r->pos      = 0;
        return;
    }

    if(c == '\0')
    {
        return;
    }

    r->in[r->pos] = (char)c;
    r->pos       += 1;

    if (c == '\n')
    {
        r->in[r->pos] = 0;
        CharSetValid(r->in, r->pos);
        nmea_check(r->in, r->pos);
        printf("[NMEA]:%s", r->in);
        
        r->pos = 0;
    }
}


int main(int argc, char *argv[])
{
    printf("main() start \n");
    mbtk_log_init("radio", "MBTK_GNSS");
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

    char read_buf[READ_SIZE] = {0};
    int read_len = -1;
    int i = 0;

    while(1)
    {
        
        read_len = Ql_UART_Read(fd, read_buf, READ_SIZE);
        if(read_len < 0 && errno != EWOULDBLOCK)
        {
            printf("error while reading from gps fd: %s\n", strerror(errno));
        }
        else
        {
            for(i = 0; i < read_len; i++)
            {
                nmea_reader_addc(&r, read_buf[i]);
            }
            memset(read_buf, 0x0 , read_len);
        }
    }
    printf("main() end \n");
    return 0;
}

