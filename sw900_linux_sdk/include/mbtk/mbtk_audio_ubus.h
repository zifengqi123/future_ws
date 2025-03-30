/*
* mbtk_audio_ubus.h
*
* MBTK audio ubus client API header.
*
* Author : lb
* Date   : 2024/3/18 16:04:33
*/
#ifndef _MBTK_AUDIO_UBUS_H
#define _MBTK_AUDIO_UBUS_H


int mbtk_audio_ubus_init();

int mbtk_audio_ubus_deinit();

/**
* mode:
*     AUDIO_MODE_INVALID = -2,
*     AUDIO_MODE_CURRENT = -1,
*     AUDIO_MODE_NORMAL = 0,
*     AUDIO_MODE_RINGTONE = 1,
*     AUDIO_MODE_IN_CALL = 2,
*     AUDIO_MODE_IN_COMMUNICATION=3,
*     AUDIO_MODE_IN_VT_CALL= 4,
*     AUDIO_MODE_CNT,
*     AUDIO_MODE_MAX = AUDIO_MODE_CNT-1
 */
int mbtk_audio_mode_set(int mode);

/**
 *    device: UINT32
 *         0: earpiece
 *         1: speaker
 *         2: headset
 */
int mbtk_audio_loopback_start(int device);

int mbtk_audio_loopback_stop();
/**
 * @brief      mbtk_audio_dsp_gain_set
 *
 * @details    dsp gain set
 *
 * @param      param
 *             type: 0:tx
 *             0: tx
 *             1: rx
 *             gain: -36~12 db

 * @return     return 
 */
int mbtk_audio_dsp_gain_set(int type, int gain);

#endif /* _MBTK_AUDIO_UBUS_H */
