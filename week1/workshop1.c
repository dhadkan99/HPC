#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

// ----------- Task 1: Command-line args -----------
void task1(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s 1 <name> <age>\n", argv[0]);
        return;
    }
    char *name = argv[2];
    int age = atoi(argv[3]);
    printf("Hello %s, you are %d years old.\n", name, age);
}

// // ----------- Task 2: scanf() input -----------
// void task2() {
//     char name[50];
//     int age;

//     printf("Enter your name: ");
//     scanf("%s", name);

//     printf("Enter your age: ");
//     scanf("%d", &age);

//     printf("Hello %s, you are %d years old.\n", name, age);
// }

// // ----------- Task 3: Bish-Bash-Bosh -----------
// void task3() {
//     for (int n = 1; n <= 100; n++) {
//         if (n % 2 == 0 && n % 3 == 0 && n % 5 == 0)
//             printf("BishBashBosh\n");
//         else if (n % 2 == 0 && n % 3 == 0)
//             printf("BishBash\n");
//         else if (n % 2 == 0 && n % 5 == 0)
//             printf("BishBosh\n");
//         else if (n % 3 == 0 && n % 5 == 0)
//             printf("BashBosh\n");
//         else if (n % 2 == 0)
//             printf("Bish\n");
//         else if (n % 3 == 0)
//             printf("Bash\n");
//         else if (n % 5 == 0)
//             printf("Bosh\n");
//         else
//             printf("%d\n", n);
//     }
// }

// // ----------- Task 4: Swap with pointers -----------
// void swap(int *x, int *y) {
//     int temp = *x;
//     *x = *y;
//     *y = temp;
// }
// void task4() {
//     int a = 3, b = 4;
//     printf("a is %d and b is %d\n", a, b);
//     swap(&a, &b);
//     printf("a is now %d and b is now %d\n", a, b);
// }

// // ----------- Task 5: malloc() dynamic array -----------
// void task5() {
//     int size;
//     printf("Enter a number between 1 and 50: ");
//     scanf("%d", &size);

//     if (size < 1 || size > 50) {
//         printf("Invalid size.\n");
//         return;
//     }

//     int *numbers = (int *)malloc(size * sizeof(int));
//     if (!numbers) {
//         printf("Memory allocation failed.\n");
//         return;
//     }

//     for (int i = 0; i < size; i++) {
//         numbers[i] = rand();
//         printf("%d is %d\n", i, numbers[i]);
//     }

//     free(numbers);
// }

// // ----------- Task 6: Multithreading dynamic threads -----------
// void *threadFunc(void *arg) {
//     int id = *(int *)arg;
//     for (int i = 0; i < 10; i++) {
//         printf("Thread %d (ID %ld): i=%d\n", id, pthread_self(), i);
//         usleep(1000);
//     }
//     free(arg);
//     return NULL;
// }

// void task6() {
//     int numThreads;
//     printf("Enter number of threads: ");
//     scanf("%d", &numThreads);

//     pthread_t *threads = malloc(numThreads * sizeof(pthread_t));

//     for (int i = 0; i < numThreads; i++) {
//         int *id = malloc(sizeof(int));
//         *id = i + 1;
//         pthread_create(&threads[i], NULL, threadFunc, id);
//     }

//     for (int i = 0; i < numThreads; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     free(threads);
// }

// // ----------- Main Menu -----------
// int main(int argc, char *argv[]) {
//     int choice;

//     printf("\n=== Workshop Menu ===\n");
//     printf("1. Command-line args (name, age)\n");
//     printf("2. scanf() input (name, age)\n");
//     printf("3. Bish-Bash-Bosh (1–100)\n");
//     printf("4. Swap using pointers\n");
//     printf("5. malloc() dynamic array\n");
//     printf("6. Multithreading (dynamic threads)\n");
//     printf("Enter your choice: ");
//     scanf("%d", &choice);

//     switch (choice) {
//         case 1: task1(argc, argv); break;
//         case 2: task2(); break;
//         case 3: task3(); break;
//         case 4: task4(); break;
//         case 5: task5(); break;
//         case 6: task6(); break;
//         default: printf("Invalid choice.\n");
//     }

//     return 0;
// }
