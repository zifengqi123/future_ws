#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "audio_if_api.h"
#include "mbtk_audio2.h"

#define BUFF_SIZE 4096

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("mbtk_pcm_play_test <wav_file> <pack_size>\n");
        return -1;
    }

    mbtk_log_init("radio", "MBTK_AUDIO");

    if(mbtk_audio_pcm_init()) {
        printf("mbtk_audio_pcm_init() fail.\n");
        return -1;
    }

    int pack_size = atoi(argv[2]);
    if(pack_size <= 0) {
        pack_size = 1024;
    }

    int rc, len, fd, frames = 0;
    //char buf[MBTK_PCM_WB_BUF_SIZE];
    char buf[BUFF_SIZE];
    char *path = (char*)argv[1];
    struct stat st;
    struct riff_wave_header riff_wave_header;
    struct chunk_header chunk_header;
    struct chunk_fmt chunk_fmt = {0};
    unsigned int more_chunks = 1;

    /* Check and open source file */
    if (access(path, F_OK) || stat(path, &st)) {
        printf("%s: error reading from file %s\n", __FUNCTION__, path);
        return -1;
    }

    if (!st.st_size) {
        printf("%s: empty file %s\n", __FUNCTION__, path);
        return -1;
    }

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("%s: error opening file %s\n", __FUNCTION__, path);
        return -1;
    }

    read(fd, &riff_wave_header, sizeof(riff_wave_header));
    if ((riff_wave_header.riff_id != ID_RIFF) || (riff_wave_header.wave_id != ID_WAVE)) {
        printf("Error: '%s' is not a riff/wave file\n", path);
        close(fd);
        return -1;
    }

    do {
        read(fd, &chunk_header, sizeof(chunk_header));

        switch (chunk_header.id) {
            case ID_FMT:
                read(fd, &chunk_fmt, sizeof(chunk_fmt));
                /* If the format header is larger, skip the rest */
                if (chunk_header.sz > sizeof(chunk_fmt))
                    lseek(fd, chunk_header.sz - sizeof(chunk_fmt), SEEK_CUR);
                break;
            case ID_DATA:
                /* Stop looking for chunks */
                more_chunks = 0;
                break;
            default:
                /* Unknown chunk, skip bytes */
                lseek(fd, chunk_header.sz, SEEK_CUR);
        }
    } while (more_chunks);

    //Support 8k/16k & mono wave file
    if (((chunk_fmt.sample_rate != 8000) && (chunk_fmt.sample_rate != 16000))
            || (chunk_fmt.num_channels != 1) ) {
        printf("%s: error wave file:sample_rate = %d, num_channels = %d!! \n",
                __FUNCTION__,chunk_fmt.sample_rate, chunk_fmt.num_channels);
        close(fd);
        return -1;
    }

    printf("%s: success open wave file:%s, sample_rate = %d, num_channels = %d.\n",
            __FUNCTION__, path, chunk_fmt.sample_rate, chunk_fmt.num_channels);

    if ((8000 == chunk_fmt.sample_rate) && (1 == chunk_fmt.num_channels)) {
        mbtk_audio_pcm_sample_rate_set(MBTK_AUDIO_SAMPLE_RATE_8000);
    } else if ((16000 == chunk_fmt.sample_rate) && (1 == chunk_fmt.num_channels)) {
        mbtk_audio_pcm_sample_rate_set(MBTK_AUDIO_SAMPLE_RATE_16000);
    }

    if(mbtk_audio_pcm_play_start()) {
        printf("mbtk_audio_pcm_play_start() fail.\n");
        return -1;
    }

    printf("%s: starting playback %d bytes every 20ms.\n", __FUNCTION__, pack_size);

    while (TRUE) {
        /* Playback loop */
        memset(buf, 0x00, sizeof(buf));
        len = read(fd, buf, pack_size);
        if (len == -1) {
            printf("%s: error reading from file\n", __FUNCTION__);
            break;
        }

        if (len == 0) {
            /* reached EOF */
            printf("%s: nothing to read\n", __FUNCTION__);
            break;
        }

        if((rc = mbtk_audio_pcm_play_data_send(buf, len)) < len) {
            printf("Send data %d/%d\n", rc, len);
            break;
        }

        printf("%s: No.%d frame playback[len - %d]\n", __FUNCTION__, ++frames, len);
    }

    if(mbtk_audio_pcm_deinit()) {
        printf("mbtk_audio_pcm_deinit() fail.\n");
        return -1;
    }

    return 0;
}

