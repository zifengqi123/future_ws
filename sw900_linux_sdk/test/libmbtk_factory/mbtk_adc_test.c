#include <stdio.h>

#include "mbtk_adc.h"
#include "mbtk_log.h"

int main(int argc, char *argv[])
{
	mbtk_log_init("radio", "MBTK_ADC");
    int adc = mbtk_adc_get(MBTK_ADC0);
    if(adc > 0) {
        printf("ADC0 = %d\n", adc);
    } else {
        printf("Get ADC0 fail.\n");
    }

    adc = mbtk_adc_get(MBTK_ADC1);
    if(adc > 0) {
        printf("ADC1 = %d\n", adc);
    } else {
        printf("Get ADC1 fail.\n");
    }
    return 0;
}

