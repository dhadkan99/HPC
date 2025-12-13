#include <cuda.h>
#include <stdio.h>

__global__ void helloFromGPU(void) {
    printf("Hello World from GPU! threadIdx.x = %d\n", threadIdx.x);
}

int main() {
    printf("Hello from CPU!\n");

    helloFromGPU<<<1,1>>>();

    cudaDeviceSynchronize();

    printf("Back to CPU!\n");

    return 0;
}
