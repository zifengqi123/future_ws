#include <stdio.h>

#include "ql/ql_adc.h"

int main(int argc, char *argv[])
{
    int adc = ql_adc_show(ADC0);
    if(adc > 0) {
        printf("ADC0 = %d\n", adc);
    } else {
        printf("Get ADC0 fail.\n");
    }

    adc = ql_adc_show(ADC1);
    if(adc > 0) {
        printf("ADC1 = %d\n", adc);
    } else {
        printf("Get ADC1 fail.\n");
    }
    return 0;
}

