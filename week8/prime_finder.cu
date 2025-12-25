#include <stdio.h>
#include <cuda.h>

#define NUM_BLOCKS 10
#define THREADS_PER_BLOCK 100
#define TOTAL_THREADS (NUM_BLOCKS * THREADS_PER_BLOCK)

#define PRIMES_PER_THREAD 1000
#define TOTAL_PRIMES (TOTAL_THREADS * PRIMES_PER_THREAD)


__device__ int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

__global__ void find_primes(int *d_primes) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    
    int count = 0;
    int number = tid * 10000 + 2; 
    
    int base_index = tid * PRIMES_PER_THREAD;
    
    while (count < PRIMES_PER_THREAD) {
        if (is_prime(number)) {
            d_primes[base_index + count] = number;
            count++;
        }
        number++;
    }
}

int main() {
    int *h_primes;
    int *d_primes;
    
    size_t size = TOTAL_PRIMES * sizeof(int);
    
 
    h_primes = (int *)malloc(size);
    if (h_primes == NULL) {
        printf("Failed to allocate host memory\n");
        return 1;
    }
    
  
    cudaError_t err = cudaMalloc((void **)&d_primes, size);
    if (err != cudaSuccess) {
        printf("cudaMalloc failed: %s\n", cudaGetErrorString(err));
        free(h_primes);
        return 1;
    }
    
    printf("Finding %d primes using %d blocks and %d threads...\n", 
           TOTAL_PRIMES, NUM_BLOCKS, THREADS_PER_BLOCK);
    
    
    dim3 blocks(NUM_BLOCKS);
    dim3 threads(THREADS_PER_BLOCK);
    find_primes<<<blocks, threads>>>(d_primes);
    

    err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("Kernel launch failed: %s\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        free(h_primes);
        return 1;
    }
    
   
    err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        printf("cudaDeviceSynchronize failed: %s\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        free(h_primes);
        return 1;
    }
    
 
    err = cudaMemcpy(h_primes, d_primes, size, cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        printf("cudaMemcpy failed: %s\n", cudaGetErrorString(err));
        cudaFree(d_primes);
        free(h_primes);
        return 1;
    }
    
    printf("Successfully found primes!\n\n");
    
    
    printf("First 50 primes:\n");
    for (int i = 0; i < 50 && i < TOTAL_PRIMES; i++) {
        printf("%d ", h_primes[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    
 
    printf("\n\nLast 50 primes:\n");
    for (int i = TOTAL_PRIMES - 50; i < TOTAL_PRIMES; i++) {
        printf("%d ", h_primes[i]);
        if ((i - (TOTAL_PRIMES - 50) + 1) % 10 == 0) printf("\n");
    }
    
    printf("\n\nTotal primes found: %d\n", TOTAL_PRIMES);
    

    cudaFree(d_primes);
    free(h_primes);
    
    return 0;
}