#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "mbtk_log.h"
#include "mbtk_type.h"

// RGB565
#define COLOR_BLACK 0x0000
#define COLOR_WRITE 0xFFFF
#define DEV_FB_PATH "/dev/fb0"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGTH 240

typedef struct {
    int left;
    int top;
    int width;
    int heigth;
} rect_t;

static uint16 fb_buffer[SCREEN_WIDTH * SCREEN_HEIGTH];

static int fb_refresh(int fd)
{
    rect_t rect;
    rect.width = SCREEN_WIDTH / 2;
    rect.heigth = SCREEN_HEIGTH / 2;
    rect.left = (SCREEN_WIDTH - rect.width) / 2;
    rect.top = (SCREEN_HEIGTH - rect.heigth) / 2;
    // Fill in buffer.
    int x,y;
    for(y = 0; y < SCREEN_HEIGTH; y++) {
        for(x = 0; x < SCREEN_WIDTH; x++) {
            if(x >= rect.left && x <= rect.left + rect.width
                && y >= rect.top && y <= rect.top + rect.heigth)
            {
                fb_buffer[x * SCREEN_HEIGTH + y] = COLOR_WRITE;
            } else {
                fb_buffer[x * SCREEN_HEIGTH + y] = COLOR_BLACK;
            }
        }
    }

    int len = write(fd, fb_buffer, sizeof(fb_buffer));
    LOGD("Write : %d/%d", len, sizeof(fb_buffer));
    // Write buffer to framebuffer.
    if(sizeof(fb_buffer) != len) {
        LOGE("Write fail:%d", errno);
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if(access(DEV_FB_PATH, F_OK) != 0) {
        LOGE("no %s, quit.", DEV_FB_PATH);
        return -1;
    }

    int fb_fd = open(DEV_FB_PATH, O_RDWR);
    if(fb_fd < 0) {
        LOGE("open() fail:%d", errno);
        return -1;
    }

    // Fresh framebuffer
    while(1) {
        if(fb_refresh(fb_fd)) {
            break;
        }

        usleep(33); // 1000 / 30
    }

    LOGD("Exit");
    return 0;
}


