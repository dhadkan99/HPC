#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define USER_COUNT 10

sem_t printerSemaphore;

void* usePrinter(void* arg) {
    int userId = *(int*)arg;

    printf("User %d is waiting for a printer...\n", userId);

    sem_wait(&printerSemaphore);

    printf("User %d is USING a printer.\n", userId);
    sleep(1);

    printf("User %d has FINISHED printing.\n", userId);

    sem_post(&printerSemaphore);

    return NULL;
}

int main() {
    pthread_t threads[USER_COUNT];
    int userIds[USER_COUNT];

    sem_init(&printerSemaphore, 0, 2);

    for (int i = 0; i < USER_COUNT; i++) {
        userIds[i] = i;
        pthread_create(&threads[i], NULL, usePrinter, &userIds[i]);
    }

    for (int i = 0; i < USER_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&printerSemaphore);

    return 0;
}
