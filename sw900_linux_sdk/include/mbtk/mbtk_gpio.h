/*
* mbtk_gpio.h
*
* MBTK GPIO process header.
*
* Author : lb
* Date   : 2024/6/14 14:57:56
*/
#ifndef _MBTK_GPIO_H
#define _MBTK_GPIO_H

typedef enum {
    MBTK_GPIO_DIRECT_UNKNOWN,
    MBTK_GPIO_DIRECT_IN,
    MBTK_GPIO_DIRECT_OUT
} mbtk_gpio_direct_t;

mbtk_gpio_direct_t mbtk_gpio_direct_get(int gpio);

int mbtk_gpio_direct_set(int gpio, mbtk_gpio_direct_t dir);

int mbtk_gpio_value_get(int gpio);

int mbtk_gpio_value_set(int gpio, mbtk_gpio_direct_t dir, int value);


#endif /* _MBTK_GPIO_H */
