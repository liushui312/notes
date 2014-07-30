#include <queue.h>

int main(void)
{
    QUEUE *man = NULL, *women = NULL;
    char name[20], *p = NULL;
    int sex, len, i, j;

    man = queue_create(20, 100);
    women = queue_create(20, 100);

    while (1)
    {
        printf("请输入姓名, 性别(1为男,0为女,-1为退出): ");
        scanf("%s%d", name, &sex);

        if (sex == -1)
            break;
        else if (sex == 0)
            queue_en(name, women);
        else
            queue_en(name, man);
    }

    len = queue_len(women) > queue_len(man) ? queue_len(man): queue_len(women);

    for (i = 0; i < 3; i++)
    {
        printf("第%d轮 : ", i + 1);
        for (j = 0; j < len; j++)
        {
            p = queue_de(man);
            printf("%s <==> ", p);
            queue_en(p, man);

            p = queue_de(women);
            printf("%s ", p);
            queue_en(p, women);
        }
        printf("\n");
    }

    queue_destroy(&man);
    queue_destroy(&women);

    return 0;
}
