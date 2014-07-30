#include <stdio.h>
#include <pthread.h>

pthread_t id;

int a = 33;

void clean(void *data)
{
    printf("clean %d\n", (int)data);
}

void *do_work(void *data)
{
    pthread_cleanup_push(clean, (void *)1111);
    pthread_cleanup_push(clean, (void *)2222);
    pthread_cleanup_push(clean, (void *)3333);
    pthread_cleanup_push(clean, (void *)4444);
    pthread_cleanup_push(clean, (void *)5555);

    if (pthread_equal(pthread_self(), id))
    {
        printf("equal!\n");
        sleep(3);
        goto EXIT;
        /*pthread_exit(&a);*/
    }

    while (1)
    {
        printf("do_work %d!\n", (int)data);
        sleep(1);
    }
EXIT:
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);

    return NULL;
}

int main(void)
{
    pthread_t id1;
    int ret;
    int *p = NULL;

    ret = pthread_create(&id, NULL, do_work, (void *)1111);
    if (ret == -1)
        perror("pthread_create");

    ret = pthread_create(&id1, NULL, do_work, (void *)22222);
    if (ret == -1)
        perror("pthread_create");

    pthread_join(id, (void **)&p);
    if (p != NULL)
        printf("p = %d\n", *p);

    pthread_cancel(id1);

    getchar();

    return 0;
}
