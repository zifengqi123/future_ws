/*
    MBTK ADC interface define.
*/
/******************************************************************************

                   EDIT HISTORY FOR FILE

  WHEN        WHO       WHAT,WHERE,WHY
--------    --------    -------------------------------------------------------
2023/4/3      b.liu    Initial version

******************************************************************************/
#ifndef __MBTK_ADC_H__
#define __MBTK_ADC_H__

/*
* MBTK ADC define.
*/
typedef enum {
    MBTK_ADC0 = 0,  /* ADC 0 */
    MBTK_ADC1,       /* ADC 1 */
    MBTK_ADC2       /* ADC 2 */
} mbtk_adc_enum;


/*===========================================================================
FUNCTION mbtk_adc_get

DESCRIPTION:
  Get ADC's value.

PARAMETERS:
  adc [IN]: ADC path, refer to mbtk_adc_enum.

RETURN VALUE:
  int : ADC value.

===========================================================================*/
int mbtk_adc_get(mbtk_adc_enum adc);
int mbtk_adc_close(void);


#endif /* __MBTK_ADC_H__ */

