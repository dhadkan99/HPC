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
    int maxNum, threadCount;

    printf("Enter the maximum number (upper limit): ");
    scanf("%d", &maxNum);

    printf("Enter the number of threads to use: ");
    scanf("%d", &threadCount);

   
    if (maxNum < 2 || threadCount < 1) {
        printf("Invalid input. Max number must be >= 2 and threads >= 1.\n");
        return 1;
    }
    pthread_t threads[threadCount];
    Range ranges[threadCount];

    int step = maxNum / threadCount;
    for (int i = 0; i < threadCount; i++) {
        ranges[i].start = i * step + 1;
        ranges[i].end = (i == threadCount - 1) ? maxNum : (i + 1) * step;
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threads[i], NULL, findPrime, (void*)&ranges[i]);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\nAll threads finished.\n");
    return 0;
}