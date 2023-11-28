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
    double localNorm = 0.0;
    double globalNorm = 0.0;

    srand(time(nullptr));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int localSize = N / size;
    double globalVector[N];

    if (rank == 0) {
        std::cout << "Verctor: ";
        for (int i = 0; i < N; i++) {
            globalVector[i] = rand() % 100;
            std::cout << globalVector[i] << " ";
        }
        std::cout << std::endl;
    }

    double localVector[20];

    MPI_Scatter(globalVector, localSize, MPI_DOUBLE, localVector, localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < localSize; i++) {
        localNorm += std::fabs(localVector[i]);
    }

    MPI_Reduce(&localNorm, &globalNorm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        globalNorm = globalNorm;
        std::cout << "Norm: " << globalNorm << std::endl;
    }

    MPI_Finalize();

    return 0;
}