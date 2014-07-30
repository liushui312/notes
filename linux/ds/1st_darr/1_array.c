#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int find(int key, int *a, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if (key == a[i]) 
            return i;
    }
    return -1; 
}
/*添加/修改/删除  查找 排序  存储 加载 遍历*/

int main(void)
{
    int n, c = 0, i;
    int *new = NULL, *prev = NULL;

    while (1)
    {
        printf("please input num : ");
        scanf("%d", &n);

        if (n == -1)
            break;

        new = malloc(sizeof(int) * (c + 1));
        memcpy(new, prev, c * sizeof(int));
        free(prev);
        /**(new + c) = n;*/
        /*new[c] = n;*/
        memcpy(new + c, &n, sizeof(int));
        prev = new;
        c++;
    }

    for (i = 0; i < c; i++)
    {
        printf("%d ", new[i]); 
    }
    printf("\n");

    free(new);

    return 0;
}
