#include <stdio.h>
#include <cuda_runtime.h>
#include <stdlib.h>
#include <math.h>

__global__ void solveQuadratic(float *a, float *b, float *c, float *r1, float *r2, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= n) return;

    float disc = b[idx]*b[idx] - 4*a[idx]*c[idx];
    if (disc >= 0) {
        float sqrt_disc = sqrtf(disc);
        r1[idx] = (-b[idx] + sqrt_disc) / (2*a[idx]);
        r2[idx] = (-b[idx] - sqrt_disc) / (2*a[idx]);
    } else {
        r1[idx] = r2[idx] = NAN;
    }
}

int main() {
    int n = 0;
    float a[100], b[100], c[100];

    FILE *fp = fopen("QuadData.txt", "r");
    if (!fp) { 
        printf("Cannot open file\n"); 
        return 1; 
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%f,%f,%f", &a[n], &b[n], &c[n]) == 3) n++;
    }
    fclose(fp);

    float *d_a, *d_b, *d_c, *d_r1, *d_r2;
    cudaMalloc(&d_a, n * sizeof(float));
    cudaMalloc(&d_b, n * sizeof(float));
    cudaMalloc(&d_c, n * sizeof(float));
    cudaMalloc(&d_r1, n * sizeof(float));
    cudaMalloc(&d_r2, n * sizeof(float));

    cudaMemcpy(d_a, a, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_c, c, n * sizeof(float), cudaMemcpyHostToDevice);

    int threads = 32;
    int blocks = (n + threads - 1) / threads;

    solveQuadratic<<<blocks, threads>>>(d_a, d_b, d_c, d_r1, d_r2, n);
    cudaDeviceSynchronize();

    float r1[100], r2[100];
    cudaMemcpy(r1, d_r1, n * sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(r2, d_r2, n * sizeof(float), cudaMemcpyDeviceToHost);

    for (int i = 0; i < n; i++) {
        if (!isnan(r1[i])) 
            printf("Equation %d: %.2f x^2 + %.2f x + %.2f -> Roots: %.2f, %.2f\n", i+1, a[i], b[i], c[i], r1[i], r2[i]);
        else
            printf("Equation %d: %.2f x^2 + %.2f x + %.2f -> Complex roots\n", i+1, a[i], b[i], c[i]);
    }

    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
    cudaFree(d_r1); cudaFree(d_r2);

    return 0;
}
