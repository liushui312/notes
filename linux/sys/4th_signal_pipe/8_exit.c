#include <stdio.h>
#include <stdlib.h>

void test(void)
{
    printf("test!\n");
}

void hello(void)
{
    printf("hello!\n");
}

int main(void)
{
    printf("main start!\n");
    atexit(test);
    atexit(hello);
    printf("main end!\n");

    /*_exit(0);*/
    /*_Exit(0);*/
    exit(0);
/*
 *    _exit(0);   //2
 *
 *    exit(0);   //3
 *    _Exit(0); //直接调用_exit
 */

    return 0;
}
