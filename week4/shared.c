#include  <pthread.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>


int shared = 10;

void* first(void* arg) {
    int x;
    x=shared;
    x=x+1;
    sleep(1);
    shared=x;
    printf("shared: %d\n",shared);
    return NULL;
}

void* second(void* arg) {
    int y;
    y=shared;
    y=y-1;
    sleep(1);
    shared=y;
    printf("shared: %d\n",shared);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, first, NULL);
    pthread_create(&t2, NULL, second, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final value: %d\n", shared);

    return 0;
}