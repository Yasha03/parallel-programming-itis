#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>
#include <cmath>

int main(int argc, char** argv) {

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 120;
    const int PART = N/size;

    int x[N];
    int y[PART];

    if (rank == 0) {
        printf("x: ");
        for (int i = 0; i < N; i++) {
            x[i] = rand() % 100 + 1;
            printf("%d ", x[i]);
        }
        printf("\n\n");
    }

    MPI_Scatter(&x[0], PART, MPI_INT, &y[0], PART, MPI_INT, 0, MPI_COMM_WORLD);

    int max_loc = 0;
    for (int i = 0; i < PART; i++) {
        if (y[i] > max_loc) {
            max_loc = y[i];
        }
    }

    int local_result[2];
    int global_result[2];

    local_result[0] = max_loc;
    local_result[1] = rank;

    int result_all[20][2];

    MPI_Gather(local_result, 2, MPI_INT, result_all, 2, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Reduce(&local_result, &global_result, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            printf("Local maximum on process %d: %d\n", result_all[i][1], result_all[i][0]);
        }
        printf("Global maximum: %d, process number %d\n\n", global_result[0], global_result[1]);
    }

    MPI_Finalize();

    return 0;
}