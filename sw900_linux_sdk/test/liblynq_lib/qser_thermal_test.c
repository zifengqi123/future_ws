#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include"lynq_qser_thermal.h"
#define MAX_SIZE 3

int main(int argc, char *argv[]){
    int numbers[MAX_SIZE];
    int ret = 0;
    ret = get_thermal_zone(numbers, MAX_SIZE);
    if (ret <= 0)
    {
        printf("get_thermal_zone error\n");
        return -1;
    }

    for (int j = 0; j < ret; ++j) {
        printf("[%s-%d] temp[%d] = %d \n", __func__, __LINE__, j, numbers[j]);
    }

    return 0;
}

