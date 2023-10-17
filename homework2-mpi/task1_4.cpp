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

    if (num_threads < 2) {
        printf("Необходимо минимум 2 потока");
        MPI_Finalize();
        return 1;
    }

    if (thread_num == 1) {
        int a[10];
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < 10; i++) {
            a[i] = rand() % 100;
        }

        std::cout << "original array: ";
        for (int i = 0; i < 10; i++)
        {
            printf("%d ", a[i]);
        }
        printf("\n");

        // отправляем остальным потокам
        for (int i = 0; i < num_threads; i++) {
            if (i != 1) {
                int send_count = 10;

                MPI_Send(&send_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

                MPI_Send(a, send_count, MPI_INT, i, 1, MPI_COMM_WORLD);
            }
        }

    } else {
        int recv_count;
        MPI_Status status;

        // принимаем кол-во данных
        MPI_Recv(&recv_count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

        // выделяем память для буффера
        int* recv_data = new int[recv_count];

        // принимаем данные
        MPI_Recv(recv_data, recv_count, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);

        std::cout << "recv array: ";
        for (int i = 0; i < recv_count; i++)
        {
            printf("%d ", recv_data[i]);
        }
        printf("\n");
    }
    

    MPI_Finalize();
    return 0;
}
