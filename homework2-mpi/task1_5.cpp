#include <stdio.h>
#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>


int main(int argc, char **argv)
{
    int thread_num;
    int num_threads;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &thread_num);
    MPI_Comm_size(MPI_COMM_WORLD, &num_threads);

    int number = thread_num * 10;

    if (thread_num == 0) {
        int* recv_nums = new int[num_threads];
        for (int source_rank = 1; source_rank < num_threads; source_rank++) {
            MPI_Recv(&recv_nums[source_rank], 1, MPI_INT, source_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        std::cout << "Числа в порядке возрастания процессов:" << std::endl;
        for (int i = 1; i < num_threads; i++) {
            std::cout << "Process " << i << ": " << recv_nums[i] << std::endl;
        }
    } else {
        // отправляем с любого потока на 0
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
