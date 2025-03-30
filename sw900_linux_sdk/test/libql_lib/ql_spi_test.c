#include "ql/ql_spi.h"


int main(int argc, char *argv[])
{
    char send_data[64] = {0};
	char read_data[64] = {0};
	char crc = 0;
	int i = 0;
	int j = 0;

    //system("echo PB6 > /sys/kernel/debug/sunxi_pinctrl/sunxi_pin");
    //system("echo PB6 1 > /sys/kernel/debug/sunxi_pinctrl/function");
    //system("echo PB6 0 > /sys/kernel/debug/sunxi_pinctrl/data");

#if 0
static const char *device = "/dev/spidev1.0\0";
static uint8_t mode = 3; /* SPI通信使用全双工，设置CPOL＝0，CPHA＝0。 */
static uint8_t bits = 8; /* ８ｂiｔｓ读写，MSB first。*/
static uint32_t speed = 100 * 1000;/* 设置0.5M传输速度 */
static uint16_t delay = 500;
#endif
    int fd = -1;
	/* spi 初始化程序 */
	if((fd = Ql_SPI_Init("/dev/spidev1.0", SPIMODE3, 8, S_13M)) <= 0)
	{
        printf("Ql_SPI_Init() fail.\n");
        return -1;
	}

	send_data[0] = 0x55;
	send_data[1] = 0x00;
	send_data[2] = 0x84;
	send_data[3] = 0x00;
	send_data[4] = 0x08;
	send_data[5] = 0x00;
	send_data[6] = 0x00;

	crc = send_data[1];
	for (i = 2; i < 7; i++)
	{
		crc ^= send_data[i];
	}
	crc = ~crc;

	send_data[7] = crc;

	printf("send data:");
	for (i = 0; i < 8; i++)
	{
		printf("%#x, ", send_data[i]);
	}
	printf("\n");

	/* spi 发送数据 */
	if(Ql_SPI_Write_Read(fd, send_data,read_data, 8)) {
        printf("Ql_SPI_Write_Read() fail.\n");
        return -1;
	}

#if 0
	printf("read data:");
	for (j = 0; j < 20; j++)
	{
		printf("%#x, ", read_data[j]);
	}

	usleep(10000);

	memset(read_data, 0, sizeof(read_data));
	memset(send_data, 0, sizeof(send_data));
	/* spi 读取数据 */
	if(Ql_SPI_Write_Read(fd, send_data,read_data, 16)) {
        printf("Ql_SPI_Write_Read() fail.\n");
        return -1;
	}

	printf("read data:");
	for (j = 0; j < 20; j++)
	{
		printf("%#x, ", read_data[j]);
	}
#endif

    if(Ql_SPI_DeInit(fd)) {
        printf("Ql_SPI_DeInit() fail.\n");
        return -1;
    }

    printf("success!!!\n");
    return 0;
}
