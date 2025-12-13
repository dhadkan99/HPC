#include <stdio.h>
#include <omp.h>
#include <unistd.h>

int booksAvailable = 3;

int main(void)
{
    omp_set_num_threads(5);

    #pragma omp parallel
    {
        int id = omp_get_thread_num() + 1;

        for (int i = 0; i < 10; i++)
        {
            int success = 0;

            printf("Borrower %d wants to borrow a book.\n", id);

            while (!success)
            {
                int oldValue;

                #pragma omp atomic capture
                oldValue = booksAvailable--;

                if (oldValue > 0)
                {
                    printf("Borrower %d gets a book.\n", id);
                    printf("Books available = %d\n", booksAvailable);
                    success = 1;
                }
                else
                {
                    
                    #pragma omp atomic
                    booksAvailable++;

                    usleep(10000);
                }
            }

            usleep(10000);

            #pragma omp atomic
            booksAvailable++;

            printf("Borrower %d is returning a book. ", id);
            printf("Books available = %d\n", booksAvailable);

            usleep(10000);
        }
    }

    return 0;
}
