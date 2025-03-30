#include <stdio.h>

#include "mbtk_at.h"

#define LOGE printf
#define LOGI printf

int main(int argc, char *argv[])
{
    if(argc != 2) {
        LOGE("./at_test at_cmd\n");
        return -1;
    }

    if(mbtk_at_init()) {
        LOGE("mbtk_at_init() fail.\n");
        return -1;
    }

    char at_rsp[2048];
    if(!mbtk_at_send(argv[1], at_rsp, 2048)) {
        LOGI("%s", at_rsp);
    } else {
        LOGE("mbtk_at_send() fail.\n");
    }

    if(mbtk_at_deinit()) {
        LOGE("mbtk_at_deinit() fail.\n");
        return -1;
    }

    return 0;
}
