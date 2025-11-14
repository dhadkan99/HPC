#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct Range {
    int start;
    int end;
};

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void* find_primes(void* arg) {
    struct Range* r = (struct Range*)arg;
    int* count = malloc(sizeof(int));
    *count = 0;

    for (int i = r->start; i <= r->end; i++) {
        if (is_prime(i)) (*count)++;
    }

    pthread_exit(count); 
}

int main() {
    int n, limit;
    printf("Enter number of threads: ");
    scanf("%d", &n);
    printf("Enter range (up to): ");
    scanf("%d", &limit);

    pthread_t threads[n];
    struct Range ranges[n];

    int range = limit / n;

    for (int i = 0; i < n; i++) {
        ranges[i].start = i * range + 1;
        ranges[i].end = (i == n - 1) ? limit : (i + 1) * range;
        pthread_create(&threads[i], NULL, find_primes, &ranges[i]);
    }

    int total_primes = 0;
    for (int i = 0; i < n; i++) {
        int* count;
        pthread_join(threads[i], (void**)&count);
        printf("Thread %d found %d prime numbers.\n", i + 1, *count);
        total_primes += *count;
        free(count);
    }

    printf("Total prime numbers: %d\n", total_primes);
    return 0;
}