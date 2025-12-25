#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include <cuda.h>

#define NUM_CPU_THREADS 8
#define NUM_TERMS 1000000000L
#define M_PI 3.14159265358979323846

typedef struct {
    long start;
    long end;
    double result;
} ThreadData;

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

void* cpu_leibniz_thread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double sum = 0.0;
    for (long k = data->start; k < data->end; k++) {
        double term = (k % 2 == 0 ? 1.0 : -1.0) / (2.0 * k + 1.0);
        sum += term;
    }
    data->result = sum;
    return NULL;
}

double cpu_leibniz_single() {
    double sum = 0.0;
    for (long k = 0; k < NUM_TERMS; k++) {
        double term = (k % 2 == 0 ? 1.0 : -1.0) / (2.0 * k + 1.0);
        sum += term;
    }
    return 4.0 * sum;
}

double cpu_leibniz_multi() {
    pthread_t threads[NUM_CPU_THREADS];
    ThreadData thread_data[NUM_CPU_THREADS];
    long terms_per_thread = NUM_TERMS / NUM_CPU_THREADS;
    
    for (int i = 0; i < NUM_CPU_THREADS; i++) {
        thread_data[i].start = i * terms_per_thread;
        thread_data[i].end = (i + 1) * terms_per_thread;
        if (i == NUM_CPU_THREADS - 1) thread_data[i].end = NUM_TERMS;
        pthread_create(&threads[i], NULL, cpu_leibniz_thread, &thread_data[i]);
    }
    
    double sum = 0.0;
    for (int i = 0; i < NUM_CPU_THREADS; i++) {
        pthread_join(threads[i], NULL);
        sum += thread_data[i].result;
    }
    
    return 4.0 * sum;
}

__global__ void cuda_leibniz(double* partial_sums, long num_terms) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int total_threads = gridDim.x * blockDim.x;
    double sum = 0.0;
    
    for (long k = tid; k < num_terms; k += total_threads) {
        double term = (k % 2 == 0 ? 1.0 : -1.0) / (2.0 * k + 1.0);
        sum += term;
    }
    
    partial_sums[tid] = sum;
}

double gpu_leibniz(int num_blocks, int num_threads) {
    int total_threads = num_blocks * num_threads;
    double* h_partial_sums = (double*)malloc(total_threads * sizeof(double));
    double* d_partial_sums;
    
    cudaMalloc(&d_partial_sums, total_threads * sizeof(double));
    cuda_leibniz<<<num_blocks, num_threads>>>(d_partial_sums, NUM_TERMS);
    cudaDeviceSynchronize();
    cudaMemcpy(h_partial_sums, d_partial_sums, total_threads * sizeof(double), cudaMemcpyDeviceToHost);
    
    double sum = 0.0;
    for (int i = 0; i < total_threads; i++) {
        sum += h_partial_sums[i];
    }
    
    cudaFree(d_partial_sums);
    free(h_partial_sums);
    
    return 4.0 * sum;
}

int main() {
    double pi_estimate, start_time, end_time;
    
    printf("Calculating Pi using Leibniz formula with %ld terms\n", NUM_TERMS);
    printf("Actual Pi value: %.10f\n", M_PI);
    printf("================================================================\n\n");
    
    // CPU Single-threaded
    start_time = get_time();
    pi_estimate = cpu_leibniz_single();
    end_time = get_time();
    printf("CPU Single-threaded:\n");
    printf("  Pi estimate: %.10f\n", pi_estimate);
    printf("  Error: %.10f\n", fabs(M_PI - pi_estimate));
    printf("  Time: %.6f seconds\n\n", end_time - start_time);
    
    // CPU Multi-threaded
    start_time = get_time();
    pi_estimate = cpu_leibniz_multi();
    end_time = get_time();
    printf("CPU Multi-threaded (%d threads):\n", NUM_CPU_THREADS);
    printf("  Pi estimate: %.10f\n", pi_estimate);
    printf("  Error: %.10f\n", fabs(M_PI - pi_estimate));
    printf("  Time: %.6f seconds\n\n", end_time - start_time);
    
    printf("================================================================\n");
    printf("CUDA Performance Analysis:\n");
    printf("================================================================\n\n");
    
    int configs[][2] = {
        {10, 100},
        {100, 100},
        {256, 256},
        {512, 512},
        {1000, 256},
        {1024, 1024}
    };
    
    int num_configs = sizeof(configs) / sizeof(configs[0]);
    
    for (int i = 0; i < num_configs; i++) {
        int blocks = configs[i][0];
        int threads = configs[i][1];
        
        start_time = get_time();
        pi_estimate = gpu_leibniz(blocks, threads);
        end_time = get_time();
        
        printf("CUDA Configuration: %d blocks x %d threads = %d total threads\n",
               blocks, threads, blocks * threads);
        printf("  Pi estimate: %.10f\n", pi_estimate);
        printf("  Error: %.10f\n", fabs(M_PI - pi_estimate));
        printf("  Time: %.6f seconds\n\n", end_time - start_time);
    }
    
    printf("================================================================\n");
    
    return 0;
}