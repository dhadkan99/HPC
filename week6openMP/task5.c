#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>

char *messages[3] = { NULL, NULL, NULL };

int main() {

    #pragma omp parallel num_threads(3) shared(messages)
    {
        int tid = omp_get_thread_num();
        char buffer[100];

        for (int round = 0; round < 10; round++) {

            if (tid == 0) {
                sprintf(buffer, "Hello from Thread 0!");
                messages[1] = strdup(buffer);
                printf("Thread 0 sent the message\n");
            }

            #pragma omp barrier

            if (tid == 1 && messages[1] != NULL) {
                printf("Thread 1 received: %s\n", messages[1]);
                free(messages[1]);
                messages[1] = NULL;

                sprintf(buffer, "Hello from Thread 1!");
                messages[2] = strdup(buffer);
                printf("Thread 1 sent the message\n");
            }

            #pragma omp barrier

         
            if (tid == 2 && messages[2] != NULL) {
                printf("Thread 2 received: %s\n", messages[2]);
                free(messages[2]);
                messages[2] = NULL;

                sprintf(buffer, "Hello from Thread 2!");
                messages[0] = strdup(buffer);
                printf("Thread 2 sent the message\n");
            }

            #pragma omp barrier

            
            if (tid == 0 && messages[0] != NULL) {
                printf("Thread 0 received: %s\n", messages[0]);
                free(messages[0]);
                messages[0] = NULL;
            }

            #pragma omp barrier
        }
    }

    return 0;
}
