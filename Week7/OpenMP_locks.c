#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int booksAvailable = 3;
omp_lock_t bookLock;

int main(void)
{
    omp_init_lock(&bookLock);

    #pragma omp parallel num_threads(5)
    {
        int id = omp_get_thread_num() + 1;

        for (int i = 0; i < 10; ++i)
        {
            printf("Borrower %d wants to borrow a book.\n", id);

            while (1)
            {
                omp_set_lock(&bookLock);

                if (booksAvailable > 0)
                {
                    booksAvailable--;
                    printf("Borrower %d gets a book.\n", id);
                    printf("Books available = %d\n", booksAvailable);
                    omp_unset_lock(&bookLock);
                    break;
                }

                omp_unset_lock(&bookLock);
                usleep(10000);
            }

            usleep(10000);

            omp_set_lock(&bookLock);
            booksAvailable++;
            printf("Borrower %d is returning a book.\n", id);
            printf("Books available = %d\n", booksAvailable);
            omp_unset_lock(&bookLock);

            usleep(10000);
        }
    }

    omp_destroy_lock(&bookLock);
    return 0;
}
