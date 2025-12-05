#include <stdio.h>
#include <stdlib.h>

// int main(int argc, char *argv[]) {
//     if (argc != 3) {
//         printf("Usage: <name> <age>\n");
//         return 1;
//     }

//     char *name = argv[1];
//     int age = atoi(argv[2]);

//     printf("Hello %s, you are %d years old.\n", name, age);
//     return 0;
// }
//Qn2
// int main() {
//     char name[50];
//     int age;

//     printf("Enter your name: ");
//     scanf("%49s", name);

//     printf("Enter your age: ");
//     scanf("%d", &age);

//     printf("Hello %s, you are %d years old.\n", name, age);

//     return 0;
// }
//Qn3
// #include <stdio.h>

// int main() {
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
//     return 0;
// }
//Q4
// #include <stdio.h>

// void swap(int *x, int *y) {
//     int temp = *x;
//     *x = *y;
//     *y = temp;
// }

// int main() {
//     int a = 3, b = 4;

//     printf("a is %d and b is %d\n", a, b);
//     swap(&a, &b);
//     printf("a is now %d and b is now %d\n", a, b);

//     return 0;
// }
//Qn5
// #include <stdio.h>
// #include <stdlib.h>

// int main() {
//     int size;

//     printf("Enter a number between 1 and 50: ");
//     scanf("%d", &size);

//     if (size < 1 || size > 50) {
//         printf("Invalid size.\n");
//         return 1;
//     }

//     int *numbers = malloc(size * sizeof(int));
//     if (!numbers) {
//         printf("Memory allocation failed.\n");
//         return 1;
//     }

//     for (int i = 0; i < size; i++) {
//         numbers[i] = rand();
//         printf("%d is %d\n", i, numbers[i]);
//     }

//     free(numbers);
//     return 0;
// }

//Qn6
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *threadFunc(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < 10; i++) {
        printf("Thread %d (ID %ld): i=%d\n", id, pthread_self(), i);
        usleep(1000);
    }

    free(arg);
    return NULL;
}

int main() {
    int numThreads;

    printf("Enter number of threads: ");
    scanf("%d", &numThreads);

    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));

    for (int i = 0; i < numThreads; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, threadFunc, id);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}
