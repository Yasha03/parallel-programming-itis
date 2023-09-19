#include <stdio.h>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[])
{
    #pragma omp parallel num_threads(8)
    {
        printf("Hello world. Thread %d of %d \n", omp_get_thread_num(), omp_get_num_threads());
    }
    return 0;
}
