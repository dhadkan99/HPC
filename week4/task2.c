#include  <pthread.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>

void *func1(void *p) {
    while (1)
    {
        printf("Thread 1 is running...\n");
        sleep(1);
        pthread_testcancel();
    }
    return NULL;
    
}

void *func2(void *p) {
    while (1)
    {
        printf("Thread 2 is running...\n");
        sleep(1);
        pthread_testcancel();
    }
    return NULL;
    
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, func1, NULL);
    pthread_create(&thread2, NULL, func2, NULL);

    sleep(5);

    printf("request to cancel threads\n");
    pthread_cancel(thread2);
    pthread_join(thread2, NULL);
    
    printf("Thread 2 has been canceled\n");
    sleep(5);

    printf("request to cancel thread 1\n");
    pthread_cancel(thread1);
    pthread_join(thread1, NULL);

    printf("Thread 1 has been canceled\n");
    sleep(5);

    printf("\n");

}