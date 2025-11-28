#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

char *messages[3] = {NULL, NULL, NULL};
volatile int turn = 0;   // controls order of execution

void *messenger(void *p) {
    intptr_t tid = (intptr_t)p;
    intptr_t next = (tid + 1) % 3;
    char buf[100];

    for (int i = 0; i < 10; i++) {

        // Wait until it's THIS thread's turn
        while (turn != tid)
            usleep(1000);

        // Wait until a message is available for this thread
        while (messages[tid] == NULL)
            usleep(1000);

        // Do not print the very first message for Thread 0
        if (!(tid == 0 && i == 0))
            printf("Thread %ld received: '%s'\n", (long)tid, messages[tid]);

        free(messages[tid]);
        messages[tid] = NULL;

        // Create outgoing message
        sprintf(buf, "Hello from Thread %ld!", (long)tid);
        messages[next] = strdup(buf);

        printf("Thread %ld sent message to Thread %ld\n", (long)tid, (long)next);

        // Pass control to the next thread
        turn = next;
    }

    return NULL;
}

int main() {
    pthread_t thr[3];

    // Initial message to start thread 0
    messages[0] = strdup("start");

    for (intptr_t i = 0; i < 3; i++)
        pthread_create(&thr[i], NULL, messenger, (void*)i);

    for (int i = 0; i < 3; i++)
        pthread_join(thr[i], NULL);

    return 0;
}
