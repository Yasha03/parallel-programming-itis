#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int count;
    int a = 10;
    int b = 12;
    const int SIZE = 15;
    const int PART = SIZE/3;

    if (rank == 0) {
        int x[SIZE];
        int y[SIZE];
        int z[SIZE];

        for (int i = 0; i < SIZE; ++i) {
            x[i] = rand() % 100;
            y[i] = rand() % 100;
        }

        printf("x: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", x[i]);
        }
        printf("\ny: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", y[i]);
        }
        printf("\n");

        int n = 1;
        for (int i = 0; i < SIZE; i += PART, n++) {
            MPI_Send(&x[i], PART, MPI_INT, n, 10, MPI_COMM_WORLD);
            MPI_Send(&y[i], PART, MPI_INT, n, 10, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < SIZE; i += PART, n++) {
            MPI_Recv(&z[i], PART, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
        }

        printf("z: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d ", z[i]);
        }
    }
    else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int x[PART];
        int y[PART];
        int z[PART];

        MPI_Recv(&x[0], count, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&y[0], count, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < count; ++i) {
            z[i] = a * x[i] + b * y[i];
        }
        MPI_Send(&z, count, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}