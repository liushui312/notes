#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *args[] = {"aa", "bb", "cc", NULL};
    char *env[] = {"aa = AA", "bb = BB", "cc = CC", NULL};
    int i;

    printf("main pid = %d, %d\n", getpid(), getppid());

/*
 *    for (i = 0; i < argc; i++)
 *    {
 *        printf("%s\n", argv[i]);
 *    } 
 *
 *    getchar();
 */

    if (fork() == 0)
    {
        setuid(1000);
        execve("./test", args, env);
        /*execv execvp execl execlp execle*/
        /*execl("./test", "./test", "aa", "bb", NULL);*/
        /*execlp("ls", "ls", "-l", "-t", "-h", NULL);*/
        /*
         *v   程序参数以指针数组形式传入
         *l   程序参数以函数可变参形式传入
         *e   支持自定义传入环境变量
         *p   执行的程序会查找PATH环境变量
         */

        perror("execve");
        exit(0);
    }

    printf("main end!\n");

    return 0;
}
