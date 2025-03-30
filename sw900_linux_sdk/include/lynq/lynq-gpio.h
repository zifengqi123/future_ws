#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sc_bsp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_GPIO_NUM        (200)
static void *test_gpio_handle[MAX_GPIO_NUM]={NULL};

int lynq_gpio_init(int gpio, int direction, int value, int pullsel);

int lynq_gpio_deinit(int gpio);

int lynq_gpio_direction_set(int gpio, int direction);

int lynq_gpio_value_set(int gpio, int value);

int lynq_gpio_value_get(int gpio);

int lynq_gpio_pullsel_set(int gpio, int pullsel);

int lynq_gpio_pullsel_get(int gpio);


#ifdef __cplusplus
}
#endif
