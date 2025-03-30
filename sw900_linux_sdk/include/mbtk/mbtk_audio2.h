/*
* mbtk_audio2.h
*
* MBTK audio API for asr1806.
*
* Author : lb
* Date   : 2023/12/20 10:12:15
*/
#ifndef _MBTK_AUDIO2_H
#define _MBTK_AUDIO2_H
#include "mbtk_type.h"

#define MBTK_PCM_NB_BUF_SIZE 320
#define MBTK_PCM_WB_BUF_SIZE 640

typedef void (*mbtk_recorder_callback_func)(void *data, uint32 data_len);

typedef enum {
    MBTK_AUDIO_DIRECTION_OUTPUT = 0,    // Play
    MBTK_AUDIO_DIRECTION_INPUT         // Recorder
} mbtk_audio_direction_enum;

typedef enum {
    MBTK_AUDIO_SAMPLE_RATE_8000 = 0,
    MBTK_AUDIO_SAMPLE_RATE_16000
} mbtk_audio_sample_rate_enum;

int mbtk_audio_pcm_init();

int mbtk_audio_pcm_sample_rate_set(mbtk_audio_sample_rate_enum sample_rate);

int mbtk_audio_pcm_play_start();

int mbtk_audio_pcm_play_data_send(const void* data,uint32 data_len);

int mbtk_audio_pcm_play_stop();

int mbtk_audio_pcm_recorder_start(mbtk_recorder_callback_func recorder_cb);

int mbtk_audio_pcm_recorder_pause();

int mbtk_audio_pcm_recorder_resume();

int mbtk_audio_pcm_recorder_stop();

int mbtk_audio_pcm_deinit();

int mbtk_audio_wav_init();

int mbtk_audio_wav_play_start(const void *wav_file);

int mbtk_audio_wav_stream_play_start(const unsigned char *pcm_data, int data_size, int sample_rate, int num_channels);

int mbtk_audio_wav_play_pause();

int mbtk_audio_wav_play_resume();

int mbtk_audio_wav_play_stop();

int mbtk_audio_wav_recorder_start(const void *wav_file, mbtk_audio_sample_rate_enum sample_rate);

int mbtk_audio_wav_recorder_pause();

int mbtk_audio_wav_recorder_resume();

int mbtk_audio_wav_recorder_stop();

int mbtk_audio_wav_deinit();





/*
* Audio device enum.
*/
typedef enum {
    MBTK_AUTIO_TYPE_OUT = 0, /* Audio play device. */
    MBTK_AUTIO_TYPE_IN /* Audio record device. */
} mbtk_audio_dev_enum;

typedef void* mbtk_audio_handle;


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




mbtk_audio_handle mbtk_audio_open_new(mbtk_audio_dev_enum dev, int flag, int rate, void *usrData);
int mbtk_audio_play_file_new(void *dev_hdl, int file_fd, int offset);
int mbtk_audio_play_stream_new(void *dev_hdl, const void *pData, int len, int gain);

int mbtk_audio_close_new(void *dev_hdl);




#endif /* _MBTK_AUDIO2_H */
