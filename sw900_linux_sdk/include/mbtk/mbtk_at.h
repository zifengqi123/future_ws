/*
    MBTK AT header file.
*/
/******************************************************************************

                          EDIT HISTORY FOR FILE

  WHEN        WHO       WHAT,WHERE,WHY
--------    --------    -------------------------------------------------------
2023/4/6      b.liu    Initial version

******************************************************************************/
#ifndef __MBTK_AT__
#define __MBTK_AT__

/*===========================================================================
FUNCTION mbtk_at_init

DESCRIPTION:
  Initial MBTK AT.

PARAMETERS:
  None.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_at_init();


/*===========================================================================
FUNCTION mbtk_at_deinit

DESCRIPTION:
  Deinitial MBTK AT.

PARAMETERS:
  None.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_at_deinit();

/*===========================================================================
FUNCTION mbtk_at_send

DESCRIPTION:
  Send AT command.

PARAMETERS:
  cmd [IN]: AT command.
  rsp [OUT]: AT command response.
  rsp_len[IN] : AT command response buffer size.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_at_send(char* cmd, char* rsp, int rsp_len);

#endif /* __MBTK_AT__ */
