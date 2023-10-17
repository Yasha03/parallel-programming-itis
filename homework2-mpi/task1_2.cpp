#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int thread_num;
    int num_threads;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &thread_num);
    MPI_Comm_size(MPI_COMM_WORLD, &num_threads);

    printf("Thread %d / %d \n", thread_num, num_threads);

    MPI_Finalize();
    return 0;
}
