// #include <stdio.h>
// #include <cuda_runtime.h>

// __global__ void display() {
//     int threadX = threadIdx.x;
//     int threadZ = threadIdx.z;
//     int blockID = blockIdx.x;
//     printf("Block : %d -> ThreadX = %d, ThreadZ = %d\n", blockID, threadX, threadZ);
// }

// int main() {
//     dim3 gridSize(1, 1, 1);      
//     dim3 blockSize(5, 1, 3);     
//     display<<<gridSize, blockSize>>>();
//     cudaDeviceSynchronize();
//     return 0;
// }
#include <stdio.h>
#include <cuda_runtime.h>

__global__ void display() {
    int threadX = threadIdx.x;
    int threadY = threadIdx.y;
    int threadZ = threadIdx.z;
    int blockID = blockIdx.x;
    printf("Block : %d -> ThreadX = %d, ThreadY = %d, ThreadZ = %d\n", blockID, threadX, threadY, threadZ);
}

int main() {
    dim3 gridSize(1, 1, 1);      
    dim3 blockSize(3, 2, 3);     
    display<<<gridSize, blockSize>>>();
    cudaDeviceSynchronize();
    return 0;
}
