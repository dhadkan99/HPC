#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>
#include "lodepng.h"

__global__ void sobel(
    const unsigned char* input,
    unsigned char* output,
    int width,
    int height
) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    int sumX = 0;
    int sumY = 0;

    for (int ky = -1; ky <= 1; ky++) {
        for (int kx = -1; kx <= 1; kx++) {
            int px = x + kx;
            int py = y + ky;

            unsigned char pixel = 0;
            if (px >= 0 && px < width && py >= 0 && py < height) {
                pixel = input[py * width + px];
            }

            sumX += pixel * Gx[ky + 1][kx + 1];
            sumY += pixel * Gy[ky + 1][kx + 1];
        }
    }

    int mag = (int)sqrtf((float)(sumX * sumX + sumY * sumY));
    if (mag > 255) mag = 255;

    output[y * width + x] = (unsigned char)mag;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Usage: %s image1.png image2.png ...\n", argv[0]);
        return 1;
    }

    for (int img = 1; img < argc; img++) {

        unsigned char* h_rgba = NULL;
        unsigned width, height;

        unsigned error = lodepng_decode32_file(
            &h_rgba, &width, &height, argv[img]
        );
        if (error) continue;

        size_t pixels = width * height;

        unsigned char* h_gray = (unsigned char*)malloc(pixels);
        unsigned char* h_output = (unsigned char*)malloc(pixels);
        unsigned char* h_out_rgba = (unsigned char*)malloc(pixels * 4);

        for (size_t i = 0; i < pixels; i++) {
            unsigned char r = h_rgba[i * 4 + 0];
            unsigned char g = h_rgba[i * 4 + 1];
            unsigned char b = h_rgba[i * 4 + 2];
            h_gray[i] = (unsigned char)(0.299f*r + 0.587f*g + 0.114f*b);
        }

        unsigned char *d_input, *d_output;
        cudaMalloc(&d_input, pixels);
        cudaMalloc(&d_output, pixels);

        cudaMemcpy(d_input, h_gray, pixels, cudaMemcpyHostToDevice);

        dim3 block(16, 16);
        dim3 grid(
            (width + block.x - 1) / block.x,
            (height + block.y - 1) / block.y
        );

        sobel<<<grid, block>>>(d_input, d_output, width, height);
        cudaDeviceSynchronize();

        cudaMemcpy(h_output, d_output, pixels, cudaMemcpyDeviceToHost);

        for (size_t i = 0; i < pixels; i++) {
            unsigned char v = h_output[i];
            h_out_rgba[i * 4 + 0] = v;
            h_out_rgba[i * 4 + 1] = v;
            h_out_rgba[i * 4 + 2] = v;
            h_out_rgba[i * 4 + 3] = 255;
        }

        char outName[256];
        snprintf(outName, sizeof(outName), "edges_%s", argv[img]);

        lodepng_encode32_file(outName, h_out_rgba, width, height);

        cudaFree(d_input);
        cudaFree(d_output);
        free(h_rgba);
        free(h_gray);
        free(h_output);
        free(h_out_rgba);
    }

    return 0;
}
