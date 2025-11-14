#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* func(void *p) {
    int *a = malloc(sizeof(int));
    *a = 42;
    pthread_exit(a);
    return NULL;
}

int main() {
    void *ptr = NULL;
    pthread_t thread;
    pthread_create(&thread, NULL, func, NULL);
    pthread_join(thread, &ptr);

    printf("Pointer value: %p\n", ptr);
    printf("Integer value: %d\n", *(int*)ptr);
    free(ptr);
    return 0;
}