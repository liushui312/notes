#include <stdio.h>

#define MAX     10

int a[MAX];

void rand_a(void)
{
    int i;

    for (i = 0; i < MAX; i++)
    {
        a[i] = rand() % MAX;
    }
}

void show(void)
{
    int i;

    for (i = 0; i < MAX; i++)
    {
        printf("%d ", a[i]); 
    }
    printf("\n");
}

void select(void)
{
    int i, j, min, index;

    for (i = 0; i < MAX - 1; i++)
    {
        min = a[i];
        index = i;
        for (j = i + 1; j < MAX; j++) 
        {
            if (min > a[j])
            {
                min = a[j];
                index = j;
            }
        }
        a[index] = a[i];
        a[i] = min;
    }
}

int my_bsearch(int key)
{
    int l = 0, h = MAX, m;

    while (l < h)
    {
        m = (l + h) / 2;
        if (key > a[m])
            l = m + 1;
        else if (key < a[m])
            h = m - 1;
        else
            return a[m];
    }
    return -1;
}

int main(int argc, char *argv[])
{
    int n;

    rand_a();
    show();
    select();
    show();

    printf("please input key : ");
    scanf("%d", &n);

    printf("find = %d\n", my_bsearch(n));

    return 0;
}
