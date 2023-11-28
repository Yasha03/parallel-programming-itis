#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>
#include <cmath>

int main(int argc, char** argv) {
    int N = 100;

    int rank, size;
    double localDotProduct = 0.0;
    double globalDotProduct = 0.0;

    srand(time(nullptr));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int localSize = N / size;

    double globalVector1[N];
    double globalVector2[N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            globalVector1[i] = rand() % 100;
            globalVector2[i] = rand() % 100;
        }
        std::cout << "Vector 1: ";
        for (int i = 0; i < N; i++) {
            std::cout << globalVector1[i] << " ";
        }
        std::cout << std::endl;

        std::cout << "Vector 2: ";
        for (int i = 0; i < N; i++) {
            std::cout << globalVector2[i] << " ";
        }
        std::cout << std::endl;
    }

    double localVector1[N/size];
    double localVector2[N/size];

    MPI_Scatter(globalVector1, localSize, MPI_DOUBLE, localVector1, localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(globalVector2, localSize, MPI_DOUBLE, localVector2, localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < localSize; i++) {
        localDotProduct += localVector1[i] * localVector2[i];
    }

    MPI_Reduce(&localDotProduct, &globalDotProduct, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Result: " << globalDotProduct << std::endl;
    }

    MPI_Finalize();

    return 0;
}