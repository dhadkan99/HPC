#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

char *messages[3] = {NULL, NULL, NULL};
sem_t sem[3];

void *messenger(void *p)
{
    intptr_t tid = (intptr_t)p;
    int next = (tid + 1) % 3;
    char tmpbuf[100];

    for (int i = 0; i < 10; i++)
    {
        sem_wait(&sem[tid]);

        if (!(tid == 0 && i == 0))
        {
            printf("Thread %ld received the message '%s'\n", (long)tid, messages[tid]);
            free(messages[tid]);
            messages[tid] = NULL;
        }

        sprintf(tmpbuf, "Hello from Thread %ld! (Round %d)", (long)tid, i);
        messages[next] = strdup(tmpbuf);
        printf("Thread %ld sent the message to Thread %d\n", (long)tid, next);

        sem_post(&sem[next]);
    }

    return NULL;
}

int main()
{
    pthread_t threads[3];

    sem_init(&sem[0], 0, 1);
    sem_init(&sem[1], 0, 0);
    sem_init(&sem[2], 0, 0);

    messages[0] = strdup("start");

    for (intptr_t i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, messenger, (void*)i);

    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
