#include <stdio.h>
#include <strings.h>
#include<stdlib.h>
#include "lynq/lynq_uci.h"




int main(int argc, const char *argv[])
{
    int ret = 0;
    ret = lynq_set_netled_on(atoi(argv[1]));
    if(ret){
        printf("[%s-%d] lynq_set_netled_on error\n", __FUNCTION__, __LINE__);
        return -1;
    }
    ret = lynq_set_statusled_on(atoi(argv[1]));
    if(ret){
        printf("[%s-%d] lynq_set_statusled_on error\n", __FUNCTION__, __LINE__);
        return -1;
    }
    
    return 0;
}
