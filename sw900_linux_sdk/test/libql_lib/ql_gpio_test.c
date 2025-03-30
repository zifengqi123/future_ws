#include "ql/ql_gpio.h"
#include "mbtk_log.h"

int main(int argc, char *argv[])
{
    mbtk_log_init("radio", "MBTK_GPIO");

    if(argc != 2) {
        printf("./gpio_test <gpio>\n");
        return -1;
    }

    int tmp_gpio = atoi(argv[1]);
    if(tmp_gpio <= 0) {
        printf("GPIO error : %d\n", tmp_gpio);
        return -1;
    }

    Enum_PinName gpio = (Enum_PinName)tmp_gpio;
    if(Ql_GPIO_Init(gpio, PINDIRECTION_OUT, PINLEVEL_LOW, PINPULLSEL_DISABLE)) {
        printf("Ql_GPIO_Init() fail.\n");
        return -1;
    }

    printf("GPIO : %d, dir : %s, level : %d\n", gpio, Ql_GPIO_GetDirection(gpio) == PINDIRECTION_IN ? "in" : "out",
        Ql_GPIO_GetLevel(gpio));

    if(Ql_GPIO_SetLevel(gpio, PINLEVEL_HIGH)) {
        printf("Ql_GPIO_SetLevel() fail.\n");
        return -1;
    }

    printf("GPIO : %d, dir : %s, level : %d\n", gpio, Ql_GPIO_GetDirection(gpio) == PINDIRECTION_IN ? "in" : "out",
        Ql_GPIO_GetLevel(gpio));

    if(Ql_GPIO_Uninit(gpio)) {
        printf("Ql_GPIO_Uninit() fail.\n");
        return -1;
    }

    printf("Success!!!\n");
    return 0;
}

