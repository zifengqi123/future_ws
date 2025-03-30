/*******************************************************
*
* @brief: Add audio api
* @details:  add liblynq-qser-audio api
* @author:   yu.dong
* @date:     2023.9.28
* @version:  V1.0
* @copyright：Copyright (c) MobileTek
*
*********************************************/
#ifndef LYNQ_QSER_AUDIO
#define LYNQ_QSER_AUDIO

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*_cb_onPlayer)(int);

int qser_AudPlayer_Open(char* device, _cb_onPlayer cb_fun);
int qser_AudPlayer_PlayFrmFile(int hdl, const char *fd, int offset);
int qser_AudPlayer_PlayPcmBuf(const unsigned char *pcm_data, int data_size, int period_size,
                                int period_count, int num_channels, int sample_rate, int ownerid);
int qser_AudPlayer_Pause(int hdl);
int qser_AudPlayer_Resume(int hdl);
void qser_AudPlayer_Stop(int hdl);
void qser_AudPlayer_Close(int hdl);

int qser_AudRecorder_Open(char* device, _cb_onPlayer cb_fun);
int qser_AudRecorder_StartRecord(int hdl, const char *fd, int offset);
int qser_AudRecorder_StartRecord_Custom(char *file, int period_size, int period_count,
            int num_channels, int sample_rate);
int qser_AudRecorder_Pause(void);
int qser_AudRecorder_Resume(void);
void qser_AudRecorder_Stop(void);
void qser_AudRecorder_Close(void);

void qser_Audio_Deinit(void);

#ifdef __cplusplus
}
#endif
#endif

