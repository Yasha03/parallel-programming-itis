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
    srand(time(nullptr));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[N][N];
    int local_max = 0;

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = rand() % 100;
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    
    int local_rows = N / size;

    int local_matrix[N/size][N];

    MPI_Scatter(matrix, local_rows * N, MPI_INT, local_matrix, local_rows * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N; i++) {
        int row_sum = 0;
        for (int j = 0; j < N; j++) {
            row_sum += std::abs(matrix[j][i]);

        }
        if (row_sum > local_max) {
            local_max = row_sum;
        }
    }

    int global_max = 0;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Norm matrix: " << global_max << std::endl;
    }

    MPI_Finalize();
    return 0;
}
