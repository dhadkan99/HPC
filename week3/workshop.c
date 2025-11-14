#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


#define MAX 10000
#define THREAD_COUNT 3  

typedef struct {
    int start;
    int end ;

}Range;

int isPrime(int n ){
    if(n <2) return 0;
    if(n ==2) return 1;
    if(n%2 ==0) return 0;
    for(int i =3; i*i <= n; i+=2){  
        if (n%i==0) return 0;
    }
    return 1;
}

void* findPrime(void *args){
    Range *range = (Range *)args;
    printf("Thread started\n", range->start, range->end);
    for(int i= range->start; i< range->end; i++){
        if(isPrime(i)){
            printf("%d ", i);
        }
    }
    pthread_exit(NULL);
}
int main() {
    pthread_t threads[THREAD_COUNT];
    Range ranges[THREAD_COUNT];
    int step = MAX / THREAD_COUNT;

  
    for (int i = 0; i < THREAD_COUNT; i++) {
        ranges[i].start = i * step + 1;
        ranges[i].end = (i == THREAD_COUNT - 1) ? MAX : (i + 1) * step;
    }

    
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, findPrime, (void*)&ranges[i]);
    }

    
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nAll threads finished.\n");
    return 0;
}