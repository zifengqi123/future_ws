#ifndef __MBTK_POWER_H__
#define __MBTK_POWER_H__

#include "mbtk_log.h"

typedef enum{
    MBTK_POWER_RESULT_SUCCESS = 0,
    MBTK_POWER_RESULT_GNSS_TIMEOUT,
    MBTK_POWER_RESULT_GNSS_CLOSE_FAIL,
    MBTK_POWER_RESULT_NO_SLEEP_NODE,
    MBTK_POWER_RESULT_UKNOWN_ERROR
}mbtk_power_result_type;

int mbtk_system_sleep(void);
#endif