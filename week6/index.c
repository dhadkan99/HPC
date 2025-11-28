#include <stdio.h>
#include <omp.h>


void sayHello() {
    printf("Hello, World from OpenMP!\n");
}

void main() {
    
    #pragma omp parallel
    {
        sayHello();
    }

}