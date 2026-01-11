#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define TOTAL 67600
#define PASS_LEN 11

__device__ char wrap_letter(char c) {
    return 'a' + (c - 'a' + 26) % 26;
}

__device__ char wrap_digit(char c) {
    return '0' + (c - '0' + 10) % 10;
}

__device__ void cudaCrypt(char* raw, char* out) {

    out[0] = raw[0] + 2;
    out[1] = raw[0] - 2;
    out[2] = raw[0] + 1;
    out[3] = raw[1] + 3;
    out[4] = raw[1] - 3;
    out[5] = raw[1] - 1;
    out[6] = raw[2] + 2;
    out[7] = raw[2] - 2;
    out[8] = raw[3] + 4;
    out[9] = raw[3] - 4;
    out[10] = '\0';

    for (int i = 0; i < 10; i++) {
        if (i < 6)
            out[i] = wrap_letter(out[i]);
        else
            out[i] = wrap_digit(out[i]);
    }
}

__global__ void generate(char* d_out) {

    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id >= TOTAL) return;

    char raw[4];
    raw[0] = 'a' + (id / 2600);
    raw[1] = 'a' + (id / 100) % 26;
    raw[2] = '0' + (id / 10) % 10;
    raw[3] = '0' + (id % 10);

    cudaCrypt(raw, &d_out[id * PASS_LEN]);
}

int main() {

    char *h_out = (char*)malloc(TOTAL * PASS_LEN);
    char *d_out = NULL;

    if (!h_out) return 1;
    if (cudaMalloc((void**)&d_out, TOTAL * PASS_LEN) != cudaSuccess) return 1;

    int threads = 256;
    int blocks = (TOTAL + threads - 1) / threads;

    generate<<<blocks, threads>>>(d_out);
    if (cudaDeviceSynchronize() != cudaSuccess) return 1;

    if (cudaMemcpy(h_out, d_out, TOTAL * PASS_LEN,
                   cudaMemcpyDeviceToHost) != cudaSuccess)
        return 1;

    FILE* fp = fopen("Cuda_passwords.txt", "w");
    if (!fp) return 1;

    for (int i = 0; i < TOTAL; i++)
        fprintf(fp, "%s\n", &h_out[i * PASS_LEN]);

    fclose(fp);
    cudaFree(d_out);
    free(h_out);

    return 0;
}
