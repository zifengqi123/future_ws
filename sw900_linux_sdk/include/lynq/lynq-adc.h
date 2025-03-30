#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sc_bsp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ADC_CHANNEL_ENUM
{
    QADC_NONE = 0,
    ADC0 =1,
    ADC1 =2,
    ADC2 =3,
    QADC_END
}ADC_CHANNEL_E;

int qser_adc_show(ADC_CHANNEL_E qadc);

#ifdef __cplusplus
}
#endif
