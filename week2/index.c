#include <stdio.h>

// int  x,y;
// int main() {
//     x = 10;
//     y = 20;
//     printf("%d,%d", x, y);
//     return 0;
// }




void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 4;
    int y = 5;

    printf("Before swap: %d %d\n", x, y);

    swap(&x, &y);

    printf("After swap:  %d %d\n", x, y);

    return 0;
}
