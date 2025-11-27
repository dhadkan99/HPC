#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

char *messages[3] = {NULL, NULL, NULL};
volatile int turn = 0;

void *messenger(void *p)
{
    intptr_t tid = (intptr_t)p;
    intptr_t next = (tid + 1) % 3;
    char buf[100];

    for (int i = 0; i < 10; i++)
    {
        while (turn != tid) 
            usleep(1000);

        while (messages[tid] == NULL)
            usleep(1000);

        if (!(tid == 0 && i == 0))
            printf("Thread %ld received: '%s'\n", (long)tid, messages[tid]);

        free(messages[tid]);
        messages[tid] = NULL;

        sprintf(buf, "Hello from Thread %ld!", (long)tid);
        messages[next] = strdup(buf);

        printf("Thread %ld sent message to Thread %ld\n", (long)tid, (long)next);

        turn = next;
    }

    return NULL;
}

int main()
{
    pthread_t thr[3];

    messages[0] = strdup("start");

    for (intptr_t i = 0; i < 3; i++)
        pthread_create(&thr[i], NULL, messenger, (void*)i);

    for (int i = 0; i < 3; i++)
        pthread_join(thr[i], NULL);

    return 0;
}
