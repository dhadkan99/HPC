#include <stdio.h>
#include <omp.h>

#define M 4
#define N 3

int main()
{
    double A[M][N], B[N][M], C[M][M];

   
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = 1.0;

    
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            B[i][j] = 2.0;

  
    #pragma omp parallel for collapse(2) num_threads(16)
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    printf("Result Matrix C:\n");
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
            printf("%6.2f ", C[i][j]);
        printf("\n");
    }

    return 0;
}
