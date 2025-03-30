/*
* mbtk_alarm.h
* MBTK alarm API.
*
* Author : lb
* Date   : 2023/11/17 16:20:43
*/
#ifndef _MBTK_ALARM_H
#define _MBTK_ALARM_H


/*===========================================================================
FUNCTION mbtk_wakealarm_set()

DESCRIPTION:
  Set sleep wake alarm.

PARAMETERS:
  time_sec [IN]: Sleep wake-up time in second.

RETURN VALUE:
  int : Return 0 if success, other for failures.

===========================================================================*/
int mbtk_wakealarm_set(unsigned long time_sec);

/*===========================================================================
FUNCTION mbtk_poweralarm_set()

DESCRIPTION:
  Set shutdown alarm clock.

PARAMETERS:
  time_sec [IN]: Shutdown wake-up time in second.

RETURN VALUE:
  int : Return 0 if success, other for failures.

===========================================================================*/
int mbtk_poweralarm_set(unsigned long time_sec);


#endif /* _MBTK_ALARM_H */
