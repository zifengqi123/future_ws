#include <stdio.h>

#include "mbtk_log.h"
#include "mbtk_utils.h"


void test3()
{
    printf("%s start\n", __FUNCTION__);
    char *ptr = 10;
    *ptr = 'a';
    printf("%s end\n", __FUNCTION__);
}

void test2()
{
    printf("%s start\n", __FUNCTION__);
    mbtk_get_kernel_cmdline(NULL, 1024);
    // test3();
    printf("%s end\n", __FUNCTION__);
}

void test1()
{
    printf("%s start\n", __FUNCTION__);
    test2();
    printf("%s end\n", __FUNCTION__);
}

void* thread_function(void* arg) {
    // 模拟一个导致SIGSEGV的操作
    int* invalid_pointer = NULL;
    *invalid_pointer = 0; // 尝试写入一个无效的指针，将触发SIGSEGV
    return NULL;
}

int main(int argc, char *argv[])
{

#ifdef MBTK_DUMP_SUPPORT
    mbtk_debug_open(NULL, TRUE);
#endif

    test1();

    pthread_t thread;
    pthread_create(&thread, NULL, &thread_function, NULL);
    pthread_join(thread, NULL);

    printf("Exit.\n");

    return 0;
}

