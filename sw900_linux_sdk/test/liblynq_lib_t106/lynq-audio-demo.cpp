#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "ring_tele.h"
#include <lynq/lynq-qser-audio.h>


static int playback_handle = 1;

void player_cmd_proc(char *cmdstr)
{
    if (strcmp(cmdstr, "P\n") == 0)
    {
        qser_AudPlayer_Pause(playback_handle);
    }
    else if (strcmp(cmdstr, "R\n") == 0)
    {
        qser_AudPlayer_Resume(playback_handle);
    }
    else if (strcmp(cmdstr, "T\n") == 0)
    {
        qser_AudPlayer_Stop(playback_handle);
    }
    else
    {
        printf("Unknown command: %s", cmdstr);
    }
}

void capture_cmd_proc(char *cmdstr)
{
    if (strcmp(cmdstr, "P\n") == 0)
    {
        qser_AudRecorder_Pause();
    }
    else if (strcmp(cmdstr, "R\n") == 0)
    {
        qser_AudRecorder_Resume();
    }
    else if (strcmp(cmdstr, "T\n") == 0)
    {
        qser_AudRecorder_Stop();
    }
    else
    {
        printf("Unknown command: %s", cmdstr);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <play|recd|playbuf> [file]\n", argv[0]);
        return 1;
    }

    const char *action = argv[1];
    const char *file = argv[2];

    int g_audio_owner_id = 0;
    char player_device[] = "device1";
    char recorder_device[] = "device2";
    char cmdstr[256];

    _cb_onPlayer cb_fun = [](int result)
    {
        if (result == 0)
        {
            printf("Audio recorder opened successfully.\n");
        }
        else
        {
            printf("Failed to open audio recorder, error code: %d\n", result);
        }
    };

    if (strcmp(action, "playbuf") == 0)
    {
        int player_open_result = qser_AudPlayer_Open(player_device, cb_fun);
        if (player_open_result != 0)
        {
            printf("Failed to open audio player.\n");
            return 1;
        }
        qser_AudPlayer_PlayPcmBuf(PCM_DATA, PCM_DATA_SIZE, 640, 3, 1, 8000, g_audio_owner_id);
        while (1)
        {
            printf("Please input a player command (P/R/T/exit) :\n");
            if (fgets(cmdstr, sizeof(cmdstr), stdin) != NULL)
            {
                if (strcmp(cmdstr, "exit\n") == 0)
                {
                    qser_AudPlayer_Close(playback_handle);
                    break;
                }
                player_cmd_proc(cmdstr);
            }
        }
        qser_AudPlayer_Close(playback_handle);
    }
    else if (strcmp(action, "play") == 0)
    {
        int player_open_result = qser_AudPlayer_Open(player_device, cb_fun);
        if (player_open_result != 0)
        {
            printf("Failed to open audio player.\n");
            return 1;
        }

        qser_AudPlayer_PlayFrmFile(g_audio_owner_id, file, 0);

        while (1)
        {
            printf("Please input a player command (P/R/T/exit) :\n");
            if (fgets(cmdstr, sizeof(cmdstr), stdin) != NULL)
            {
                if (strcmp(cmdstr, "exit\n") == 0)
                {
                    qser_AudPlayer_Close(playback_handle);
                    break;
                }
                player_cmd_proc(cmdstr);
            }
        }

        qser_AudPlayer_Close(playback_handle);
    }
    else if (strcmp(action, "recd") == 0)
    {
        int recorder_open_result = qser_AudRecorder_Open(recorder_device, cb_fun);
        if (recorder_open_result != 0) {
            printf("Failed to open audio recorder.\n");
            return 1;
        }

        qser_AudRecorder_StartRecord(g_audio_owner_id, file, 0);

        while (1)
        {
            printf("Please input a player command (P/R/T/exit) :\n");
            if (fgets(cmdstr, sizeof(cmdstr), stdin) != NULL)
            {
                if (strcmp(cmdstr, "exit\n") == 0)
                {
                    qser_AudRecorder_Close();
                    break;
                }
                capture_cmd_proc(cmdstr);
            }
        }
        qser_AudRecorder_Close();
    }
    else
    {
        printf("Unknown action: %s\n", action);
        return 1;
    }

    qser_Audio_Deinit();

    return 0;
}
