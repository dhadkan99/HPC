#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// bool isPrime(int n) {
//     if (n <= 1) return false;
//     if (n == 2) return true;
//     if (n % 2 == 0) return false;
//     for (int i = 3; i <= sqrt(n); i += 2)
//         if (n % i == 0) return false;
//     return true;
// }

// int main() {
//     int limit;
//     printf("Enter the upper limit to find prime numbers: ");
//     scanf("%d", &limit);
//     if (limit < 2) { printf("No prime numbers.\n"); return 0; }
//     printf("Prime numbers up to %d:\n", limit);
//     if (limit >= 2) printf("2 ");
//     for (int i = 3; i <= limit; i += 2)
//         if (isPrime(i)) printf("%d ", i);
//     printf("\n");
//     return 0;
// }

// int main() {
//     printf("Hello, World!\n");
//     return 0;
// }
// Function to check if a number is prime
// int isPrime(int n) {

int prime(int a){
    if(a<=1){
        return 0;
    }
    for(int i=2; i*i<=a; i++){
        if(a%i==0){
            return 0;
        }
    }
    return 1;
}

void main(){
    int a, b; 
    printf("enter a number :");
    scanf("%d", &a);

    int sum = 0;
    while (a > 0) {
        b = a%10;
        a= a/10;
        sum +=  b*b;
    }

    printf("The sum of squares of digits is: %d\n", sum);

    if(prime(sum)){
        printf("The sum is prime");
    }else{
        printf("The sum is not prime");
    }
}
