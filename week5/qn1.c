#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

char *messages[3] = {NULL, NULL, NULL};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[3];
intptr_t turn = 0;

void *messenger(void *p)
{
    intptr_t tid = (intptr_t)p;
    intptr_t dest = (tid + 1) % 3;
    char tmpbuf[100];

    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&lock);

        while (turn != tid)
            pthread_cond_wait(&cond[tid], &lock);

        sprintf(tmpbuf, "Hello from Thread %ld!", (long)tid);
        messages[dest] = strdup(tmpbuf);
        printf("Thread %ld sent the message to Thread %ld\n", (long)tid, (long)dest);

        printf("Thread %ld received: '%s'\n", (long)tid, messages[tid]);
        free(messages[tid]);
        messages[tid] = NULL;

        turn = dest;
        pthread_cond_signal(&cond[turn]);

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main()
{
    pthread_t thr[3];

    for (int i = 0; i < 3; i++)
        pthread_cond_init(&cond[i], NULL);

    pthread_create(&thr[0], NULL, messenger, (void*)0);
    pthread_create(&thr[1], NULL, messenger, (void*)1);
    pthread_create(&thr[2], NULL, messenger, (void*)2);

    pthread_join(thr[0], NULL);
    pthread_join(thr[1], NULL);
    pthread_join(thr[2], NULL);

    return 0;
}
