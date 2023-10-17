#include <stdio.h>
#include "mpi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>


int main(int argc, char **argv)
{
    int thread_num;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &thread_num);

    if (thread_num == 0) {
        int a[10];
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < 10; i++) {
            a[i] = rand() % 100;
        }

        std::cout << "original array: ";
        for (int i = 0; i < 10; i++)
        {
            std::cout << a[i] << " ";
        }
        std::cout << std::endl;

        MPI_Send(a, 10, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (thread_num == 1) {
        int send_a[10];

        MPI_Recv(send_a, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "result array: ";
        for (int i = 0; i < 10; i++)
        {
            std::cout << send_a[i] << " ";
        }
        std::cout << std::endl;
    }


    MPI_Finalize();
    return 0;
}
