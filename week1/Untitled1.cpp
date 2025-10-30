#include <stdio.h>
#include <stdlib.h>
//
//int main() {
//    char name[50];
//    int age;
//
//    printf("Enter your name: ");
//    scanf("%s", name);
//
//    printf("Enter your age: ");
//    scanf("%d", &age);
//
//    printf("Hello %s, you are %d years old.\n", name, age);
//    return 0;
//}
//3
//int  main(int argc, char *argv[])
//{
//    for (int n = 0; n < 100; n++)
//    {
//        if (n % 2 == 0)
//        {
//            printf("Bish\n");
//        }
//        else if (n % 3 == 0)
//        {
//            printf("Bash\n");
//        }
//        else if (n % 5 == 0)
//        {
//            printf("Bosh\n");
//        }
//        else if (n % 2 == 0 && n % 3 == 0)
//        {
//            printf("Bish Bash\n");
//        }
//        else if (n % 2 == 0 && n % 5 == 0)
//        {
//            printf("Bish Bosh\n");
//        }
//        else if (n % 3 == 0 && n % 5 == 0)
//        {
//            printf("Bash Bosh\n");
//        }
//        else if (n % 2 == 0 && n % 3 == 0 && n % 5 == 0)
//        {
//            printf("Bish Bash Bosh\n");
//        }
//        else
//        {
//            printf("%d\n", n);
//        }
//       
//    }
//}

//q4
//void swap(int *x, int *y){
//    int temp = *x;
//    *x = *y;
//    *y = temp;
//}
//
//int main(int argc, char *argv[]){
//    int a = 3;
//    int b = 4;
//    printf("a is %d and b is %d\n", a, b);
//    swap(&a, &b);
//    printf("After swapping: \n");
//    printf("a is %d and b is %d", a, b);
//}

//q5
int main() {
    int num;
    
    printf("Enter a number between 1 and 50: ");
    scanf("%d", &num);

    if (num < 1 || num > 50) {
        printf("Invalid input! Please enter a number between 1 and 50.\n");
        return 1;
    }
    
    int *array = (int *)malloc(num * sizeof(int));

    
    for (int i = 0; i < num; i++) {
        array[i] = rand();
    }
    
    printf("Array elements:\n");
    for (int i = 0; i < num; i++) {
        printf("Element %d: %d\n", i, array[i]);
    }
    
    return 0;
}