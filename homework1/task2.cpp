#include <stdio.h>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int threads_num = 3;

    #pragma omp parallel if(threads_num > 1)
    {
        printf("Space 1. Thread %d of %d \n", omp_get_thread_num(), omp_get_num_threads());
    }

    threads_num = 1;

    #pragma omp parallel if(threads_num > 1)
    {
        printf("Space 2. Thread %d of %d \n", omp_get_thread_num(), omp_get_num_threads());
    }

    return 0;
}
