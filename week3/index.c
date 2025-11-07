#include <stdio.h>
//single instruction single data
// int main() {
//     int a = 5, b = 10, c;
//     c = a + b; 
//     printf("SISD Result: %d\n", c);
//     return 0;
// }
//same instruction on multiple data
// int main() {
//     int a[4] = {1, 2, 3, 4};
//     int b[4] = {10, 20, 30, 40};
//     int c[4];

//     for (int i = 0; i < 4; i++) {
//         c[i] = a[i] + b[i]; 
//     }

//     printf("SIMD Result: ");
//     for (int i = 0; i < 4; i++) printf("%d ", c[i]);
//     printf("\n");
//     return 0;
// }
//multiple instruction single data
// int main() {
//     int x = 5;
//     int result1 = x * 2;  
//     int result2 = x + 10;  
//     printf("MISD Results: %d and %d\n", result1, result2);
//     return 0;
// }
//multiple instruction multiple data
//  int main() {
//     int a[2] = {1, 2};
//     int b[2] = {10, 20};
//     int sum[2], product[2];

//     for (int i = 0; i < 2; i++) {
//         sum[i] = a[i] + b[i];         
//         product[i] = a[i] * b[i];     
//     }

//     printf("MIMD Results:\n");
//     for (int i = 0; i < 2; i++) {
//         printf("Sum[%d]: %d, Product[%d]: %d\n", i, sum[i], i, product[i]);
//     }
//     return 0;   
// }
// multiple instruction multiple data with thread 
#include <pthread.h>

void sisd() {
    int a = 5, b = 10;
    int c = a + b; 
    printf("Result: %d \n", c);
}

void simd() {
    int data[4] = {1, 2, 3, 4};
    int result[4];
    int i;

    for (i = 0; i < 4; i++) {
        result[i] = data[i] * 2;
    }

    for (i = 0; i < 4; i++)
        printf("%d ", result[i]);
    printf("\n");
}


void misd() {
    int x = 5;
    int a = x + 2;
    int b = x * 2;
    int c = x - 2;

    printf("%d, %d, %d\n", a, b, c);
}

void* task(void* arg) {
    int num = *(int*)arg;
    printf("Thread %d processing its own data: %d * 2 = %d\n", num, num, num * 2);
    return NULL;
}

void mimd() {
    printf("\nMIMD (Multiple Instruction Multiple Data):\n");
    pthread_t t1, t2, t3, t4;
    int d1 = 1, d2 = 2, d3 = 3, d4 = 4;

    pthread_create(&t1, NULL, task, &d1);
    pthread_create(&t2, NULL, task, &d2);
    pthread_create(&t3 , NULL, task, &d3);
    pthread_create(&t4 , NULL, task, &d4);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}


int main() {
    sisd();
    simd();
    misd();
    mimd();
    return 0;
}