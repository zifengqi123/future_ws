#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pub_int_d = 4;
char *str = "abc";
int bss_1;

//int bss_1;
//static int static_bss_2;

void test(int c)
{
    int d = 10;
    //printf("函数参数：test_c = %p, 局部变量：d = %p\n", &c, &d);
}

int main(int argc, char *argv[])
{
    printf("[栈]函数参数：argc = %p, argv = %p\n", &argc, argv);

    int int_a;
    static int static_int_b;
    char *temp_malloc = (char*)malloc(10);
    const char temp[10];

    printf("[栈]局部变量：int_a[%d] = %p, [BSS]局部静态变量：static_int_b[%d] = %p\n", int_a, &int_a, static_int_b, &static_int_b);
    printf("[DATA]全局变量：pub_int_d[%d] = %p\n", pub_int_d, &pub_int_d);
    printf("常量：str = %p, 堆空间：temp_malloc = %p\n", str, temp_malloc);
    printf("函数：test_func = %p\n", test);
    printf("const_str = %p, &(temp[3]) = %p\n", temp, &(temp[3]));
    printf("BSS : %d, %p\n", bss_1, &bss_1);

    test(5);

    while(1) {
        sleep(24 * 60 * 60);
    }

    return 0;
}

