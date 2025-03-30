#include "ql/ql_i2c.h"


int main(int argc, char *argv[])
{
	int fd = -1;
    if(argc != 2) {
        printf("./i2c_test <dev>\n");
        return -1;
    }
    char send_data[64] = {0};
	send_data[0] = 0x55;
	send_data[1] = 0x00;
	send_data[2] = 0x84;
    fd= Ql_I2C_Init(argv[1]);
    if(fd <= 0) {
        printf("Ql_I2C_Init() fail.\n");
        return -1;
    }

	if(Ql_I2C_Write(fd, 0x12, 0x10,send_data, 3) < 0) {
        printf("Ql_I2C_Write() fail.\n");
        return -1;
	}

    Ql_I2C_Deinit(fd);

    printf("Success!!!\n");

    return 0;
}
