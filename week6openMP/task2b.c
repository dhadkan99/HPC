#include <stdio.h>
#include <omp.h>

int main() {
    int c;

    #pragma omp parallel for num_threads(5) private(c)
    for (int n = 2; n <= 1000; n++) {
        c = 0;

        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                c = 1;
                break;
            }
        }

        if (c == 0) {
            printf("Thread %d found %d\n",
                   omp_get_thread_num(), n);
        }
    }

    return 0;
}
