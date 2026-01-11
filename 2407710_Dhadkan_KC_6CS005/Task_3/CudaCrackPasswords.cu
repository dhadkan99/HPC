#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda_runtime.h>

#define MAX_HASH_LEN 130
#define TOTAL_CUDA_PASSWORDS 67600

__device__ int device_strcmp(const char *a, const char *b) {
    for (int i = 0; i < MAX_HASH_LEN; i++) {
        if (a[i] != b[i]) return 0;
        if (a[i] == '\0' && b[i] == '\0') return 1;
    }
    return 1;
}

__global__ void crackKernel(
    char *fileHashes,
    char *cudaHashes,
    int *results,
    int totalFileHashes
) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= totalFileHashes) return;

    char *target = &fileHashes[tid * MAX_HASH_LEN];

    for (int i = 0; i < TOTAL_CUDA_PASSWORDS; i++) {
        char *candidate = &cudaHashes[i * MAX_HASH_LEN];
        if (device_strcmp(target, candidate)) {
            results[tid] = i;
            return;
        }
    }
    results[tid] = -1;
}

void indexToPassword(int index, char *out) {
    out[0] = 'a' + (index / 2600);
    out[1] = 'a' + (index / 100) % 26;
    out[2] = '0' + (index / 10) % 10;
    out[3] = '0' + index % 10;
    out[4] = '\0';
}

int main() {

    FILE *f1 = fopen("file_hashes.txt", "r");
    FILE *f2 = fopen("cuda_hashes.txt", "r");
    if (!f1 || !f2) return 1;

    int count = 0;
    char line[MAX_HASH_LEN];
    while (fgets(line, MAX_HASH_LEN, f1)) count++;
    rewind(f1);

    char *h_fileHashes = (char*)malloc(count * MAX_HASH_LEN);
    char *h_cudaHashes = (char*)malloc(TOTAL_CUDA_PASSWORDS * MAX_HASH_LEN);

    for (int i = 0; i < count; i++) {
        fgets(&h_fileHashes[i * MAX_HASH_LEN], MAX_HASH_LEN, f1);
        h_fileHashes[i * MAX_HASH_LEN +
            strcspn(&h_fileHashes[i * MAX_HASH_LEN], "\n")] = '\0';
    }

    for (int i = 0; i < TOTAL_CUDA_PASSWORDS; i++) {
        fgets(&h_cudaHashes[i * MAX_HASH_LEN], MAX_HASH_LEN, f2);
        h_cudaHashes[i * MAX_HASH_LEN +
            strcspn(&h_cudaHashes[i * MAX_HASH_LEN], "\n")] = '\0';
    }

    fclose(f1);
    fclose(f2);

    char *d_fileHashes, *d_cudaHashes;
    int *d_results;

    cudaMalloc((void**)&d_fileHashes, count * MAX_HASH_LEN);
    cudaMalloc((void**)&d_cudaHashes, TOTAL_CUDA_PASSWORDS * MAX_HASH_LEN);
    cudaMalloc((void**)&d_results, count * sizeof(int));

    cudaMemcpy(d_fileHashes, h_fileHashes,
               count * MAX_HASH_LEN, cudaMemcpyHostToDevice);
    cudaMemcpy(d_cudaHashes, h_cudaHashes,
               TOTAL_CUDA_PASSWORDS * MAX_HASH_LEN, cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks = (count + threads - 1) / threads;

    crackKernel<<<blocks, threads>>>(
        d_fileHashes, d_cudaHashes, d_results, count);
    cudaDeviceSynchronize();

    int *h_results = (int*)malloc(count * sizeof(int));
    cudaMemcpy(h_results, d_results,
               count * sizeof(int), cudaMemcpyDeviceToHost);

    FILE *out = fopen("decrypted_passwords.txt", "w");
    char raw[5];

    for (int i = 0; i < count; i++) {
        if (h_results[i] >= 0) {
            indexToPassword(h_results[i], raw);
            fprintf(out, "%s\n", raw);
        } else {
            fprintf(out, "NOT FOUND\n");
        }
    }

    fclose(out);

    cudaFree(d_fileHashes);
    cudaFree(d_cudaHashes);
    cudaFree(d_results);
    free(h_fileHashes);
    free(h_cudaHashes);
    free(h_results);

    return 0;
}
