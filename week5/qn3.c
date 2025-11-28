#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#define N 3

char *messages[N] = {NULL, NULL, NULL};
pthread_mutex_t msg_lock[N];
pthread_mutex_t turn_lock;
int turn = 0;

void *messenger(void *p)
{
    intptr_t tid = (intptr_t)p;
    intptr_t next = (tid + 1) % N;
    char buf[100];

    for (int i = 0; i < 10; i++)
    {
        // wait until it's this thread's turn
        pthread_mutex_lock(&turn_lock);
        while (turn != tid) {
            pthread_mutex_unlock(&turn_lock);
            usleep(1000);
            pthread_mutex_lock(&turn_lock);
        }
        pthread_mutex_unlock(&turn_lock);

        // wait for my message
        pthread_mutex_lock(&msg_lock[tid]);
        while (messages[tid] == NULL) {
            pthread_mutex_unlock(&msg_lock[tid]);
            usleep(1000);
            pthread_mutex_lock(&msg_lock[tid]);
        }

        if (!(tid == 0 && i == 0))
            printf("Thread %ld received: '%s'\n", (long)tid, messages[tid]);

        free(messages[tid]);
        messages[tid] = NULL;
        pthread_mutex_unlock(&msg_lock[tid]);

        // send message to next thread
        sprintf(buf, "Hello from Thread %ld!", (long)tid);
        pthread_mutex_lock(&msg_lock[next]);
        messages[next] = strdup(buf);
        pthread_mutex_unlock(&msg_lock[next]);

        printf("Thread %ld sent message to Thread %ld\n", (long)tid, (long)next);

        pthread_mutex_lock(&turn_lock);
        turn = next;     // pass turn
        pthread_mutex_unlock(&turn_lock);
    }

    return NULL;
}

int main()
{
    pthread_t thr[N];

    for (int i = 0; i < N; i++)
        pthread_mutex_init(&msg_lock[i], NULL);
    pthread_mutex_init(&turn_lock, NULL);

    // initial message for thread 0
    pthread_mutex_lock(&msg_lock[0]);
    messages[0] = strdup("start");
    pthread_mutex_unlock(&msg_lock[0]);

    for (intptr_t i = 0; i < N; i++)
        pthread_create(&thr[i], NULL, messenger, (void*)i);

    for (int i = 0; i < N; i++)
        pthread_join(thr[i], NULL);

    return 0;
}
