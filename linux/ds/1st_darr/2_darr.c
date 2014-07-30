#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *insert(void *data, void *prev, int *count, int size)
{
    void *new = NULL;
    int c = *count;

    new = malloc(size * (c + 1));
    memcpy(new, prev, c * size);
    free(prev);
    memcpy(new + c * size, data, size);
    c++;

    *count = c;

    return new;
}

int main(void)
{
    int c = 0, i;
    char *new = NULL;
    char n;

    while (1)
    {
        printf("please input num : ");
        scanf("%c", &n);

        while (getchar() != '\n')
            continue;

        if (n == 'q')
            break;

        new = insert(&n, new, &c, sizeof(char));
    }

    for (i = 0; i < c; i++)
    {
        printf("%c ", new[i]); 
    }
    printf("\n");

    free(new);

    return 0;
}
