#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

#include "mbtk_audio2.h"
#include "mbtk_log.h"

static void sig_handler(int sig)
{
    if(mbtk_audio_pcm_recorder_stop()) {
        printf("mbtk_audio_pcm_recorder_stop() fail.\n");
        //goto exit;
    }

    if(mbtk_audio_pcm_deinit()) {
        printf("mbtk_audio_pcm_deinit() fail.\n");
    }

    printf("Success exit by signal...\n");
    exit(0);
}

static void recorder_cb(void *data, uint32 data_len)
{
    if(data_len > 0) {
        LOGD("Recorver data:%d", data_len);
    } else {
        LOGD("Recorver data end.");
    }
}

int main(int argc, char *argv[])
{
    if(argc != 1) {
        printf("mbtk_pcm_recorder_test\n");
        return -1;
    }

    mbtk_log_init("radio", "MBTK_AUDIO");

    if(mbtk_audio_pcm_init()) {
        printf("mbtk_audio_pcm_init() fail.\n");
        return -1;
    }

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    char cmd[100];
    bool running = TRUE;
    while(running)
    {
        memset(cmd, 0, 100);
        int err;
        printf("1 : Recorder    2 : Pause    3 : Resume    4 : Stop    Other : Exit\n");
        if(fgets(cmd, 100, stdin))
        {
            if(cmd[0] == '\r' || cmd[0] == '\n')
                continue;
            int state = atoi(cmd);
            switch(state){
                case 1:
                {
                    if(mbtk_audio_pcm_recorder_start(recorder_cb)) {
                        printf("mbtk_audio_pcm_recorder_start() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                case 2:
                {
                    if(mbtk_audio_pcm_recorder_pause()) {
                        printf("mbtk_audio_pcm_recorder_pause() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                case 3:
                {
                    if(mbtk_audio_pcm_recorder_resume()) {
                        printf("mbtk_audio_pcm_recorder_resume() fail.\n");
                        //goto exit;
                    }
                    break;
                }
                case 4:
                {
                    if(mbtk_audio_pcm_recorder_stop()) {
                        printf("mbtk_audio_pcm_recorder_stop() fail.\n");
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
    if(mbtk_audio_pcm_deinit()) {
        printf("mbtk_audio_pcm_deinit() fail.\n");
        return -1;
    }

    printf("Success exit.\n");
    return 0;
}



