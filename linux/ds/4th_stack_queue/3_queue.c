#include <stdio.h>

#define MAX     10

int queue[MAX];
int front = 0;
int end = 0;

int is_empty(void)
{
    if (front == end)
        return 1;
    return 0;
}

int is_full(void)
{
    if ((end + 1) % MAX == front)
        return 1;
    return 0;
}

int en(int data)
{
    if (is_full())
        return -1;

    queue[end++] = data;
    /*end %= MAX;*/
    if (end == MAX)
        end = 0;

    return 0;
}

int de(void)
{
    int data;

    if (is_empty())
        return -1;

    data = queue[front++];
    front %= MAX;
    if (front == MAX)
        front;

    return data;
}

void travel(void)
{
    int i, len, j;

    if (end > front)
        len = end - front;
    else
        len = MAX - (front - end);

    for (i = 0, j = front; i < len; i++, j++)
    {
        j %= MAX;
        printf("%d ", queue[j]); 
    }
}

int main(void)
{
    int i, n;

    for (i = 0; i < MAX * 2; i++)
    {
        n = rand() % 100;
        printf("%d ", n);
        en(n);
    }
    printf("\n");

    for (i = 0; i < MAX * 2; i++)
    {
        n = de();
        printf("%d ", n);
    }
    printf("\n");

    return 0;
}
