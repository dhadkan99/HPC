#include <stdio.h>
#include <stdlib.h>

// void task1(int argc, char *argv[]) {
//     if (argc != 4) {
//         printf("Usage: %s 1 <name> <age>\n", argv[0]);
//         return;
//     }
//     char *name = argv[2];
//     int age = atoi(argv[3]);
//     printf("Hello %s, you are %d years old.\n", name, age);
// }

void task2() {
    char name[50];
    int age;

    printf("Enter your name: ");
    scanf("%s", name);

    printf("Enter your age: ");
    scanf("%d", &age);

    printf("Hello %s, you are %d years old.\n", name, age);
}