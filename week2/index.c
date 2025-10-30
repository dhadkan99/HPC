#include <stdio.h>

// int  x,y;
// int main() {
//     x = 10;
//     y = 20;
//     printf("%d,%d", x, y);
//     return 0;
// }



void swap(int a, int b){
    int temp = a;
    printf("before swap %d %d \n", a, b);
    a = b;
    b = temp;
    printf("after swap %d %d \n", a, b);
}

int main() {
    int x,y;
    printf("%d %d \n", &x, &y);
    swap(4, 5);
    return 0;
}