#include <stdio.h>

#define MAX     100000

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

void bubble(void)
{
    int i, j;

    for (i = 0; i < MAX; i++)
    {
        for (j = 0; j < MAX - i - 1; j++) 
        {
            if (a[j] > a[j + 1])
            {
                a[j] ^= a[j + 1];
                a[j + 1] ^= a[j];
                a[j] ^= a[j + 1];
            }
        }
    }
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

void swaps(void)
{
    int i, j;

    for (i = 0; i < MAX - 1; i++)
    {
        for (j = i + 1; j < MAX; j++) 
        {
            if (a[i] > a[j]) 
            {
                a[i] ^= a[j];
                a[j] ^= a[i];
                a[i] ^= a[j];
            }
        }
    }
}


void insert(void)
{
    int i, j, t;

    for (i = 1; i < MAX; i++)
    {
        t = a[i];
        for (j = i; t < a[j - 1] && j > 0; j--) 
        {
            a[j] = a[j - 1];
        }
        a[j] = t;
    }
}

int cmp(const void *d1, const void *d2)
{
    return *(int *)d1 - *(int *)d2;
}

int main(int argc, char *argv[])
{
    rand_a();
    switch (atoi(argv[1]))
    {
        case 1:
            bubble();
            break;
        case 2:
            swaps();
            break;
        case 3:
            select();
            break;
        case 4:
            insert(); 
            break;
        case 5:
            qsort(a, MAX, sizeof(int), cmp);
            break;
        default:
            break;
    }
    /*show();*/
    /*insert();*/
    /*bubble();*/
    /*select();*/
    /*show();*/

    return 0;
}
