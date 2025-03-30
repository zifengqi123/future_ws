#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lynq/lynq-adc.h>

void handle_adc(const char* adc_name, int adc_type)
{
    if (adc_name == NULL)
    {
        printf("Error: adc_name is a null pointer\n");
        return;
    }
    int adc_value = qser_adc_show((ADC_CHANNEL_E)adc_type);
    if (adc_value < 0)
    {
        printf("Error: Failed to get the value of %s\n", adc_name);
        return;
    }
    printf("%s value: %d mV\n", adc_name, adc_value);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <ADC0|ADC1|ADC2>\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "ADC0") == 0)
    {
        handle_adc("ADC0", ADC0);
    }
    else if (strcmp(argv[1], "ADC1") == 0)
    {
        handle_adc("ADC1", ADC1);
    }
    else if (strcmp(argv[1], "ADC2") == 0)
    {
        handle_adc("ADC2", ADC2);
    }
    else
    {
        printf("Please enter valid parameters: ADC0|ADC1|ADC2\n");
        return -1;
    }

    return 0;
}
