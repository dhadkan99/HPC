#include <stdio.h>
#include <omp.h>

int main() {
    int total_primes = 0;

    #pragma omp parallel for reduction(+:total_primes)
    for (int n = 2; n <= 1000; n++) {
        int prime = 1;

        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                prime = 0;
                break;
            }
        }

        if (prime == 1) {
            total_primes++;
        }
    }

    printf("Total primes = %d\n", total_primes);
    return 0;
}
