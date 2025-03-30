/**
 *   \file mbtk_audio_alsa.h
 *   \brief A Documented file.
 *
 *  Detailed description
 *   Audio API definition.
 *
 *   \Author:  wangjinshu <js.wang@mobiletek.cn>
 *   \Version: 1.0.0
 *   \Date: 2020.04.05
 */

#ifndef __MBTK_AUDIO_ALSA_H__
#define __MBTK_AUDIO_ALSA_H__
#include <pthread.h>
#include "mbtk_type.h"

typedef void* mbtk_audio_handle;
typedef void(*mbtk_audio_record_cb_func)(int result, char* databuf, unsigned int len); //callback function get recording data
typedef int(*mbtk_dtmf_cb)(char result);
typedef int(*mbtk_volume_cb)(int result);
typedef uint32 mbtk_audio_client_handle_type;

/*
* Audio device enum.
*/
typedef enum {
    MBTK_AUTIO_TYPE_OUT = 0, /* Audio play device. */
    MBTK_AUTIO_TYPE_IN /* Audio record device. */
} mbtk_audio_dev_enum;

/*
* Audio running state..
*/
typedef enum {
    AUDIO_NONE = -1,
    AUDIO_STOP = 0,
    AUDIO_OPEN = 1,
    AUDIO_RUNNING = 2,
    AUDIO_PAUSE = 3,
    AUDIO_RESUME = 4
} mbtk_audio_state_enum;

// 0-Tx_CodecGain, 1-Tx_DSPGain, 2-Rx_CodecGain, 3-Rx_DSPGain
typedef enum {
    AUDIO_GAIN_TYPE_TX_CODECGAIN = 0,
    AUDIO_GAIN_TYPE_TX_DSPGAIN,
    AUDIO_GAIN_TYPE_RX_CODECGAIN,
    AUDIO_GAIN_TYPE_RX_DSPGAIN
} mbtk_audio_gain_type_enum;

/****************************************************************************
*  Audio function
***************************************************************************/
/*===========================================================================
FUNCTION mbtk_audio_open

DESCRIPTION:
  Initial MBTK audio interface.

PARAMETERS:
  dev [IN]: Audio device, refer for mbtk_audio_dev_enum.
  channel [IN]: Audio channel number.
  rate [IN]: Audio bite rate.
  usrData [IN]: Additional data, usually specified as NULL.

RETURN VALUE:
  mbtk_audio_handle : Return audio handle if success, NULL for failure.

===========================================================================*/
mbtk_audio_handle mbtk_audio_open(mbtk_audio_dev_enum dev, int channels, int rate, void* usrData);


/*===========================================================================
FUNCTION mbtk_audio_play_stream

DESCRIPTION:
  Play audio stream.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.
  pdata [IN]: Audio stream data.
  len [IN]: Audio stream data length.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_audio_play_stream(mbtk_audio_handle dev_hdl, const void* pdata, int len);


/*===========================================================================
FUNCTION mbtk_audio_play_file

DESCRIPTION:
  Play audio file.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.
  file_fd [IN]: File descriptor to play.
  offset [IN]: File offset.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_audio_play_file(mbtk_audio_handle dev_hdl, int file_fd, int offset);

/*===========================================================================
FUNCTION mbtk_audio_record

DESCRIPTION:
  Start audio record.This function will call cb_func in the new thread to return record data.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.
  cb_func [IN]: Audio record callback function.
  cb_date [IN]: Audio record additional data,usually specified as NULL.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_audio_record(mbtk_audio_handle dev_hdl, mbtk_audio_record_cb_func cb_func, void* cb_date);
int mbtk_audio_play_waitend(void* hdl);

/*===========================================================================
FUNCTION mbtk_audio_pause

DESCRIPTION:
  Audio play pause.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_audio_pause(mbtk_audio_handle dev_hdl);


/*===========================================================================
FUNCTION mbtk_audio_resume

DESCRIPTION:
  Audio play resume.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_audio_resume(void* dev_hdl);
int mbtk_audio_prepare(void* dev_hdl);
/*===========================================================================
FUNCTION mbtk_audio_state_get

DESCRIPTION:
  Get audio play state.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.

RETURN VALUE:
  mbtk_audio_state_enum : Return audio state,refer to mbtk_audio_state_enum.

===========================================================================*/
mbtk_audio_state_enum mbtk_audio_state_get(void *hdl);


/*===========================================================================
FUNCTION mbtk_audio_stop

DESCRIPTION:
  Stop audio play.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_audio_stop(void* dev_hdl);


/*===========================================================================
FUNCTION mbtk_audio_resume

DESCRIPTION:
  Close audio handle.

PARAMETERS:
  dev_hdl [IN]: Audio handle, get by call mbtk_audio_open.

RETURN VALUE:
  int : Return 0 if success,other for failure.

===========================================================================*/
int mbtk_audio_close(void* dev_hdl);


int mbtk_audio_ubus_client_init(mbtk_audio_client_handle_type *ph_dtmf, mbtk_dtmf_cb cb);
int mbtk_audio_ubus_client_deinit(mbtk_audio_client_handle_type h_dtmf);
void mbtk_audio_ubus_volume_set(unsigned int volume);
void mbtk_audio_ubus_volume_get(mbtk_volume_cb cb);
int mbtk_audio_dsp_set(int type, int gain);
void mbtk_audio_ubus_gain_set(mbtk_audio_gain_type_enum type, uint16 *gain, int gain_num);
void mbtk_audio_ubus_reg_set(int reg_addr, int reg_value);

/**
 * @brief      mbtk_audio_switch_pcm
 *
 * @details    Turn on/off MSA PCM
 *
 * @param      param
 *             "param0":UINT32 mode
 *             0: Turn off MSA PCM
 *             1: Turn on MSA PCM as NB PCM(i.e. 8KHz sample rate)
 *             2: Turn on MSA PCM as WB PCM(i.e. 16KHz sample rate)
 * @return     return type
 */
void mbtk_audio_switch_pcm(int mode);

/**
 * @brief      mbtk_audio_mode_set
 *
 * @details    audio_mode_set
 *
 * @param      param
 *             "param0": UINT32
 *             typedef enum {
 *                 AUDIO_MODE_INVALID = -2,
 *                 AUDIO_MODE_CURRENT = -1,
 *                 AUDIO_MODE_NORMAL = 0,
 *                 AUDIO_MODE_RINGTONE = 1,
 *                 AUDIO_MODE_IN_CALL = 2,
 *                 AUDIO_MODE_IN_COMMUNICATION=3,
 *                 AUDIO_MODE_IN_VT_CALL= 4,
 *                 AUDIO_MODE_CNT,
 *                 AUDIO_MODE_MAX = AUDIO_MODE_CNT-1,
 *             } audio_mode_
 * @return     return type
 */
void mbtk_audio_mode_set(int mode);

/**
 * @brief      mbtk_audio_path_enable
 *
 * @details    first set the audio_mode_set ,then need set it
 *
 * @param      param
 *             "param0": UINT32
 *             1: enable
 *             1: disable
 * @return     return type
 */
void mbtk_audio_path_enable(int mode);

int mbtk_audio_get_status(void* hdl);


int mbtk_audio_mp3_to_wav(const char *wavpath, char *mp3path);
int mbtk_audio_mp3_to_play(char *mp3path, int hdl, int sample_rate);
#endif /*__MBTK_AUDIO_ALSA_H__*/
