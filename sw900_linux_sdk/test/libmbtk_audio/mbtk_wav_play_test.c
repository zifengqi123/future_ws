#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

#include "mbtk_audio2.h"
#include "mbtk_audio_ubus.h"

static void sig_handler(int sig)
{
    if(mbtk_audio_wav_play_stop()) {
        printf("mbtk_audio_wav_play_stop() fail.\n");
        //goto exit;
    }

    if(mbtk_audio_wav_deinit()) {
        printf("mbtk_audio_wav_deinit() fail.\n");
    }

    printf("Success exit by signal...\n");
    exit(0);
}


int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("mbtk_wav_play_test <wav_file>\n");
        return -1;
    }

    mbtk_log_init("radio", "MBTK_AUDIO");

    if(mbtk_audio_wav_init()) {
        printf("mbtk_audio_wav_init() fail.\n");
        return -1;
    }

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    char cmd[100];
    int dsp=0;
    bool running = TRUE;
    while(running)
    {
        memset(cmd, 0, 100);
        int err;
        printf("1 : Play    2 : Pause    3 : Resume   4 : dsp set  5 : Stop    Other : Exit\n");
        if(fgets(cmd, 100, stdin))
        {
            if(cmd[0] == '\r' || cmd[0] == '\n')
                continue;
            int state = atoi(cmd);
            switch(state){
                case 1:
                {
                    if(mbtk_audio_wav_play_start(argv[1])) {
                        printf("mbtk_audio_wav_play_start() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                case 2:
                {
                    if(mbtk_audio_wav_play_pause()) {
                        printf("mbtk_audio_wav_play_pause() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                case 3:
                {
                    if(mbtk_audio_wav_play_resume()) {
                        printf("mbtk_audio_wav_play_resume() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                case 4:
                {
			mbtk_audio_ubus_init();
                    if(mbtk_audio_dsp_gain_set(1, -15)) {
                        printf("mbtk_audio_dsp_gain_set() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                case 5:
                {
                    if(mbtk_audio_wav_play_stop()) {
                        printf("mbtk_audio_wav_play_stop() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                default:
                {
                    running = FALSE;
                    break;
                }
            }
        }
    }

exit:
    if(mbtk_audio_wav_deinit()) {
        printf("mbtk_audio_wav_deinit() fail.\n");
        return -1;
    }

    printf("Success exit.\n");
    return 0;
}


