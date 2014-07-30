#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int find_num(int lev)
{
    int i, j, bit[6];
    int k = 0, num = 0;

    for (i = 500 * (lev + 2); i < 500 * (lev + 3) - 1; i++ )
    {
        j = i;
        k = 0;
        while (j != 0)                
        {
            bit[k++] = j % 10; 
            j /= 10;
        }

        if (bit[0] == bit[3] && bit[1] == bit[2])
            num++;
    }

    return num;
}

int main(void)
{
    int i = 0, num = 0, sum = 0;
    int status;
    pid_t id[10];

    for (i = 0; i < 10; i++)
    {
        if ((id[i] = fork()) == -1) 
        {
            perror("fork");
            exit(0);
        }
        
        if (id[i] == 0)
        {
            num = find_num(i);
            exit(num);
        }
    }
    
    for (i = 0; i < 10; i++)
    {
        waitpid(id[i], &status, 0);
        sum += WEXITSTATUS(status);
    }

    printf("sum = %d\n", sum);

    return 0;
}
