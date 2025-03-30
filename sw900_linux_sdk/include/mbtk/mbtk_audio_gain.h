/*
* mbtk_audio_gain.h
*
* MBTK audio gain for nvm : /NVM/audio_gain.nvm.
*
* Author : lb
* Date   : 2024/3/1 16:04:46
*/
#ifndef _MBTK_AUDIO_GAIN_H
#define _MBTK_AUDIO_GAIN_H
#include "mbtk_type.h"

#define MBTK_AUDIO_GAIN_ADD_START 0x0108
#define MBTK_AUDIO_GAIN_MIN -36
#define MBTK_AUDIO_GAIN_MAX 12
#define CONFIG_DSPGAIN_MUTE_ON  (-100)
#define CONFIG_DSPGAIN_MUTE_OFF  (100)

#define MBTK_AUDIO_GAIN_PATH "/NVM/audio_gain.nvm"

typedef enum
{
	CONFIG_DSPGAIN_TX = 0, /* TX: near to far end */
	CONFIG_DSPGAIN_RX, /* RX: far to near end */
//	CONFIG_DSPGAIN_SIDETONE,
//	CONFIG_DSPGAIN_NUMBER = CONFIG_DSPGAIN_SIDETONE
} CONFIG_DSPGAIN_Direction;

typedef enum
{
     VC_HANDSET = 0,
     VC_HANDSFREE,
     VC_HEADSET,
     VC_HEADPHONE,

     VC_HEAD_NUM
} AUDIO_PROFILE_ID;

typedef enum
{
    AUDIOHAL_SPK_MUTE    = 0,
    AUDIOHAL_SPK_VOL_1,
    AUDIOHAL_SPK_VOL_2,
    AUDIOHAL_SPK_VOL_3,
    AUDIOHAL_SPK_VOL_4,
    AUDIOHAL_SPK_VOL_5,
    AUDIOHAL_SPK_VOL_6,
    AUDIOHAL_SPK_VOL_7,
    AUDIOHAL_SPK_VOL_8,
    AUDIOHAL_SPK_VOL_9,
    AUDIOHAL_SPK_VOL_10,
    AUDIOHAL_SPK_VOL_QTY //11
}  AUDIOHAL_SPK_LEVEL_T;

typedef struct
{
    uint16  PGA_Stage1;
    uint16  PGA_Stage2;
    uint16  ADC_DigGain;
    uint16  SideTone_Gain;
    uint16  Reserved1;
    uint16  Reserved2;
}  ACMCodec_GainInT;

typedef struct
{
    uint16  DAC_DigGain;
    uint16  DAC_Gain;
    uint16  RCV_Gain;
    uint16  ClassG_Mode;
    uint16  ClassD_Gain;
    uint16  Reserved;
}  ACMCodec_GainOutT;

typedef struct{
    AUDIO_PROFILE_ID    Profile_id;
    uint16      CodecControl;
    ACMCodec_GainInT    Tx_CodecGain; //TX has the same gain for every volume
    int16               Tx_DSPGain;   //TX has the same gain for every volume, High 8bits: WB gain, Low 8bits: NB gain
    ACMCodec_GainOutT   Rx_CodecGain[AUDIOHAL_SPK_VOL_QTY + 1];
    int16               Rx_DSPGain[AUDIOHAL_SPK_VOL_QTY + 1];   //High 8bits: WB gain, Low 8bits: NB gain
    int16               Rx_DSPSideToneGain;                 //High 8bits: WB gain, Low 8bits: NB gain
} ACMCodec_GainT;

int mbtk_dsp_gain_set(CONFIG_DSPGAIN_Direction dir, int gain);

int mbtk_dsp_gain_get(int *rx_gain, int *tx_gain);

#endif /* _MBTK_AUDIO_GAIN_H */
