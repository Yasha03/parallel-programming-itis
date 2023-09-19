#include <stdio.h>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int a = 10;
    int b = 20;

    printf("Before. a = %d, b = %d\n", a, b);

    #pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        int thread_id = omp_get_thread_num();
        a += thread_id;
        b += thread_id;
        
        printf("Thread: %d. a = %d, b = %d\n", thread_id, a, b);
    }

    printf("After first space. a = %d, b = %d\n", a, b);

    #pragma omp parallel num_threads(4) shared(a) private(b)
    {
        int thread_id = omp_get_thread_num();
        a -= thread_id;
        b -= thread_id;

        printf("Thread: %d. a = %d, b = %d\n", thread_id, a, b);
    }
    printf("After second space. a = %d, b = %d\n", a, b);

    return 0;
}

/*
private - для каждого потока своя копия
firstprivate - для каждого потока своя копия с значением извне параллельной ообласти
shared - общая для всех потоков
*/