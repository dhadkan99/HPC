#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 10000

pthread_t* threads;
int prime_count = 0;
pthread_mutex_t lock;

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void* find_primes(void* arg) {
    int id = *(int*)arg;
    int start = id * (MAX / 3) + 1;
    int end = (id == 2) ? MAX : (id + 1) * (MAX / 3);

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            pthread_mutex_lock(&lock);
            prime_count++;
            printf("Thread %d found prime: %d (count=%d)\n", id + 1, i, prime_count);
            if (prime_count >= 5) {
                printf("\n5th prime found! Cancelling all threads...\n");
                for (int j = 0; j < 3; j++) {
                    if (j != id) pthread_cancel(threads[j]);
                }
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL);
            }
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&lock, NULL);
    threads = malloc(3 * sizeof(pthread_t));
    int ids[3] = {0, 1, 2};

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, find_primes, &ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nAll threads done.\n");
    pthread_mutex_destroy(&lock);
    free(threads);
    return 0;
}