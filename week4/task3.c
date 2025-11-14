#include <stdio.h>
#include <pthread.h>

int shared_value = 10; 

void* increment(void* arg) {
    int i;
    for(i = 0; i < 2; i++) {
        shared_value++;  
    }
    return NULL;
}

void* decrement(void* arg) {
    int i;
    for(i = 0; i < 2; i++) {
        shared_value--;  
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, decrement, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
   
    printf("Final value: %d\n", shared_value);

    return 0;
}