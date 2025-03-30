#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "lynq_chip_id.h"

int main()
{
    char chip_id[128]={0};
    int ret = 0;
    ret = lynq_get_chip_id(chip_id);
    if(!ret)
    {
        printf("chip_id:%s",chip_id);
    }
    else{
        printf("get chip_id error");
    }
    return 0;
}



