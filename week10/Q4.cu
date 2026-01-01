// #include <stdio.h>
// #include <cuda_runtime.h>

// __global__ void displayIdx() {
//     int tid = threadIdx.y * blockDim.x + threadIdx.x;
//     printf("%d\n", tid);
// }

// int main() {
//     int n;
//     printf("Enter n: ");
//     scanf("%d", &n);

//     dim3 gridSize(1, 1, 1);
//     dim3 blockSize(n, n, 1);

//     displayIdx<<<gridSize, blockSize>>>();
//     cudaDeviceSynchronize();

//     return 0;
// }
#include <stdio.h>
#include <cuda_runtime.h>

__global__ void displayIdx() {
    
    int tid = threadIdx.z * blockDim.y * blockDim.x
            + threadIdx.y * blockDim.x
            + threadIdx.x;
    printf("%d\n", tid);
}

int main() {
    int n;
    printf("Enter n: ");
    scanf("%d", &n);

    
    dim3 gridSize(1, 1, 1);
    dim3 blockSize(n, n, n);

    displayIdx<<<gridSize, blockSize>>>();
    cudaDeviceSynchronize();

    return 0;
}
