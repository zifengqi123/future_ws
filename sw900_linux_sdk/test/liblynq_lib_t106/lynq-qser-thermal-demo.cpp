#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#include"lynq-qser-thermal-demo.h"


int (*get_thermal_zone)(int *numbers, int size);


int main(int argc, char *argv[]){
    int numbers[MAX_SIZE];
    int ret = 0;
    const char *lynq_libpath_thermal = "/lib/liblynq-qser-thermal.so";

    void *dlHandle_thermal = dlopen(lynq_libpath_thermal, RTLD_NOW);
    if (dlHandle_thermal == NULL) 
    {
        printf("dlopen dlHandle_thermal failed: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    get_thermal_zone = (int(*)(int *numbers, int size))dlsym(dlHandle_thermal,"get_thermal_zone");
    if(NULL != get_thermal_zone)
    {
        ret = get_thermal_zone(numbers, MAX_SIZE);
        if (ret <= 0) {
            printf("get_thermal_zone error\n");
            return -1;
    }
    }else{
            printf("get_thermal_zone dlsym error\n");
     }

    for (int j = 0; j < ret; ++j) {
        printf("[%s-%d] temp[%d] = %d \n", __func__, __LINE__, j, numbers[j]);
    }

    return 0;
}




