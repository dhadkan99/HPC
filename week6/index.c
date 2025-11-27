#include <stdio.h>

void sayHello() {
    printf("Hello, World from OpenMP!\n");
}

void main() {
    #pragma omp parallel
    {
        sayHello();
    }

}