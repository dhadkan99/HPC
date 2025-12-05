#include <stdio.h>
#include <omp.h>

int main() {
    int c;

    #pragma omp parallel for private(c)
    for (int n = 2; n <= 1000; n++) {
        c = 0;

        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                c = 1;
                break;
            }
        }

        if (c == 0) {
            printf("%d\n", n);
        }
    }

    return 0;
}
