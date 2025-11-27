#include <stdio.h>
#include <pthread.h>

typedef struct {
    int accountNumber;
    double balance;
} Account;

Account accounts[10];
pthread_mutex_t accountMutex[10];   
void *withdraw(void *p) {
    int accountId = *(int *)p;
    double amount = 100;

    // Lock the specific account
    pthread_mutex_lock(&accountMutex[accountId]);
    
    accounts[accountId].balance -= amount;

    pthread_mutex_unlock(&accountMutex[accountId]);
    return NULL;
}

void *deposit(void *p) {
    int accountId = *(int *)p;
    double amount = 100;

    // Lock the specific account
    pthread_mutex_lock(&accountMutex[accountId]);
    
    accounts[accountId].balance += amount;

    pthread_mutex_unlock(&accountMutex[accountId]);
    return NULL;
}

int main() {
    pthread_t threads[20];
    int ids[10];

    // Initialize accounts and mutexes
    for (int i = 0; i < 10; i++) {
        accounts[i].accountNumber = i;
        accounts[i].balance = 1000;
        pthread_mutex_init(&accountMutex[i], NULL);
    }

    // Create withdraw + deposit threads for each account
    for (int i = 0; i < 10; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, withdraw, &ids[i]);
        pthread_create(&threads[i + 10], NULL, deposit, &ids[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < 20; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print final balances
    for (int i = 0; i < 10; i++) {
        printf("Account %d balance = %.2f\n", i, accounts[i].balance);
    }

    // Destroy mutexes
    for (int i = 0; i < 10; i++) {
        pthread_mutex_destroy(&accountMutex[i]);
    }

    return 0;
}
